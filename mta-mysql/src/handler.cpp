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
#include "handler.h"


/**
 ** Tool functions
 **/
static struct
{
  const char* name;
  unsigned long flag;
} client_flags[] = {
  { "compress",         CLIENT_COMPRESS },
  { "found_rows",       CLIENT_FOUND_ROWS },
  { "ignore_sigpipe",   CLIENT_IGNORE_SIGPIPE },
  { "ignore_space",     CLIENT_IGNORE_SPACE },
  { "interactive",      CLIENT_INTERACTIVE },
  { "local_files",      CLIENT_LOCAL_FILES },
  { "no_schema",        CLIENT_NO_SCHEMA },
  { "odbc",             CLIENT_ODBC },
  { 0,                  0 }
};

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(WIN32)
static inline int strncasecmp(const char* s1, const char* s2, size_t n)
{
  if (n == 0)
    return 0;

  while (n-- != 0 && tolower(*s1) == tolower(*s2))
    {
      if (n == 0 || *s1 == '\0' || *s2 == '\0')
        break;
      s1++;
      s2++;
    }

  return tolower(*(unsigned char *) s1) - tolower(*(unsigned char *) s2);
}
#endif

static inline unsigned long getClientFlag(const char* flagStr, size_t len)
{
  unsigned long ret = 0;

  for (int i = 0; client_flags[i].name != 0; i++)
  {
    if (!strncasecmp(client_flags[i].name, flagStr, len))
    {
      ret = client_flags[i].flag;
      break;
    }
  }

  return ret;
}

static inline MySQL* checkMySQLHandler(lua_State* L)
{
  void *ud = luaL_checkudata(L, 1, "LuaBook.mysqlHandler");
  MySQL* handler = *((MySQL **)ud);
  luaL_argcheck(L, handler != 0 && handler->OK() == true, 1, "Expected a valid MySQL link");

  return handler;
}


/**
 ** Module interface functions
 **/

/** Metatable functions for this module userdatas **/
int MySQL::HandlerGC(lua_State* L)
{
  void *ud = luaL_checkudata(L, 1, "LuaBook.mysqlHandler");

  if (ud != 0 && *((MySQL **)ud) != 0)
  {
    MySQL* handler = *((MySQL **)ud);
    delete handler;
  }

  return 0;
}

int MySQL::HandlerTostring(lua_State* L)
{
  void *ud = luaL_checkudata(L, 1, "LuaBook.mysqlHandler");

  if (ud != 0 && *((MySQL **)ud) != 0)
  {
    lua_pushstring(L, "MySQL connection");
  }
  else
  {
    lua_pushnil(L);
  }
  return 1;
}

int MySQL::ResultGC(lua_State* L)
{
  void *ud = luaL_checkudata(L, 1, "LuaBook.mysqlResult");

  if (ud != 0 && *((MySQL_Result**)ud) != 0)
  {
    MySQL_Result* result = *((MySQL_Result **)ud);
    delete result;
  }

  return 0;
}

int MySQL::ResultTostring(lua_State* L)
{
  void *ud = luaL_checkudata(L, 1, "LuaBook.mysqlResult");

  if (ud != 0 && *((MySQL_Result**)ud) != 0)
  {
    lua_pushstring(L, "MySQL result");
  }
  else
  {
    lua_pushnil(L);
  }
  return 1;
}



/** Module startup, create the metatables **/
void MySQL::Startup(lua_State* L)
{
  luaL_newmetatable(L, "LuaBook.mysqlHandler");
  lua_pushstring(L, "__gc");
  lua_pushcfunction(L, MySQL::HandlerGC);
  lua_settable(L, -3);
  lua_pushstring(L, "__tostring");
  lua_pushcfunction(L, MySQL::HandlerTostring);
  lua_settable(L, -3);

  luaL_newmetatable(L, "LuaBook.mysqlResult");
  lua_pushstring(L, "__gc");
  lua_pushcfunction(L, MySQL::ResultGC);
  lua_settable(L, -3);
  lua_pushstring(L, "__tostring");
  lua_pushcfunction(L, MySQL::ResultTostring);
  lua_settable(L, -3);
}

/**
 ** Connect - Connects to a MySQL database
 **
 ** Parameters:
 **   1 - hostname
 **   2 - username
 **   3 - password
 **   4 - database
 ** Optional parameters:
 **   5 - port
 **   6 - unix socket
 **   7 - client flags, separated by pipes (|), where they are:
 **       compress         - CLIENT_COMPRESS
 **       found_rows       - CLIENT_FOUND_ROWS
 **       ignore_sigpipe   - CLIENT_IGNORE_SIGPIPE
 **       ignore_space     - CLIENT_IGNORE_SPACE
 **       interactive      - CLIENT_INTERACTIVE
 **       local_files      - CLIENT_LOCAL_FILES
 **       no_schema        - CLIENT_NO_SCHEMA
 **       odbc             - CLIENT_ODBC
 **    Refer to http://dev.mysql.com/doc/refman/5.0/en/mysql-real-connect.html
 **    for more information about the client flags.
 **/
