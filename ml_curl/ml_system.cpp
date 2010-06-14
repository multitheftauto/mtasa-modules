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
		pModuleManager->RegisterFunction ( luaVM, "curl_send", CFunctions::curl_send );
		pModuleManager->RegisterFunction ( luaVM, "curl_strerror", CFunctions::curl_strerror );

		CFunctions::addEvent(luaVM, "onCurlDataRecieve");
		CFunctions::addEvent(luaVM, "onCurlConnect"); // not yet working
		CFunctions::addEvent(luaVM, "onCurlFileCreated"); // not yet working

		// Global variables for curl.
		CFunctions::registerCurlOption(luaVM, "CURLOPT_AUTOREFERER", (CURLoption *) CURLOPT_AUTOREFERER);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_COOKIESESSION", (CURLoption *) CURLOPT_COOKIESESSION);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_CRLF", (CURLoption *) CURLOPT_CRLF);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_DNS_USE_GLOBAL_CACHE", (CURLoption *) CURLOPT_DNS_USE_GLOBAL_CACHE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FAILONERROR", (CURLoption *) CURLOPT_FAILONERROR);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FILETIME", (CURLoption *) CURLOPT_FILETIME);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FOLLOWLOCATION", (CURLoption *) CURLOPT_FOLLOWLOCATION);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FORBID_REUSE", (CURLoption *) CURLOPT_FORBID_REUSE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FRESH_CONNECT", (CURLoption *) CURLOPT_FRESH_CONNECT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FTP_USE_EPRT", (CURLoption *) CURLOPT_FTP_USE_EPRT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FTP_USE_EPSV", (CURLoption *) CURLOPT_FTP_USE_EPSV);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FTPAPPEND", (CURLoption *) CURLOPT_FTPAPPEND);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FTPLISTONLY", (CURLoption *) CURLOPT_FTPLISTONLY);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_HEADER", (CURLoption *) CURLOPT_HEADER);
		//CFunctions::createCurlGlobal(luaVM, "CURLINFO_HEADER_OUT", (CURLINFO *) CURLINFO_HEADER_OUT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_HTTPGET", (CURLoption *) CURLOPT_HTTPGET);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_HTTPPROXYTUNNEL", (CURLoption *) CURLOPT_HTTPPROXYTUNNEL);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_NETRC", (CURLoption *) CURLOPT_NETRC);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_NOBODY", (CURLoption *) CURLOPT_NOBODY);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_NOPROGRESS", (CURLoption *) CURLOPT_NOPROGRESS);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_NOSIGNAL", (CURLoption *) CURLOPT_NOSIGNAL);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_POST", (CURLoption *) CURLOPT_POST);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_PUT", (CURLoption *) CURLOPT_PUT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_TRANSFERTEXT", (CURLoption *) CURLOPT_TRANSFERTEXT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_UNRESTRICTED_AUTH", (CURLoption *) CURLOPT_UNRESTRICTED_AUTH);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_UPLOAD", (CURLoption *) CURLOPT_UPLOAD);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_VERBOSE", (CURLoption *) CURLOPT_VERBOSE);

		// curl options with a integer value
		CFunctions::registerCurlOption(luaVM, "CURLOPT_BUFFERSIZE", (CURLoption *) CURLOPT_BUFFERSIZE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_CLOSEPOLICY", (CURLoption *) CURLOPT_CLOSEPOLICY);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_CONNECTTIMEOUT", (CURLoption *) CURLOPT_CONNECTTIMEOUT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_CONNECTTIMEOUT_MS", (CURLoption *) CURLOPT_CONNECTTIMEOUT_MS);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_DNS_CACHE_TIMEOUT", (CURLoption *) CURLOPT_DNS_CACHE_TIMEOUT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FTPSSLAUTH", (CURLoption *) CURLOPT_FTPSSLAUTH);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_HTTP_VERSION", (CURLoption *) CURLOPT_HTTP_VERSION);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_HTTPAUTH", (CURLoption *) CURLOPT_HTTPAUTH);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_INFILESIZE", (CURLoption *) CURLOPT_INFILESIZE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_LOW_SPEED_LIMIT", (CURLoption *) CURLOPT_LOW_SPEED_LIMIT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_LOW_SPEED_TIME", (CURLoption *) CURLOPT_LOW_SPEED_TIME);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_MAXCONNECTS", (CURLoption *) CURLOPT_MAXCONNECTS);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_MAXREDIRS", (CURLoption *) CURLOPT_MAXREDIRS);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_PORT", (CURLoption *) CURLOPT_PORT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_PROTOCOLS", (CURLoption *) CURLOPT_PROTOCOLS);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_PROXYPORT", (CURLoption *) CURLOPT_PROXYPORT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_PROXYTYPE", (CURLoption *) CURLOPT_PROXYTYPE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_REDIR_PROTOCOLS", (CURLoption *) CURLOPT_REDIR_PROTOCOLS);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_RESUME_FROM", (CURLoption *) CURLOPT_RESUME_FROM);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSL_VERIFYHOST", (CURLoption *) CURLOPT_SSL_VERIFYHOST);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSLVERSION", (CURLoption *) CURLOPT_SSLVERSION);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_TIMECONDITION", (CURLoption *) CURLOPT_TIMECONDITION);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_TIMEOUT", (CURLoption *) CURLOPT_TIMEOUT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_TIMEOUT_MS", (CURLoption *) CURLOPT_TIMEOUT_MS);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_TIMEVALUE", (CURLoption *) CURLOPT_TIMEVALUE);

		// curl options with a string value
		CFunctions::registerCurlOption(luaVM, "CURLOPT_CAINFO", (CURLoption *) CURLOPT_CAINFO);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_CAPATH", (CURLoption *) CURLOPT_CAPATH);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_COOKIE", (CURLoption *) CURLOPT_COOKIE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_COOKIEFILE", (CURLoption *) CURLOPT_COOKIEFILE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_COOKIEJAR", (CURLoption *) CURLOPT_COOKIEJAR);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_CUSTOMREQUEST", (CURLoption *) CURLOPT_CUSTOMREQUEST);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_EGDSOCKET", (CURLoption *) CURLOPT_EGDSOCKET);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_ENCODING", (CURLoption *) CURLOPT_ENCODING);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_FTPPORT", (CURLoption *) CURLOPT_FTPPORT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_INTERFACE", (CURLoption *) CURLOPT_INTERFACE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_KRB4LEVEL", (CURLoption *) CURLOPT_KRB4LEVEL);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_POSTFIELDS", (CURLoption *) CURLOPT_POSTFIELDS);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_PROXY", (CURLoption *) CURLOPT_PROXY);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_PROXYUSERPWD", (CURLoption *) CURLOPT_PROXYUSERPWD);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_RANDOM_FILE", (CURLoption *) CURLOPT_RANDOM_FILE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_RANGE", (CURLoption *) CURLOPT_RANGE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_REFERER", (CURLoption *) CURLOPT_REFERER);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSL_CIPHER_LIST", (CURLoption *) CURLOPT_SSL_CIPHER_LIST);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSLCERT", (CURLoption *) CURLOPT_SSLCERT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSLCERTPASSWD", (CURLoption *) CURLOPT_SSLCERTPASSWD);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSLCERTTYPE", (CURLoption *) CURLOPT_SSLCERTTYPE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSLENGINE", (CURLoption *) CURLOPT_SSLENGINE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSLENGINE_DEFAULT", (CURLoption *) CURLOPT_SSLENGINE_DEFAULT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSLKEY", (CURLoption *) CURLOPT_SSLKEY);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSLKEYPASSWD", (CURLoption *) CURLOPT_SSLKEYPASSWD);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_SSLKEYTYPE", (CURLoption *) CURLOPT_SSLKEYTYPE);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_URL", (CURLoption *) CURLOPT_URL);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_USERAGENT", (CURLoption *) CURLOPT_USERAGENT);
		CFunctions::registerCurlOption(luaVM, "CURLOPT_USERPWD", (CURLoption *) CURLOPT_USERPWD);

		// curl auth variables.
		CFunctions::registerCurlGlobal(luaVM, "CURLAUTH_BASIC", CURLAUTH_BASIC);
		CFunctions::registerCurlGlobal(luaVM, "CURLAUTH_DIGEST", CURLAUTH_DIGEST);
		CFunctions::registerCurlGlobal(luaVM, "CURLAUTH_GSSNEGOTIATE", CURLAUTH_GSSNEGOTIATE);
		CFunctions::registerCurlGlobal(luaVM, "CURLAUTH_NTLM", CURLAUTH_NTLM);
		CFunctions::registerCurlGlobal(luaVM, "CURLAUTH_ANY", CURLAUTH_ANY);
		CFunctions::registerCurlGlobal(luaVM, "CURLAUTH_ANYSAFE", CURLAUTH_ANYSAFE);

		// curl close policy variables
		/*CFunctions::registerCurlCode(luaVM, "CURLCLOSEPOLICY_LEAST_RECENTLY_USED", CURLCLOSEPOLICY_LEAST_RECENTLY_USED);
		CFunctions::registerCurlCode(luaVM, "CURLCLOSEPOLICY_LEAST_TRAFFIC", CURLCLOSEPOLICY_LEAST_TRAFFIC);
		CFunctions::registerCurlCode(luaVM, "CURLCLOSEPOLICY_SLOWEST", CURLCLOSEPOLICY_SLOWEST);
		CFunctions::registerCurlCode(luaVM, "CURLCLOSEPOLICY_CALLBACK", CURLCLOSEPOLICY_CALLBACK);
		CFunctions::registerCurlCode(luaVM, "CURLCLOSEPOLICY_OLDEST", CURLCLOSEPOLICY_OLDEST);*/

		// curl info variables
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_PRIVATE", (CURLINFO *) CURLINFO_PRIVATE);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_EFFECTIVE_URL", (CURLINFO *) CURLINFO_EFFECTIVE_URL);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_HTTP_CODE", (CURLINFO *) CURLINFO_HTTP_CODE);
		// CFunctions::registerCurlInfo(luaVM, "CURLINFO_HEADER_OUT", (CURLINFO *) CURLINFO_HEADER_OUT);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_HEADER_SIZE", (CURLINFO *) CURLINFO_HEADER_SIZE);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_REQUEST_SIZE", (CURLINFO *) CURLINFO_REQUEST_SIZE);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_TOTAL_TIME", (CURLINFO *) CURLINFO_TOTAL_TIME);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_NAMELOOKUP_TIME", (CURLINFO *) CURLINFO_NAMELOOKUP_TIME);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_CONNECT_TIME", (CURLINFO *) CURLINFO_CONNECT_TIME);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_PRETRANSFER_TIME", (CURLINFO *) CURLINFO_PRETRANSFER_TIME);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_SIZE_UPLOAD", (CURLINFO *) CURLINFO_SIZE_UPLOAD);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_SIZE_DOWNLOAD", (CURLINFO *) CURLINFO_SIZE_DOWNLOAD);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_SPEED_DOWNLOAD", (CURLINFO *) CURLINFO_SPEED_DOWNLOAD);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_SPEED_UPLOAD", (CURLINFO *) CURLINFO_SPEED_UPLOAD);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_FILETIME", (CURLINFO *) CURLINFO_FILETIME);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_SSL_VERIFYRESULT", (CURLINFO *) CURLINFO_SSL_VERIFYRESULT);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_CONTENT_LENGTH_DOWNLOAD", (CURLINFO *) CURLINFO_CONTENT_LENGTH_DOWNLOAD);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_CONTENT_LENGTH_UPLOAD", (CURLINFO *) CURLINFO_CONTENT_LENGTH_UPLOAD);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_STARTTRANSFER_TIME", (CURLINFO *) CURLINFO_STARTTRANSFER_TIME);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_CONTENT_TYPE", (CURLINFO *) CURLINFO_CONTENT_TYPE);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_REDIRECT_TIME", (CURLINFO *) CURLINFO_REDIRECT_TIME);
		CFunctions::registerCurlInfo(luaVM, "CURLINFO_REDIRECT_COUNT", (CURLINFO *) CURLINFO_REDIRECT_COUNT);

		// girl timecond variables -- leave later version
		/*CFunctions::registerCurlInfo(luaVM, "CURL_TIMECOND_IFMODSINCE", CURL_TIMECOND_IFMODSINCE);
		CFunctions::createCurlGlobal(luaVM, "CURL_TIMECOND_IFUNMODSINCE", CURL_TIMECOND_IFUNMODSINCE);
		CFunctions::createCurlGlobal(luaVM, "CURL_TIMECOND_LASTMOD", CURL_TIMECOND_LASTMOD);*/

		// curl version variables -- leave later version
		/*CFunctions::registerCurlCode(luaVM, "CURL_VERSION_IPV6", CURL_VERSION_IPV6);
		CFunctions::createCurlGlobal(luaVM, "CURL_VERSION_KERBEROS4", CURL_VERSION_KERBEROS4);
		CFunctions::createCurlGlobal(luaVM, "CURL_VERSION_SSL", CURL_VERSION_SSL);
		CFunctions::createCurlGlobal(luaVM, "CURL_VERSION_LIBZ", CURL_VERSION_LIBZ);
		CFunctions::createCurlGlobal(luaVM, "CURLVERSION_NOW", CURLVERSION_NOW);*/

		// curl E variables (easy package used here)
		CFunctions::registerCurlCode(luaVM, "CURLE_OK", (CURLcode *) CURLE_OK);
		CFunctions::registerCurlCode(luaVM, "CURLE_UNSUPPORTED_PROTOCOL", (CURLcode *) CURLE_UNSUPPORTED_PROTOCOL);
		CFunctions::registerCurlCode(luaVM, "CURLE_FAILED_INIT", (CURLcode *) CURLE_FAILED_INIT);
		CFunctions::registerCurlCode(luaVM, "CURLE_URL_MALFORMAT", (CURLcode *) CURLE_URL_MALFORMAT);
		CFunctions::registerCurlCode(luaVM, "CURLE_URL_MALFORMAT_USER", (CURLcode *) CURLE_URL_MALFORMAT_USER);
		CFunctions::registerCurlCode(luaVM, "CURLE_COULDNT_RESOLVE_PROXY", (CURLcode *) CURLE_COULDNT_RESOLVE_PROXY);
		CFunctions::registerCurlCode(luaVM, "CURLE_COULDNT_RESOLVE_HOST", (CURLcode *) CURLE_COULDNT_RESOLVE_HOST);
		CFunctions::registerCurlCode(luaVM, "CURLE_COULDNT_CONNECT", (CURLcode *) CURLE_COULDNT_CONNECT);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_WEIRD_SERVER_REPLY", (CURLcode *) CURLE_FTP_WEIRD_SERVER_REPLY);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_ACCESS_DENIED", (CURLcode *) CURLE_FTP_ACCESS_DENIED);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_USER_PASSWORD_INCORRECT", (CURLcode *) CURLE_FTP_USER_PASSWORD_INCORRECT);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_WEIRD_PASS_REPLY", (CURLcode *) CURLE_FTP_WEIRD_PASS_REPLY);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_WEIRD_USER_REPLY", (CURLcode *) CURLE_FTP_WEIRD_USER_REPLY);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_WEIRD_PASV_REPLY", (CURLcode *) CURLE_FTP_WEIRD_PASV_REPLY);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_WEIRD_227_FORMAT", (CURLcode *) CURLE_FTP_WEIRD_227_FORMAT);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_CANT_GET_HOST", (CURLcode *) CURLE_FTP_CANT_GET_HOST);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_CANT_RECONNECT", (CURLcode *) CURLE_FTP_CANT_RECONNECT);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_COULDNT_SET_BINARY", (CURLcode *) CURLE_FTP_COULDNT_SET_BINARY);
		CFunctions::registerCurlCode(luaVM, "CURLE_PARTIAL_FILE", (CURLcode *) CURLE_PARTIAL_FILE);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_COULDNT_RETR_FILE", (CURLcode *) CURLE_FTP_COULDNT_RETR_FILE);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_WRITE_ERROR", (CURLcode *) CURLE_FTP_WRITE_ERROR);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_QUOTE_ERROR", (CURLcode *) CURLE_FTP_QUOTE_ERROR);
		CFunctions::registerCurlCode(luaVM, "CURLE_HTTP_NOT_FOUND", (CURLcode *) CURLE_HTTP_NOT_FOUND);
		CFunctions::registerCurlCode(luaVM, "CURLE_WRITE_ERROR", (CURLcode *) CURLE_WRITE_ERROR);
		CFunctions::registerCurlCode(luaVM, "CURLE_MALFORMAT_USER", (CURLcode *) CURLE_MALFORMAT_USER);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_COULDNT_STOR_FILE", (CURLcode *) CURLE_FTP_COULDNT_STOR_FILE);
		CFunctions::registerCurlCode(luaVM, "CURLE_READ_ERROR", (CURLcode *) CURLE_READ_ERROR);
		CFunctions::registerCurlCode(luaVM, "CURLE_OUT_OF_MEMORY", (CURLcode *) CURLE_OUT_OF_MEMORY);
		CFunctions::registerCurlCode(luaVM, "CURLE_OPERATION_TIMEOUTED", (CURLcode *) CURLE_OPERATION_TIMEOUTED);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_COULDNT_SET_ASCII", (CURLcode *) CURLE_FTP_COULDNT_SET_ASCII);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_PORT_FAILED", (CURLcode *) CURLE_FTP_PORT_FAILED);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_COULDNT_USE_REST", (CURLcode *) CURLE_FTP_COULDNT_USE_REST);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_COULDNT_GET_SIZE", (CURLcode *) CURLE_FTP_COULDNT_GET_SIZE);
		CFunctions::registerCurlCode(luaVM, "CURLE_HTTP_RANGE_ERROR", (CURLcode *) CURLE_HTTP_RANGE_ERROR);
		CFunctions::registerCurlCode(luaVM, "CURLE_HTTP_POST_ERROR", (CURLcode *) CURLE_HTTP_POST_ERROR);
		CFunctions::registerCurlCode(luaVM, "CURLE_SSL_CONNECT_ERROR", (CURLcode *) CURLE_SSL_CONNECT_ERROR);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_BAD_DOWNLOAD_RESUME", (CURLcode *) CURLE_FTP_BAD_DOWNLOAD_RESUME);
		CFunctions::registerCurlCode(luaVM, "CURLE_FILE_COULDNT_READ_FILE", (CURLcode *) CURLE_FILE_COULDNT_READ_FILE);
		CFunctions::registerCurlCode(luaVM, "CURLE_LDAP_CANNOT_BIND", (CURLcode *) CURLE_LDAP_CANNOT_BIND);
		CFunctions::registerCurlCode(luaVM, "CURLE_LDAP_SEARCH_FAILED", (CURLcode *) CURLE_LDAP_SEARCH_FAILED);
		CFunctions::registerCurlCode(luaVM, "CURLE_LIBRARY_NOT_FOUND", (CURLcode *) CURLE_LIBRARY_NOT_FOUND);
		CFunctions::registerCurlCode(luaVM, "CURLE_FUNCTION_NOT_FOUND", (CURLcode *) CURLE_FUNCTION_NOT_FOUND);
		CFunctions::registerCurlCode(luaVM, "CURLE_ABORTED_BY_CALLBACK", (CURLcode *) CURLE_ABORTED_BY_CALLBACK);
		CFunctions::registerCurlCode(luaVM, "CURLE_BAD_FUNCTION_ARGUMENT", (CURLcode *) CURLE_BAD_FUNCTION_ARGUMENT);
		CFunctions::registerCurlCode(luaVM, "CURLE_BAD_CALLING_ORDER", (CURLcode *) CURLE_BAD_CALLING_ORDER);
		CFunctions::registerCurlCode(luaVM, "CURLE_HTTP_PORT_FAILED", (CURLcode *) CURLE_HTTP_PORT_FAILED);
		CFunctions::registerCurlCode(luaVM, "CURLE_BAD_PASSWORD_ENTERED", (CURLcode *) CURLE_BAD_PASSWORD_ENTERED);
		CFunctions::registerCurlCode(luaVM, "CURLE_TOO_MANY_REDIRECTS", (CURLcode *) CURLE_TOO_MANY_REDIRECTS);
		CFunctions::registerCurlCode(luaVM, "CURLE_UNKNOWN_TELNET_OPTION", (CURLcode *) CURLE_UNKNOWN_TELNET_OPTION);
		CFunctions::registerCurlCode(luaVM, "CURLE_TELNET_OPTION_SYNTAX", (CURLcode *) CURLE_TELNET_OPTION_SYNTAX);
		CFunctions::registerCurlCode(luaVM, "CURLE_OBSOLETE", (CURLcode *) CURLE_OBSOLETE);
		CFunctions::registerCurlCode(luaVM, "CURLE_SSL_PEER_CERTIFICATE", (CURLcode *) CURLE_SSL_PEER_CERTIFICATE);
		CFunctions::registerCurlCode(luaVM, "CURLE_GOT_NOTHING", (CURLcode *) CURLE_GOT_NOTHING);
		CFunctions::registerCurlCode(luaVM, "CURLE_SSL_ENGINE_NOTFOUND", (CURLcode *) CURLE_SSL_ENGINE_NOTFOUND);
		CFunctions::registerCurlCode(luaVM, "CURLE_SSL_ENGINE_SETFAILED", (CURLcode *) CURLE_SSL_ENGINE_SETFAILED);
		CFunctions::registerCurlCode(luaVM, "CURLE_SEND_ERROR", (CURLcode *) CURLE_SEND_ERROR);
		CFunctions::registerCurlCode(luaVM, "CURLE_RECV_ERROR", (CURLcode *) CURLE_RECV_ERROR);
		CFunctions::registerCurlCode(luaVM, "CURLE_SHARE_IN_USE", (CURLcode *) CURLE_SHARE_IN_USE);
		CFunctions::registerCurlCode(luaVM, "CURLE_SSL_CERTPROBLEM", (CURLcode *) CURLE_SSL_CERTPROBLEM);
		CFunctions::registerCurlCode(luaVM, "CURLE_SSL_CIPHER", (CURLcode *) CURLE_SSL_CIPHER);
		CFunctions::registerCurlCode(luaVM, "CURLE_SSL_CACERT", (CURLcode *) CURLE_SSL_CACERT);
		CFunctions::registerCurlCode(luaVM, "CURLE_BAD_CONTENT_ENCODING", (CURLcode *) CURLE_BAD_CONTENT_ENCODING);
		CFunctions::registerCurlCode(luaVM, "CURLE_LDAP_INVALID_URL", (CURLcode *) CURLE_LDAP_INVALID_URL);
		CFunctions::registerCurlCode(luaVM, "CURLE_FILESIZE_EXCEEDED", (CURLcode *) CURLE_FILESIZE_EXCEEDED);
		CFunctions::registerCurlCode(luaVM, "CURLE_FTP_SSL_FAILED", (CURLcode *) CURLE_FTP_SSL_FAILED);

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
		CFunctions::registerCurlGlobal(luaVM, "CURLMSG_DONE", CURLMSG_DONE);
    }
}


MTAEXPORT bool DoPulse ( void )
{
	// p_curl->doPulse();
    return true;
}

MTAEXPORT bool ShutdownModule ( void )
{
	delete mtacurls;
    return true;
}
