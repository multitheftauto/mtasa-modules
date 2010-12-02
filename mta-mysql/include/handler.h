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
#ifndef __HANDLER_H
#define __HANDLER_H

#include <map>
#include <mysql.h>
#include "result.h"

struct lua_State;

class MySQL
{
 /**
  ** Static interface methods
  **/
private:
  static unsigned int GetNextHandler ()
  {
    static unsigned int s_num = 0;
    return s_num++;
  }

public:
  /* Metatable */
  struct C_String_Compare {
    bool operator() ( const char* s1, const char* s2 ) const
    {
      return strcmp ( s1, s2 ) < 0;
    }
  };

  typedef std::map<const char*, lua_CFunction, C_String_Compare> mapType;
  static mapType ms_handlerMaps [ 256 ];
  static mapType ms_resultMaps [ 256 ];
  static bool ms_bHandlerMapInitialized;
  static bool ms_bResultMapInitialized;
  static int HandlerGC(lua_State* L);
  static int HandlerTostring(lua_State* L);
  static int HandlerIndex(lua_State* L);
  static int ResultGC(lua_State* L);
  static int ResultTostring(lua_State* L);
  static int ResultIndex(lua_State* L);
  static int NullEqual(lua_State* L);

  static void Startup(lua_State* L);

  static int Connect(lua_State* L);
  static int Close(lua_State* L);
  static int Errno(lua_State* L);
  static int Error(lua_State* L);
  static int Ping(lua_State* L);
  static int SelectDB(lua_State* L);
  static int EscapeString(lua_State* L);
  static int AffectedRows(lua_State* L);
  static int ChangeUser(lua_State* L);
  static int GetCharacterSetInfo(lua_State* L);
  static int GetClientInfo(lua_State* L);
  static int GetClientVersion(lua_State* L);
  static int GetHostInfo(lua_State* L);
  static int GetProtoInfo(lua_State* L);
  static int GetServerInfo(lua_State* L);
  static int GetServerVersion(lua_State* L);
  static int HexString(lua_State* L);
  static int Info(lua_State* L);
  static int InsertID(lua_State* L);
  static int Query(lua_State* L);
  static int UnbufferedQuery(lua_State* L);
  static int SetCharacterSet(lua_State* L);
  static int Stat(lua_State* L);
  static int WarningCount(lua_State* L);




public:
  ~MySQL();
  MySQL(const char* hostname,
        const char* username,
        const char* password,
        const char* database,
        unsigned int port,
        const char* unixSocket = 0,
        unsigned long clientFlags = 0);

  void Close();
  bool Ping();
  bool SelectDB(const char* db);
  size_t EscapeString(const char* str, char* dest, size_t len);
  unsigned long AffectedRows();
  bool ChangeUser(const char* username, const char* password, const char* database);
  void GetCharacterSetInfo(MY_CHARSET_INFO* cs);
  const char* GetHostInfo();
  unsigned long GetProtoInfo();
  const char* GetServerInfo();
  unsigned long GetServerVersion();
  const char* Info();
  unsigned long long InsertID();
  MySQL_Result* Query(const char* query, size_t len);
  MySQL_Result* UnbufferedQuery(const char* query, size_t len);
  bool SetCharacterSet(const char* csname);
  const char* Stat();
  unsigned long WarningCount();

  bool OK() { return m_ok; }
  unsigned int Errno();
  const char* Error();

  unsigned int GetNumHandler () const { return m_numHandler; }

private:
  MYSQL* m_handle;
  bool m_ok;
  unsigned int m_numHandler;
};

#endif /* #ifndef __HANDLER_H */