int MySQL::Connect(lua_State* L)
{
  int numparams = lua_gettop(L);
  if (numparams < 4)
    return luaL_error(L, "Wrong parameter count for `mysql_connect'");

  const char* hostname      = luaL_checkstring(L, 1);
  const char* username      = luaL_checkstring(L, 2);
  const char* password      = luaL_checkstring(L, 3);
  const char* database      = 0;
  unsigned int port         = 3306;
  const char* unixSocket    = 0;
  unsigned long clientFlags = 0;

  if (numparams >= 4)
  {
    database = luaL_checkstring(L, 4);
    if (numparams >= 5)
    {
      port = (unsigned int)luaL_checkint(L, 5);
      if (numparams >= 6)
      {
        if (lua_type(L, 6) != LUA_TNIL)
          unixSocket = luaL_checkstring(L, 6);

        if (numparams >= 7)
        {
          /* Extract the client flags */
          const char* p = luaL_checkstring(L, 7);
          const char* s;

          while ((s = strchr(p, '|')) != 0)
          {
            clientFlags |= getClientFlag(p, (size_t)(s - p));
            p = s + 1;
          }
          if (*p != '\0')
            clientFlags |= getClientFlag(p, strlen(p));
        }
      }
    }
  }

  MySQL* handler = new MySQL(hostname, username, password, database, port, unixSocket, clientFlags);
  if (handler->OK() == false)
  {
    char szError [ 1024 ];
    snprintf ( szError, sizeof ( szError ), "Unable to connect to mysql: (%d) %s\n", handler->Errno(), handler->Error() );
    pModuleManager->ErrorPrintf( szError );
    delete handler;
    lua_pushnil(L);
  }
  else
  {
    MySQL** pResource = (MySQL**)lua_newuserdata(L, sizeof(MySQL*));
    luaL_getmetatable(L, "LuaBook.mysqlHandler");
    lua_setmetatable(L, -2);
    *pResource = handler;
  }

  return 1;
}


/**
 ** Close - Closes a MySQL database connection
 **
 ** Parameters:
 **   1 - A valid MySQL handler
 **/
int MySQL::Close(lua_State* L)
{
  void *ud = luaL_checkudata(L, 1, "LuaBook.mysqlHandler");
  MySQL* handler = *((MySQL **)ud);

  luaL_argcheck(L, handler != 0 && handler->OK() == true, 1, "Expected a valid MySQL link");
  delete handler;
  *((MySQL **)ud) = 0;

  return 0;
}


/**
 ** Errno - Returns the last error number
 **
 ** Parameters:
 **   1 - A valid MySQL handler
 **/
int MySQL::Errno(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushinteger(L, handler->Errno());

  return 1;
}


/**
 ** Error - Returns the last error string
 **
 ** Parameters:
 **   1 - A valid MySQL handler
 **/
int MySQL::Error(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushstring(L, handler->Error());

  return 1;
}

/**
 ** Ping - Sends a ping to the database to check if the connection is alive
 **
 ** Parameters:
 **   1 - A valid MySQL handler
 **/
int MySQL::Ping(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushboolean(L, handler->Ping());

  return 1;
}


/**
 ** SelectDB - Selects a database in a valid MySQL link
 **
 ** Parameters:
 **   1 - A valid MySQL handler
 **   2 - The database name
 **/
int MySQL::SelectDB(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  const char* dbname = luaL_checkstring(L, 2);

  lua_pushboolean(L, handler->SelectDB(dbname));
  return 1;
}



/**
 ** EscapeString - Escapes a string ready to execute a query
 **
 ** Parameters:
 **   1 - A valid MySQL handler
 **   2 - The string to escape
 **/
int MySQL::EscapeString(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  size_t len;
  const char* str = luaL_checklstring(L, 2, &len);

  char* res = new char[(len << 1) + 1];
  size_t newlen = handler->EscapeString(str, res, len);
  lua_pushlstring(L, res, newlen);

  delete [] res;

  return 1;
}


/**
 ** AffectedRows - Returns the number of afected rows in the last query
 **
 ** Parameters:
 **   1 - A valid MySQL handler
 **/
