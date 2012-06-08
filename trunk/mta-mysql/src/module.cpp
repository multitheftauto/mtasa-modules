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
#define snprintf _snprintf
#endif
#include "luaimports.h"
#include "module.h"
#include "handler.h"
#include "result.h"

ILuaModuleManager* pModuleManager = 0;


MTAEXPORT bool InitModule (ILuaModuleManager *pManager, char *szModuleName, char *szAuthor, float *fVersion)
{
    pModuleManager = pManager;

    // set the module info
    strncpy ( szModuleName, MODULE_NAME, MAX_INFO_LENGTH );
    strncpy ( szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH );
    (*fVersion) = MODULE_VERSION;

    ImportLua();

    return true;
}

MTAEXPORT void RegisterFunctions(lua_State * L)
{
    MySQL::Startup(L);

    /* Register MySQL handler functions */
    pModuleManager->RegisterFunction(L, "mysql_connect",                MySQL::Connect);
    pModuleManager->RegisterFunction(L, "mysql_close",                  MySQL::Close);
    pModuleManager->RegisterFunction(L, "mysql_errno",                  MySQL::Errno);
    pModuleManager->RegisterFunction(L, "mysql_error",                  MySQL::Error);
    pModuleManager->RegisterFunction(L, "mysql_ping",                   MySQL::Ping);
    pModuleManager->RegisterFunction(L, "mysql_select_db",              MySQL::SelectDB);
    pModuleManager->RegisterFunction(L, "mysql_escape_string",          MySQL::EscapeString);
    pModuleManager->RegisterFunction(L, "mysql_affected_rows",          MySQL::AffectedRows);
    pModuleManager->RegisterFunction(L, "mysql_change_user",            MySQL::ChangeUser);
    pModuleManager->RegisterFunction(L, "mysql_get_character_set_info", MySQL::GetCharacterSetInfo);
    pModuleManager->RegisterFunction(L, "mysql_get_client_info",        MySQL::GetClientInfo);
    pModuleManager->RegisterFunction(L, "mysql_get_client_version",     MySQL::GetClientVersion);
    pModuleManager->RegisterFunction(L, "mysql_get_host_info",          MySQL::GetHostInfo);
    pModuleManager->RegisterFunction(L, "mysql_get_proto_info",         MySQL::GetProtoInfo);
    pModuleManager->RegisterFunction(L, "mysql_get_server_info",        MySQL::GetServerInfo);
    pModuleManager->RegisterFunction(L, "mysql_get_server_version",     MySQL::GetServerVersion);
    pModuleManager->RegisterFunction(L, "mysql_hex_string",             MySQL::HexString);
    pModuleManager->RegisterFunction(L, "mysql_info",                   MySQL::Info);
    pModuleManager->RegisterFunction(L, "mysql_insert_id",              MySQL::InsertID);
    pModuleManager->RegisterFunction(L, "mysql_query",                  MySQL::Query);
    pModuleManager->RegisterFunction(L, "mysql_unbuffered_query",       MySQL::UnbufferedQuery);
    pModuleManager->RegisterFunction(L, "mysql_set_character_set",      MySQL::SetCharacterSet);
    pModuleManager->RegisterFunction(L, "mysql_stat",                   MySQL::Stat);
    pModuleManager->RegisterFunction(L, "mysql_warning_count",          MySQL::WarningCount);


    /* Register MySQL result functions */
    pModuleManager->RegisterFunction(L, "mysql_data_seek",              MySQL_Result::DataSeek);
    pModuleManager->RegisterFunction(L, "mysql_fetch_field",            MySQL_Result::FetchField);
    pModuleManager->RegisterFunction(L, "mysql_fields",                 MySQL_Result::Fields);
    pModuleManager->RegisterFunction(L, "mysql_fetch_lengths",          MySQL_Result::FetchLengths);
    pModuleManager->RegisterFunction(L, "mysql_fetch_row",              MySQL_Result::FetchRow);
    pModuleManager->RegisterFunction(L, "mysql_rows",                   MySQL_Result::Rows);
    pModuleManager->RegisterFunction(L, "mysql_fetch_assoc",            MySQL_Result::FetchAssoc);
    pModuleManager->RegisterFunction(L, "mysql_rows_assoc",             MySQL_Result::RowsAssoc);
    pModuleManager->RegisterFunction(L, "mysql_field_length",           MySQL_Result::FieldLength);
    pModuleManager->RegisterFunction(L, "mysql_field_name",             MySQL_Result::FieldName);
    pModuleManager->RegisterFunction(L, "mysql_field_seek",             MySQL_Result::FieldSeek);
    pModuleManager->RegisterFunction(L, "mysql_field_tell",             MySQL_Result::FieldTell);
    pModuleManager->RegisterFunction(L, "mysql_num_fields",             MySQL_Result::NumFields);
    pModuleManager->RegisterFunction(L, "mysql_num_rows",               MySQL_Result::NumRows);
    pModuleManager->RegisterFunction(L, "mysql_numfields",              MySQL_Result::NumFields);
    pModuleManager->RegisterFunction(L, "mysql_numrows",                MySQL_Result::NumRows);
    pModuleManager->RegisterFunction(L, "mysql_result",                 MySQL_Result::Result);
    pModuleManager->RegisterFunction(L, "mysql_free_result",            MySQL_Result::FreeResult);
    pModuleManager->RegisterFunction(L, "mysql_null",                   MySQL_Result::Null);
}

MTAEXPORT bool DoPulse(void)
{
    return true;
}

MTAEXPORT bool ShutdownModule(void)
{
    return true;
}

MTAEXPORT bool ResourceStopping ( lua_State * luaVM )
{
    return true;
}

MTAEXPORT bool ResourceStopped ( lua_State * luaVM )
{
    return true;
}
