/*
 * Copyright (c) 2008, Alberto Alonso
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this
 *       list of conditions and the following disclaimer in the documentation and/or other
 *       materials provided with the distribution.
 *     * Neither the name of the mta-mysql nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without specific prior
 *       written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32)
#include <windows.h>
#include <winsock.h>
#endif
#include <mysql.h>
#include "module.h"
#include "result.h"

/**
 ** Tool functions
 **/
static inline MySQL_Result* checkMySQLResult(lua_State* L)
{
  void *ud = luaL_checkudata(L, 1, "LuaBook.mysqlResult");
  MySQL_Result* result = *((MySQL_Result **)ud);

  luaL_argcheck(L, result != 0, 1, "Expected a valid MySQL result");

  return result;
}

/* Obviously, taken from PHP source code */
static inline const char *php_mysql_get_field_name(int field_type)
{
  switch(field_type) {
    case FIELD_TYPE_STRING:
    case FIELD_TYPE_VAR_STRING:
      return "string";
      break;
#ifdef MYSQL_HAS_TINY
    case FIELD_TYPE_TINY:
#endif
    case FIELD_TYPE_SHORT:
    case FIELD_TYPE_LONG:
    case FIELD_TYPE_LONGLONG:
    case FIELD_TYPE_INT24:
      return "int";
      break;
    case FIELD_TYPE_FLOAT:
    case FIELD_TYPE_DOUBLE:
    case FIELD_TYPE_DECIMAL:
#ifdef FIELD_TYPE_NEWDECIMAL
    case FIELD_TYPE_NEWDECIMAL:
#endif
      return "real";
      break;
    case FIELD_TYPE_TIMESTAMP:
      return "timestamp";
      break;
#ifdef MYSQL_HAS_YEAR
    case FIELD_TYPE_YEAR:
      return "year";
      break;
#endif
    case FIELD_TYPE_DATE:
#ifdef FIELD_TYPE_NEWDATE
    case FIELD_TYPE_NEWDATE:
#endif
      return "date";
      break;
    case FIELD_TYPE_TIME:
      return "time";
      break;
    case FIELD_TYPE_SET:
      return "set";
      break;
    case FIELD_TYPE_ENUM:
      return "enum";
      break;
#ifdef FIELD_TYPE_GEOMETRY
    case FIELD_TYPE_GEOMETRY:
      return "geometry";
      break;
#endif
    case FIELD_TYPE_DATETIME:
      return "datetime";
      break;
    case FIELD_TYPE_TINY_BLOB:
    case FIELD_TYPE_MEDIUM_BLOB:
    case FIELD_TYPE_LONG_BLOB:
    case FIELD_TYPE_BLOB:
      return "blob";
      break;
    case FIELD_TYPE_NULL:
      return "null";
      break;
    default:
      return "unknown";
      break;
  }
}