int MySQL::AffectedRows(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);

  lua_pushinteger(L, handler->AffectedRows());
  return 1;
}


/**
 ** ChangeUSer - Changes the current session user
 **
 ** Parameters:
 **   1 - A valid MySQL handler
 **   2 - New username
 **   3 - New username password
 ** Optional parameters:
 **   4 - New database
 **/
int MySQL::ChangeUser(lua_State* L)
{
  int numargs = lua_gettop(L);
  MySQL* handler = checkMySQLHandler(L);
  const char* username = luaL_checkstring(L, 2);
  const char* password = luaL_checkstring(L, 3);
  const char* database = 0;
  if (numargs >= 4)
    database = luaL_checkstring(L, 4);

  lua_pushboolean(L, handler->ChangeUser(username, password, database));
  return 1;
}


/**
 ** GetCharacterSetInfo
 **
 ** Parameters:
 **   1 - A valid MySQL handler
 **/
int MySQL::GetCharacterSetInfo(lua_State* L)
{
  MY_CHARSET_INFO cs;
  MySQL* handler = checkMySQLHandler(L);
  handler->GetCharacterSetInfo(&cs);

  lua_pushstring(L, cs.name);
  lua_pushstring(L, cs.csname);
  lua_pushstring(L, cs.comment);
  lua_pushstring(L, cs.dir);
  lua_pushinteger(L, cs.mbminlen);
  lua_pushinteger(L, cs.mbmaxlen);

  return 6;
}


/**
 ** GetClientInfo
 **/
int MySQL::GetClientInfo(lua_State* L)
{
  lua_pushstring(L, mysql_get_client_info());
  return 1;
}


/**
 ** GetClientVersion
 **/
int MySQL::GetClientVersion(lua_State* L)
{
  lua_pushinteger(L, mysql_get_client_version());
  return 1;
}


/**
 ** GetHostInfo
 **
 ** Parameters:
 **   1 - A valid MySQL link
 **/
int MySQL::GetHostInfo(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushstring(L, handler->GetHostInfo());
  return 1;
}


/**
 ** GetProtoInfo
 **
 ** Parameters:
 **   1 - A valid MySQL link
 **/
int MySQL::GetProtoInfo(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushinteger(L, handler->GetProtoInfo());
  return 1;
}


/**
 ** GetServerInfo
 **
 ** Parameters:
 **   1 - A valid MySQL link
 **/
int MySQL::GetServerInfo(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushstring(L, handler->GetServerInfo());
  return 1;
}


/**
 ** GetServerVersion
 **
 ** Parameters:
 **   1 - A valid MySQL link
 **/
int MySQL::GetServerVersion(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushinteger(L, handler->GetServerVersion());
  return 1;
}


/**
 ** HexString
 **
 ** Parameters:
 **   1 - The source string
 **/
int MySQL::HexString(lua_State* L)
{
  size_t len;
  const char* str = luaL_checklstring(L, 1, &len);

  char* dest = new char[(len << 1) + 1];
  size_t newlen = static_cast < size_t > ( mysql_hex_string(dest, str, static_cast < unsigned int > ( len )) );

  lua_pushlstring(L, dest, newlen);
  delete [] dest;

  return 1;
}


/**
 ** Info
 **
 ** Parameters:
 **   1 - A valid MySQL link
 **/
int MySQL::Info(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushstring(L, handler->Info());
  return 1;
}

/**
 ** InsetID
 **
 ** Parameters:
 **   1 - A valid MySQL link
 **/
int MySQL::InsertID(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushinteger(L, static_cast < lua_Integer > ( handler->InsertID() ));
  return 1;
}


/**
 ** Query
 **
 ** Parameters:
 **   1 - A valid MySQL link
 **   2 - The query
 **/
int MySQL::Query(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  size_t len;
  const char* query = luaL_checklstring(L, 2, &len);

  MySQL_Result* result = handler->Query(query, len);
  if (!result)
    lua_pushnil(L);
  else
  {
    MySQL_Result** pResult = (MySQL_Result**)lua_newuserdata(L, sizeof(MySQL_Result*));
    luaL_getmetatable(L, "LuaBook.mysqlResult");
    lua_setmetatable(L, -2);
    *pResult = result;
  }
  return 1;
}

int MySQL::UnbufferedQuery(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  size_t len;
  const char* query = luaL_checklstring(L, 2, &len);

  MySQL_Result* result = handler->UnbufferedQuery(query, len);
  if (!result)
    lua_pushnil(L);
  else
  {
    MySQL_Result** pResult = (MySQL_Result**)lua_newuserdata(L, sizeof(MySQL_Result*));
    luaL_getmetatable(L, "LuaBook.mysqlResult");
    lua_setmetatable(L, -2);
    *pResult = result;
  }

  return 1;
}


