/*********************************************************
*
*  Multi Theft Auto: San Andreas
*
*  Curl module for mta.
*
*  Developers:
*  Alexander de Jong <alexander.t.dejong@gmail.com>
*
*********************************************************/

#include "ml_system.h"

ILuaModuleManager10 *pModuleManager = NULL;
Mtacurls* mtacurls = NULL;

// Initialisation function (module entrypoint)
MTAEXPORT bool InitModule ( ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion )
{
    pModuleManager = pManager;

    // Set the module info
    strncpy ( szModuleName, MODULE_NAME, MAX_INFO_LENGTH );
    strncpy ( szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH );
    (*fVersion) = MODULE_VERSION;

	mtacurls = new Mtacurls();

    return true;
}

MTAEXPORT void RegisterFunctions ( lua_State * luaVM )
{
    if ( pModuleManager && luaVM )
    {
		CFunctions::saveLuaData(luaVM);

		pModuleManager->RegisterFunction ( luaVM, "curl_init", CFunctions::curl_init );
		pModuleManager->RegisterFunction ( luaVM, "curl_close", CFunctions::curl_close );
		pModuleManager->RegisterFunction ( luaVM, "curl_setopt", CFunctions::curl_setopt );
		pModuleManager->RegisterFunction ( luaVM, "curl_sleep", CFunctions::curl_func_register );
		pModuleManager->RegisterFunction ( luaVM, "curl_cleanup", CFunctions::curl_cleanup );
		pModuleManager->RegisterFunction ( luaVM, "curl_escape", CFunctions::curl_escape );
		pModuleManager->RegisterFunction ( luaVM, "curl_perform", CFunctions::curl_perform );
		// pModuleManager->RegisterFunction ( luaVM, "curl_send", CFunctions::curl_send );
		pModuleManager->RegisterFunction ( luaVM, "curl_strerror", CFunctions::curl_strerror );

		// CFunctions::addEvent(luaVM, "onCurlDataRecieve");
		// CFunctions::addEvent(luaVM, "onCurlConnect"); // not yet working
		// CFunctions::addEvent(luaVM, "onCurlFileCreated"); // not yet working

		// Global variables for curl.
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_AUTOREFERER", (void*)CURLOPT_AUTOREFERER);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_COOKIESESSION", (void*)CURLOPT_COOKIESESSION);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_CRLF", (void*)CURLOPT_CRLF);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_DNS_USE_GLOBAL_CACHE", (void*)CURLOPT_DNS_USE_GLOBAL_CACHE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FAILONERROR", (void*)CURLOPT_FAILONERROR);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FILETIME", (void*)CURLOPT_FILETIME);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FOLLOWLOCATION", (void*)CURLOPT_FOLLOWLOCATION);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FORBID_REUSE", (void*)CURLOPT_FORBID_REUSE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FRESH_CONNECT", (void*)CURLOPT_FRESH_CONNECT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FTP_USE_EPRT", (void*)CURLOPT_FTP_USE_EPRT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FTP_USE_EPSV", (void*)CURLOPT_FTP_USE_EPSV);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FTPAPPEND", (void*)CURLOPT_FTPAPPEND);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FTPLISTONLY", (void*)CURLOPT_FTPLISTONLY);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_HEADER", (void*)CURLOPT_HEADER);
		//CFunctions::createCurlGlobal(luaVM, "CURLINFO_HEADER_OUT", (void*)CURLINFO_HEADER_OUT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_HTTPGET", (void*)CURLOPT_HTTPGET);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_HTTPPROXYTUNNEL", (void*)CURLOPT_HTTPPROXYTUNNEL);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_NETRC", (void*)CURLOPT_NETRC);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_NOBODY", (void*)CURLOPT_NOBODY);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_NOPROGRESS", (void*)CURLOPT_NOPROGRESS);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_NOSIGNAL", (void*)CURLOPT_NOSIGNAL);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_POST", (void*)CURLOPT_POST);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_PUT", (void*)CURLOPT_PUT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_TRANSFERTEXT", (void*)CURLOPT_TRANSFERTEXT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_UNRESTRICTED_AUTH", (void*)CURLOPT_UNRESTRICTED_AUTH);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_UPLOAD", (void*)CURLOPT_UPLOAD);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_VERBOSE", (void*)CURLOPT_VERBOSE);

		// curl options with a integer value
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_BUFFERSIZE", (void*)CURLOPT_BUFFERSIZE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_CLOSEPOLICY", (void*)CURLOPT_CLOSEPOLICY);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_CONNECTTIMEOUT", (void*)CURLOPT_CONNECTTIMEOUT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_CONNECTTIMEOUT_MS", (void*)CURLOPT_CONNECTTIMEOUT_MS);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_DNS_CACHE_TIMEOUT", (void*)CURLOPT_DNS_CACHE_TIMEOUT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FTPSSLAUTH", (void*)CURLOPT_FTPSSLAUTH);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_HTTP_VERSION", (void*)CURLOPT_HTTP_VERSION);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_HTTPAUTH", (void*)CURLOPT_HTTPAUTH);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_INFILESIZE", (void*)CURLOPT_INFILESIZE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_LOW_SPEED_LIMIT", (void*)CURLOPT_LOW_SPEED_LIMIT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_LOW_SPEED_TIME", (void*)CURLOPT_LOW_SPEED_TIME);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_MAXCONNECTS", (void*)CURLOPT_MAXCONNECTS);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_MAXREDIRS", (void*)CURLOPT_MAXREDIRS);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_PORT", (void*)CURLOPT_PORT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_PROTOCOLS", (void*)CURLOPT_PROTOCOLS);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_PROXYPORT", (void*)CURLOPT_PROXYPORT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_PROXYTYPE", (void*)CURLOPT_PROXYTYPE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_REDIR_PROTOCOLS", (void*)CURLOPT_REDIR_PROTOCOLS);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_RESUME_FROM", (void*)CURLOPT_RESUME_FROM);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSL_VERIFYHOST", (void*)CURLOPT_SSL_VERIFYHOST);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSLVERSION", (void*)CURLOPT_SSLVERSION);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_TIMECONDITION", (void*)CURLOPT_TIMECONDITION);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_TIMEOUT", (void*)CURLOPT_TIMEOUT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_TIMEOUT_MS", (void*)CURLOPT_TIMEOUT_MS);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_TIMEVALUE", (void*)CURLOPT_TIMEVALUE);

		// curl options with a string value
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_CAINFO", (void*)CURLOPT_CAINFO);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_CAPATH", (void*)CURLOPT_CAPATH);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_COOKIE", (void*)CURLOPT_COOKIE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_COOKIEFILE", (void*)CURLOPT_COOKIEFILE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_COOKIEJAR", (void*)CURLOPT_COOKIEJAR);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_CUSTOMREQUEST", (void*)CURLOPT_CUSTOMREQUEST);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_EGDSOCKET", (void*)CURLOPT_EGDSOCKET);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_ENCODING", (void*)CURLOPT_ENCODING);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_FTPPORT", (void*)CURLOPT_FTPPORT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_INTERFACE", (void*)CURLOPT_INTERFACE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_KRB4LEVEL", (void*)CURLOPT_KRB4LEVEL);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_POSTFIELDS", (void*)CURLOPT_POSTFIELDS);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_PROXY", (void*)CURLOPT_PROXY);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_PROXYUSERPWD", (void*)CURLOPT_PROXYUSERPWD);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_RANDOM_FILE", (void*)CURLOPT_RANDOM_FILE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_RANGE", (void*)CURLOPT_RANGE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_REFERER", (void*)CURLOPT_REFERER);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSL_CIPHER_LIST", (void*)CURLOPT_SSL_CIPHER_LIST);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSLCERT", (void*)CURLOPT_SSLCERT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSLCERTPASSWD", (void*)CURLOPT_SSLCERTPASSWD);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSLCERTTYPE", (void*)CURLOPT_SSLCERTTYPE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSLENGINE", (void*)CURLOPT_SSLENGINE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSLENGINE_DEFAULT", (void*)CURLOPT_SSLENGINE_DEFAULT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSLKEY", (void*)CURLOPT_SSLKEY);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSLKEYPASSWD", (void*)CURLOPT_SSLKEYPASSWD);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_SSLKEYTYPE", (void*)CURLOPT_SSLKEYTYPE);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_URL", (void*)CURLOPT_URL);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_USERAGENT", (void*)CURLOPT_USERAGENT);
		CFunctions::registerLuaGlobal(luaVM, "CURLOPT_USERPWD", (void*)CURLOPT_USERPWD);

		// curl auth variables.
		CFunctions::registerLuaGlobal(luaVM, "CURLAUTH_BASIC", (void*)CURLAUTH_BASIC);
		CFunctions::registerLuaGlobal(luaVM, "CURLAUTH_DIGEST", (void*)CURLAUTH_DIGEST);
		CFunctions::registerLuaGlobal(luaVM, "CURLAUTH_GSSNEGOTIATE", (void*)CURLAUTH_GSSNEGOTIATE);
		CFunctions::registerLuaGlobal(luaVM, "CURLAUTH_NTLM", (void*)CURLAUTH_NTLM);
		CFunctions::registerLuaGlobal(luaVM, "CURLAUTH_ANY", (void*)CURLAUTH_ANY);
		CFunctions::registerLuaGlobal(luaVM, "CURLAUTH_ANYSAFE", (void*)CURLAUTH_ANYSAFE);

		// curl close policy variables
		/*CFunctions::registerLuaGlobal(luaVM, "CURLCLOSEPOLICY_LEAST_RECENTLY_USED", CURLCLOSEPOLICY_LEAST_RECENTLY_USED);
		CFunctions::registerLuaGlobal(luaVM, "CURLCLOSEPOLICY_LEAST_TRAFFIC", CURLCLOSEPOLICY_LEAST_TRAFFIC);
		CFunctions::registerLuaGlobal(luaVM, "CURLCLOSEPOLICY_SLOWEST", CURLCLOSEPOLICY_SLOWEST);
		CFunctions::registerLuaGlobal(luaVM, "CURLCLOSEPOLICY_CALLBACK", CURLCLOSEPOLICY_CALLBACK);
		CFunctions::registerLuaGlobal(luaVM, "CURLCLOSEPOLICY_OLDEST", CURLCLOSEPOLICY_OLDEST);*/

		// curl info variables
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_PRIVATE", (void*)CURLINFO_PRIVATE);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_EFFECTIVE_URL", (void*)CURLINFO_EFFECTIVE_URL);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_HTTP_CODE", (void*)CURLINFO_HTTP_CODE);
		// CFunctions::registerLuaGlobal(luaVM, "CURLINFO_HEADER_OUT", (void*)CURLINFO_HEADER_OUT);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_HEADER_SIZE", (void*)CURLINFO_HEADER_SIZE);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_REQUEST_SIZE", (void*)CURLINFO_REQUEST_SIZE);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_TOTAL_TIME", (void*)CURLINFO_TOTAL_TIME);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_NAMELOOKUP_TIME", (void*)CURLINFO_NAMELOOKUP_TIME);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_CONNECT_TIME", (void*)CURLINFO_CONNECT_TIME);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_PRETRANSFER_TIME", (void*)CURLINFO_PRETRANSFER_TIME);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_SIZE_UPLOAD", (void*)CURLINFO_SIZE_UPLOAD);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_SIZE_DOWNLOAD", (void*)CURLINFO_SIZE_DOWNLOAD);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_SPEED_DOWNLOAD", (void*)CURLINFO_SPEED_DOWNLOAD);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_SPEED_UPLOAD", (void*)CURLINFO_SPEED_UPLOAD);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_FILETIME", (void*)CURLINFO_FILETIME);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_SSL_VERIFYRESULT", (void*)CURLINFO_SSL_VERIFYRESULT);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_CONTENT_LENGTH_DOWNLOAD", (void*)CURLINFO_CONTENT_LENGTH_DOWNLOAD);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_CONTENT_LENGTH_UPLOAD", (void*)CURLINFO_CONTENT_LENGTH_UPLOAD);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_STARTTRANSFER_TIME", (void*)CURLINFO_STARTTRANSFER_TIME);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_CONTENT_TYPE", (void*)CURLINFO_CONTENT_TYPE);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_REDIRECT_TIME", (void*)CURLINFO_REDIRECT_TIME);
		CFunctions::registerLuaGlobal(luaVM, "CURLINFO_REDIRECT_COUNT", (void*)CURLINFO_REDIRECT_COUNT);

		// curl timecond variables -- leave later version
		/*CFunctions::registerLuaGlobal(luaVM, "CURL_TIMECOND_IFMODSINCE", CURL_TIMECOND_IFMODSINCE);
		CFunctions::createCurlGlobal(luaVM, "CURL_TIMECOND_IFUNMODSINCE", CURL_TIMECOND_IFUNMODSINCE);
		CFunctions::createCurlGlobal(luaVM, "CURL_TIMECOND_LASTMOD", CURL_TIMECOND_LASTMOD);*/

		// curl version variables -- leave later version
		/*CFunctions::registerLuaGlobal(luaVM, "CURL_VERSION_IPV6", CURL_VERSION_IPV6);
		CFunctions::createCurlGlobal(luaVM, "CURL_VERSION_KERBEROS4", CURL_VERSION_KERBEROS4);
		CFunctions::createCurlGlobal(luaVM, "CURL_VERSION_SSL", CURL_VERSION_SSL);
		CFunctions::createCurlGlobal(luaVM, "CURL_VERSION_LIBZ", CURL_VERSION_LIBZ);
		CFunctions::createCurlGlobal(luaVM, "CURLVERSION_NOW", CURLVERSION_NOW);*/

		// curl E variables (easy package used here)
		CFunctions::registerLuaGlobal(luaVM, "CURLE_OK", (void*)CURLE_OK);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_UNSUPPORTED_PROTOCOL", (void*)CURLE_UNSUPPORTED_PROTOCOL);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FAILED_INIT", (void*)CURLE_FAILED_INIT);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_URL_MALFORMAT", (void*)CURLE_URL_MALFORMAT);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_URL_MALFORMAT_USER", (void*)CURLE_URL_MALFORMAT_USER);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_COULDNT_RESOLVE_PROXY", (void*)CURLE_COULDNT_RESOLVE_PROXY);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_COULDNT_RESOLVE_HOST", (void*)CURLE_COULDNT_RESOLVE_HOST);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_COULDNT_CONNECT", (void*)CURLE_COULDNT_CONNECT);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_WEIRD_SERVER_REPLY", (void*)CURLE_FTP_WEIRD_SERVER_REPLY);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_ACCESS_DENIED", (void*)CURLE_FTP_ACCESS_DENIED);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_USER_PASSWORD_INCORRECT", (void*)CURLE_FTP_USER_PASSWORD_INCORRECT);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_WEIRD_PASS_REPLY", (void*)CURLE_FTP_WEIRD_PASS_REPLY);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_WEIRD_USER_REPLY", (void*)CURLE_FTP_WEIRD_USER_REPLY);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_WEIRD_PASV_REPLY", (void*)CURLE_FTP_WEIRD_PASV_REPLY);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_WEIRD_227_FORMAT", (void*)CURLE_FTP_WEIRD_227_FORMAT);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_CANT_GET_HOST", (void*)CURLE_FTP_CANT_GET_HOST);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_CANT_RECONNECT", (void*)CURLE_FTP_CANT_RECONNECT);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_COULDNT_SET_BINARY", (void*)CURLE_FTP_COULDNT_SET_BINARY);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_PARTIAL_FILE", (void*)CURLE_PARTIAL_FILE);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_COULDNT_RETR_FILE", (void*)CURLE_FTP_COULDNT_RETR_FILE);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_WRITE_ERROR", (void*)CURLE_FTP_WRITE_ERROR);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_QUOTE_ERROR", (void*)CURLE_FTP_QUOTE_ERROR);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_HTTP_NOT_FOUND", (void*)CURLE_HTTP_NOT_FOUND);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_WRITE_ERROR", (void*)CURLE_WRITE_ERROR);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_MALFORMAT_USER", (void*)CURLE_MALFORMAT_USER);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_COULDNT_STOR_FILE", (void*)CURLE_FTP_COULDNT_STOR_FILE);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_READ_ERROR", (void*)CURLE_READ_ERROR);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_OUT_OF_MEMORY", (void*)CURLE_OUT_OF_MEMORY);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_OPERATION_TIMEOUTED", (void*)CURLE_OPERATION_TIMEOUTED);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_COULDNT_SET_ASCII", (void*)CURLE_FTP_COULDNT_SET_ASCII);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_PORT_FAILED", (void*)CURLE_FTP_PORT_FAILED);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_COULDNT_USE_REST", (void*)CURLE_FTP_COULDNT_USE_REST);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_COULDNT_GET_SIZE", (void*)CURLE_FTP_COULDNT_GET_SIZE);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_HTTP_RANGE_ERROR", (void*)CURLE_HTTP_RANGE_ERROR);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_HTTP_POST_ERROR", (void*)CURLE_HTTP_POST_ERROR);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_SSL_CONNECT_ERROR", (void*)CURLE_SSL_CONNECT_ERROR);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_BAD_DOWNLOAD_RESUME", (void*)CURLE_FTP_BAD_DOWNLOAD_RESUME);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FILE_COULDNT_READ_FILE", (void*)CURLE_FILE_COULDNT_READ_FILE);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_LDAP_CANNOT_BIND", (void*)CURLE_LDAP_CANNOT_BIND);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_LDAP_SEARCH_FAILED", (void*)CURLE_LDAP_SEARCH_FAILED);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_LIBRARY_NOT_FOUND", (void*)CURLE_LIBRARY_NOT_FOUND);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FUNCTION_NOT_FOUND", (void*)CURLE_FUNCTION_NOT_FOUND);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_ABORTED_BY_CALLBACK", (void*)CURLE_ABORTED_BY_CALLBACK);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_BAD_FUNCTION_ARGUMENT", (void*)CURLE_BAD_FUNCTION_ARGUMENT);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_BAD_CALLING_ORDER", (void*)CURLE_BAD_CALLING_ORDER);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_HTTP_PORT_FAILED", (void*)CURLE_HTTP_PORT_FAILED);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_BAD_PASSWORD_ENTERED", (void*)CURLE_BAD_PASSWORD_ENTERED);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_TOO_MANY_REDIRECTS", (void*)CURLE_TOO_MANY_REDIRECTS);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_UNKNOWN_TELNET_OPTION", (void*)CURLE_UNKNOWN_TELNET_OPTION);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_TELNET_OPTION_SYNTAX", (void*)CURLE_TELNET_OPTION_SYNTAX);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_OBSOLETE", (void*)CURLE_OBSOLETE);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_SSL_PEER_CERTIFICATE", (void*)CURLE_SSL_PEER_CERTIFICATE);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_GOT_NOTHING", (void*)CURLE_GOT_NOTHING);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_SSL_ENGINE_NOTFOUND", (void*)CURLE_SSL_ENGINE_NOTFOUND);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_SSL_ENGINE_SETFAILED", (void*)CURLE_SSL_ENGINE_SETFAILED);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_SEND_ERROR", (void*)CURLE_SEND_ERROR);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_RECV_ERROR", (void*)CURLE_RECV_ERROR);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_SHARE_IN_USE", (void*)CURLE_SHARE_IN_USE);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_SSL_CERTPROBLEM", (void*)CURLE_SSL_CERTPROBLEM);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_SSL_CIPHER", (void*)CURLE_SSL_CIPHER);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_SSL_CACERT", (void*)CURLE_SSL_CACERT);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_BAD_CONTENT_ENCODING", (void*)CURLE_BAD_CONTENT_ENCODING);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_LDAP_INVALID_URL", (void*)CURLE_LDAP_INVALID_URL);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FILESIZE_EXCEEDED", (void*)CURLE_FILESIZE_EXCEEDED);
		CFunctions::registerLuaGlobal(luaVM, "CURLE_FTP_SSL_FAILED", (void*)CURLE_FTP_SSL_FAILED);

		// Curl proxy variables
		/*CFunctions::createCurlGlobal(luaVM, "CURLPROXY_HTTP", CURLPROXY_HTTP);
		CFunctions::createCurlGlobal(luaVM, "CURLPROXY_SOCKS5", CURLPROXY_SOCKS5);

		// Curl netrc variables
		CFunctions::createCurlGlobal(luaVM, "CURL_NETRC_OPTIONAL", CURL_NETRC_OPTIONAL);
		CFunctions::createCurlGlobal(luaVM, "CURL_NETRC_IGNORED", CURL_NETRC_IGNORED);
		CFunctions::createCurlGlobal(luaVM, "CURL_NETRC_REQUIRED", CURL_NETRC_REQUIRED);

		// Curl http variables
		CFunctions::createCurlGlobal(luaVM, "CURL_HTTP_VERSION_NONE", CURL_HTTP_VERSION_NONE);
		CFunctions::createCurlGlobal(luaVM, "CURL_HTTP_VERSION_1_0", CURL_HTTP_VERSION_1_0);
		CFunctions::createCurlGlobal(luaVM, "CURL_HTTP_VERSION_1_1", CURL_HTTP_VERSION_1_1);

		// Curl M variables (multi, not implemented yet. Dont publish these variables. It wont work.)
		CFunctions::createCurlGlobal(luaVM, "CURLM_CALL_MULTI_PERFORM", CURLM_CALL_MULTI_PERFORM);
		CFunctions::createCurlGlobal(luaVM, "CURLM_OK", CURLM_OK);
		CFunctions::createCurlGlobal(luaVM, "CURLM_BAD_HANDLE", CURLM_BAD_HANDLE);
		CFunctions::createCurlGlobal(luaVM, "CURLM_BAD_EASY_HANDLE", CURLM_BAD_EASY_HANDLE);
		CFunctions::createCurlGlobal(luaVM, "CURLM_OUT_OF_MEMORY", CURLM_OUT_OF_MEMORY);
		CFunctions::createCurlGlobal(luaVM, "CURLM_INTERNAL_ERROR", CURLM_INTERNAL_ERROR);*/

		// Curl message variable
		CFunctions::registerLuaGlobal(luaVM, "CURLMSG_DONE", (void*)CURLMSG_DONE);
    }
}


MTAEXPORT bool DoPulse ( void )
{
	mtacurls->DoPulse();
    return true;
}

MTAEXPORT bool ShutdownModule ( void )
{
	delete mtacurls;
    return true;
}
