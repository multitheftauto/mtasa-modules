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

#ifndef __RESULT_H
#define __RESULT_H

#include <mysql.h>

class MySQL_Result
{
  friend class MySQL;

  /**
   ** Static interface functions
   **/
private:
  static unsigned int GetNextResult ()
  {
    static unsigned int s_num = 0;
    return s_num++;
  }

public:
  static int DataSeek(lua_State* L);
  static int FetchField(lua_State* L);
  static int Fields(lua_State* L);
  static int FetchLengths(lua_State* L);
  static int FetchRow(lua_State* L);
  static int Rows(lua_State* L);
  static int FetchAssoc(lua_State* L);
  static int RowsAssoc(lua_State* L);
  static int FieldLength(lua_State* L);
  static int FieldName(lua_State* L);
  static int FieldSeek(lua_State* L);
  static int FieldTell(lua_State* L);
  static int NumFields(lua_State* L);
  static int NumRows(lua_State* L);
  static int Result(lua_State* L);
  static int FreeResult(lua_State* L);
  static int Null(lua_State* L);


private:
  MySQL_Result(MYSQL_RES* res);
public:
  ~MySQL_Result();
  bool Empty();

  void DataSeek(unsigned long long offset);
  MYSQL_FIELD* FetchField();
  unsigned long* FetchLengths();
  MYSQL_ROW FetchRow();
  unsigned int FieldSeek(unsigned int offset);
  unsigned int FieldTell();
  unsigned int NumFields();
  unsigned long long NumRows();

  unsigned int GetNumResult () const { return m_numResult; }

private:
  MYSQL_RES* m_result;
  unsigned int m_numResult;
};

#endif /* #ifndef __RESULT_H */