/**
 ** SetCharacterSet
 **
 ** Parameters:
 **   1 - A valid MySQL link
 **   2 - Character set name
 **/
int MySQL::SetCharacterSet(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  const char* csname = luaL_checkstring(L, 2);

  lua_pushboolean(L, handler->SetCharacterSet(csname));
  return 1;
}


/**
 ** Stat
 **
 ** Parameters:
 **   1 - A valid MySQL link
 **/
int MySQL::Stat(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushstring(L, handler->Stat());
  return 1;
}


/**
 ** WarningCount
 **
 ** Parameters:
 **   1 - A valid MySQL link
 **/
int MySQL::WarningCount(lua_State* L)
{
  MySQL* handler = checkMySQLHandler(L);
  lua_pushinteger(L, handler->WarningCount());
  return 1;
}



/**
 ** MySQL handler
 **/
MySQL::~MySQL()
{
  Close();
}

MySQL::MySQL(const char* hostname,
             const char* username,
             const char* password,
             const char* database,
             unsigned int port,
             const char* unixSocket,
             unsigned long clientFlags)
  : m_ok(false)
{
  m_handle = mysql_init(0);
  if (m_handle)
  {
    my_bool reconnect = false;
    mysql_options(m_handle, MYSQL_OPT_RECONNECT, &reconnect);

    if (mysql_real_connect(m_handle, hostname, username, password, database, port, unixSocket, clientFlags))
      m_ok = true;
  }
}

unsigned int MySQL::Errno()
{
  return mysql_errno(m_handle);
}

const char* MySQL::Error()
{
  return mysql_error(m_handle);
}

void MySQL::Close()
{
  if (m_handle != 0)
  {
     mysql_close(m_handle);
     m_handle = 0;
     m_ok = false;
  }
}

bool MySQL::Ping()
{
  if (mysql_ping(m_handle))
    return false;
  return true;
}

bool MySQL::SelectDB(const char* dbname)
{
  if (mysql_select_db(m_handle, dbname))
    return false;
  return true;
}

size_t MySQL::EscapeString(const char* str, char* dest, size_t len)
{
  return mysql_real_escape_string(m_handle, dest, str, static_cast < unsigned long > ( len ));
}

unsigned long MySQL::AffectedRows()
{
  return static_cast < unsigned long > ( mysql_affected_rows(m_handle) );
}

bool MySQL::ChangeUser(const char* username, const char* password, const char* database)
{
  if (mysql_change_user(m_handle, username, password, database))
    return false;
  return true;
}

void MySQL::GetCharacterSetInfo(MY_CHARSET_INFO* cs)
{
  mysql_get_character_set_info(m_handle, cs);
}

const char* MySQL::GetHostInfo()
{
  return mysql_get_host_info(m_handle);
}

unsigned long MySQL::GetProtoInfo()
{
  return mysql_get_proto_info(m_handle);
}

const char* MySQL::GetServerInfo()
{
  return mysql_get_server_info(m_handle);
}

unsigned long MySQL::GetServerVersion()
{
  return mysql_get_server_version(m_handle);
}

const char* MySQL::Info()
{
  return mysql_info(m_handle);
}

unsigned long long MySQL::InsertID()
{
  return mysql_insert_id(m_handle);
}

MySQL_Result* MySQL::Query(const char* query, size_t len)
{
  MYSQL_RES* res;
  MySQL_Result* result;

  if (mysql_real_query(m_handle, query, static_cast < unsigned long > ( len )))
    return 0;
  res = mysql_store_result(m_handle);
  if (res == 0 && mysql_field_count(m_handle) > 0)
    return 0;
  
  result = new MySQL_Result(res);
  return result;
}

MySQL_Result* MySQL::UnbufferedQuery(const char* query, size_t len)
{
  MYSQL_RES* res;
  MySQL_Result* result;

  if (mysql_real_query(m_handle, query, static_cast < unsigned long > ( len )))
    return 0;
  res = mysql_use_result(m_handle);
  if (res == 0 && mysql_field_count(m_handle) > 0)
    return 0;

  result = new MySQL_Result(res);
  return result;
}

bool MySQL::SetCharacterSet(const char* csname)
{
  if (mysql_set_character_set(m_handle, csname))
    return false;
  return true;
}

const char* MySQL::Stat()
{
  return mysql_stat(m_handle);
}

unsigned long MySQL::WarningCount()
{
  return mysql_warning_count(m_handle);
}