static inline void PushFieldData(lua_State* L, MYSQL_FIELD* field)
{
  lua_createtable(L, 0, 14);
  lua_pushstring(L, "name");
  lua_pushstring(L, field->name);
  lua_settable(L, -3);
  lua_pushstring(L, "org_name");
  lua_pushstring(L, field->org_name);
  lua_settable(L, -3);
  lua_pushstring(L, "table");
  lua_pushstring(L, field->table);
  lua_settable(L, -3);
  lua_pushstring(L, "org_table");
  lua_pushstring(L, field->org_table);
  lua_settable(L, -3);
  lua_pushstring(L, "length");
  lua_pushinteger(L, field->length);
  lua_settable(L, -3);
  lua_pushstring(L, "max_length");
  lua_pushinteger(L, field->max_length);
  lua_settable(L, -3);
  lua_pushstring(L, "not_null");
  lua_pushboolean(L, (field->flags & NOT_NULL_FLAG));
  lua_settable(L, -3);
  lua_pushstring(L, "primary_key");
  lua_pushboolean(L, (field->flags & PRI_KEY_FLAG));
  lua_settable(L, -3);
  lua_pushstring(L, "unique_key");
  lua_pushboolean(L, (field->flags & UNIQUE_KEY_FLAG));
  lua_settable(L, -3);
  lua_pushstring(L, "multiple_key");
  lua_pushboolean(L, (field->flags & MULTIPLE_KEY_FLAG));
  lua_settable(L, -3);
  lua_pushstring(L, "numeric");
  lua_pushboolean(L, IS_NUM(field->flags));
  lua_settable(L, -3);
  lua_pushstring(L, "blob");
  lua_pushboolean(L, IS_BLOB(field->flags));
  lua_settable(L, -3);
  lua_pushstring(L, "type");
  lua_pushstring(L, php_mysql_get_field_name(field->type));
  lua_settable(L, -3);
  lua_pushstring(L, "unsigned");
  lua_pushboolean(L, (field->flags & UNSIGNED_FLAG));
  lua_settable(L, -3);
  lua_pushstring(L, "zerofill");
  lua_pushboolean(L, (field->flags & ZEROFILL_FLAG));
  lua_settable(L, -3);
}

static char* get_null()
{
  static char c = 0;
  return &c;
}

static inline void PushRowData(lua_State* L, MYSQL_ROW row, unsigned int numfields)
{
  lua_createtable(L, 0, numfields);
  for (unsigned int i = 0; i < numfields; i++)
  {
    if (row[i])
    {
      lua_pushstring(L, row[i]);
    }
    else
    {
      lua_pushlightuserdata(L, get_null());
    }
    lua_rawseti(L, -2, i + 1);
  }
}

static inline void PushRowAssoc(lua_State* L, MYSQL_ROW row, MySQL_Result* result)
{
  unsigned int numfields = result->NumFields();
  lua_createtable(L, 0, numfields);

  result->FieldSeek((unsigned int)0);
  MYSQL_FIELD* field;
  int i;
  for (field = result->FetchField(), i = 0; field != 0; field = result->FetchField(), i++)
  {
    lua_pushstring(L, field->name);
    if (row[i])
    {
      lua_pushstring(L, row[i]);
    }
    else
    {
      lua_pushlightuserdata(L, get_null());
    }
    lua_settable(L, -3);
  }
}


/**
 ** Static interface functions
 **/
int MySQL_Result::DataSeek(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);
  unsigned long long offset = luaL_checkinteger(L, 2);

  if (!result->Empty())
    result->DataSeek(offset);

  return 0;
}

int MySQL_Result::FetchField(lua_State* L)
{
  int numargs = lua_gettop(L);
  MySQL_Result* result = checkMySQLResult(L);

  if (result->Empty())
  {
    lua_pushnil(L);
    return 1;
  }

  if (numargs > 1 && lua_type(L, 2) != LUA_TNIL)
  {
    int seek = static_cast < int > ( luaL_checkinteger(L, 2) );

    if (seek < 0 || seek > static_cast < int > ( result->NumFields() ))
      return luaL_error(L, "Bad offset in mysql_fetch_field");

    result->FieldSeek(seek);
  }

  MYSQL_FIELD* field = result->FetchField();
  if (field)
    PushFieldData(L, field);
  else
    lua_pushnil(L);

  return 1;
}

static int MySQL_Result_Fields_Iterator(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);
  MYSQL_FIELD* field = result->FetchField();

  if (field)
  {
    lua_pushvalue(L, 1);
    PushFieldData(L, field);
  }
  else
  {
    lua_pushnil(L);
    lua_pushnil(L);
  }
  
  return 2;
}

int MySQL_Result::Fields(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);

  result->FieldSeek((unsigned int)0);
  lua_pushcfunction(L, MySQL_Result_Fields_Iterator);
  lua_pushvalue(L, 1);

  return 2;
}

int MySQL_Result::FetchLengths(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);

  if (result->Empty())
  {
    lua_pushnil(L);
    return 1;
  }

  unsigned long* lengths = result->FetchLengths();
  if (lengths)
  {
    unsigned int numfields = result->NumFields();

    lua_createtable(L, 0, numfields);
    for (unsigned int i = 0; i < numfields; i++)
    {
      lua_pushinteger(L, lengths[i]);
      lua_rawseti(L, -2, i + 1);
    }
  }
  else
    lua_pushnil(L);

  return 1;
}

int MySQL_Result::FreeResult(lua_State* L)
{
  void *ud = luaL_checkudata(L, 1, "LuaBook.mysqlResult");
  MySQL_Result* result = *((MySQL_Result **)ud);

  if (result != 0)
  {
    delete result;
    *((MySQL_Result **)ud) = 0;
  }

  return 0;
}

int MySQL_Result::FetchRow(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);

  if (result->Empty())
  {
    lua_pushnil(L);
    return 1;
  }

  MYSQL_ROW row = result->FetchRow();
  if (!row)
    lua_pushnil(L);
  else
    PushRowData(L, row, result->NumFields());

  return 1;
}

static int MySQL_Result_Rows_Iterator(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);
  MYSQL_ROW row = result->FetchRow();

  if (row)
  {
    lua_pushvalue(L, 1);
    PushRowData(L, row, result->NumFields());
  }
  else
  {
    lua_pushnil(L);
    lua_pushnil(L);
  }
  
  return 2;
}

int MySQL_Result::Rows(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);

  if (!result->Empty())
    result->DataSeek((unsigned long long)0);
  lua_pushcfunction(L, MySQL_Result_Rows_Iterator);
  lua_pushvalue(L, 1);

  return 2;
}

int MySQL_Result::FetchAssoc(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);

  if (result->Empty())
  {
    lua_pushnil(L);
    return 1;
  }

  MYSQL_ROW row = result->FetchRow();
  if (!row)
    lua_pushnil(L);
  else
    PushRowAssoc(L, row, result);

  return 1;
}

static int MySQL_Result_Rows_Assoc_Iterator(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);
  MYSQL_ROW row = result->FetchRow();

  if (row)
  {
    lua_pushvalue(L, 1);
    PushRowAssoc(L, row, result);
  }
  else
  {
    lua_pushnil(L);
    lua_pushnil(L);
  }

  return 2;
}

int MySQL_Result::RowsAssoc(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);

  if (!result->Empty())
    result->DataSeek((unsigned long long)0);
  lua_pushcfunction(L, MySQL_Result_Rows_Assoc_Iterator);
  lua_pushvalue(L, 1);

  return 2;
}

int MySQL_Result::FieldLength(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);
  unsigned int offset = static_cast < unsigned int > ( luaL_checkinteger(L, 2) );

  if (result->Empty())
  {
    lua_pushnil(L);
    return 1;
  }

  if (offset < 1 || offset > result->NumFields())
  {
    pModuleManager->ErrorPrintf("Invalid offset in mysql_field_length\n");
    lua_pushnil(L);
    return 1;
  }

  unsigned long* lengths = result->FetchLengths();
  lua_pushinteger(L, lengths[offset - 1]);

  return 1;
}

int MySQL_Result::FieldName(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);
  unsigned int offset = static_cast < unsigned int > ( luaL_checkinteger(L, 2) );

  if (result->Empty())
  {
    lua_pushnil(L);
    return 1;
  }

  if (offset < 1 || offset > result->NumFields())
  {
    pModuleManager->ErrorPrintf("Invalid offset in mysql_field_name\n");
    lua_pushnil(L);
    return 1;
  }

  result->FieldSeek(offset - 1);
  MYSQL_FIELD* field = result->FetchField();
  if (!field)
    lua_pushnil(L);
  else
    lua_pushstring(L, field->name);

  return 1;
}

int MySQL_Result::FieldSeek(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);
  unsigned int offset = static_cast < unsigned int > ( luaL_checkinteger(L, 2) );

  if (result->Empty())
  {
    lua_pushnil(L);
    return 1;
  }

  if (offset < 1 || offset > result->NumFields())
  {
    pModuleManager->ErrorPrintf("Invalid offset in mysql_field_seek\n");
    lua_pushnil(L);
    return 1;
  }

  unsigned int oldoffset = result->FieldSeek(offset - 1);
  lua_pushinteger(L, oldoffset + 1);

  return 1;
}

int MySQL_Result::FieldTell(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);

  if (result->Empty())
  {
    lua_pushnil(L);
    return 1;
  }

  unsigned int offset = result->FieldTell();
  lua_pushinteger(L, offset + 1);

  return 1;
}

int MySQL_Result::NumFields(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);

  if (result->Empty())
    lua_pushinteger(L, 0);
  else
    lua_pushinteger(L, result->NumFields());

  return 1;
}

int MySQL_Result::NumRows(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);

  if (result->Empty())
  {
    lua_pushinteger(L, 0);
    return 1;
  }

  unsigned long long numrows = result->NumRows();
  lua_pushinteger(L, static_cast < lua_Integer > ( numrows ));

  return 1;
}

int MySQL_Result::Result(lua_State* L)
{
  MySQL_Result* result = checkMySQLResult(L);
  unsigned long long rowoffset = luaL_checkinteger(L, 2);
  unsigned int fieldoffset = static_cast < unsigned int > ( luaL_checkinteger(L, 3) );

  if (result->Empty())
  {
    lua_pushnil(L);
    return 1;
  }

  if (rowoffset < 1 || rowoffset > result->NumRows())
  {
    pModuleManager->ErrorPrintf("Invalid row offset in mysql_result\n");
    lua_pushnil(L);
    return 1;
  }
  if (fieldoffset < 1 || fieldoffset > result->NumFields())
  {
    pModuleManager->ErrorPrintf("Invalid field offset in mysql_result\n");
    lua_pushnil(L);
    return 1;
  }

  result->DataSeek(rowoffset - 1);
  MYSQL_ROW row = result->FetchRow();
  if (!row)
    lua_pushnil(L);
  else
  {
    if (row[fieldoffset - 1])
      lua_pushstring(L, row[fieldoffset - 1]);
    else
    {
      lua_pushlightuserdata(L, get_null());
    }
  }

  return 1;
}

int MySQL_Result::Null(lua_State* L)
{
  lua_pushlightuserdata(L, get_null());

  return 1;
}






/**
 ** MySQL result
 **/
MySQL_Result::MySQL_Result(MYSQL_RES* res)
  : m_result(res)
{
}

MySQL_Result::~MySQL_Result()
{
  if (m_result != 0)
    mysql_free_result(m_result);
}

bool MySQL_Result::Empty()
{
  if (m_result)
    return false;
  return true;
}

void MySQL_Result::DataSeek(unsigned long long offset)
{
  mysql_data_seek(m_result, offset);
}

MYSQL_FIELD* MySQL_Result::FetchField()
{
  return mysql_fetch_field(m_result);
}

unsigned long* MySQL_Result::FetchLengths()
{
  return mysql_fetch_lengths(m_result);
}

MYSQL_ROW MySQL_Result::FetchRow()
{
  if (m_result)
    return mysql_fetch_row(m_result);
  return 0;
}

unsigned int MySQL_Result::FieldSeek(unsigned int offset)
{
  return mysql_field_seek(m_result, offset);
}

unsigned int MySQL_Result::FieldTell()
{
  return mysql_field_tell(m_result);
}

unsigned int MySQL_Result::NumFields()
{
  return mysql_num_fields(m_result);
}

unsigned long long MySQL_Result::NumRows()
{
  return mysql_num_rows(m_result);
}

