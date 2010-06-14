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

struct curl_opt_boolean
{
	const char* name;
	CURLoption option;
};

struct curl_opt_string
{
	const char* name;
	CURLoption option;
};

struct curl_opt_number
{
	const char* name;
	CURLoption option;
};

MTAEXPORT void RegisterFunctions ( lua_State * luaVM )
{
    if ( pModuleManager && luaVM )
    {
		CFunctions::saveLuaData(luaVM);

		pModuleManager->RegisterFunction ( luaVM, "curl_init", CFunctions::curl_init );
		pModuleManager->RegisterFunction ( luaVM, "curl_close", CFunctions::curl_close );
		pModuleManager->RegisterFunction ( luaVM, "curl_setopt", CFunctions::curl_setopt );

		CFunctions::addEvent(luaVM, "onCurlDataRecieve");
		CFunctions::addEvent(luaVM, "onCurlConnect"); // not yet working
		CFunctions::addEvent(luaVM, "onCurlFileCreated"); // not yet working

		// Global variables for curl.
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_AUTOREFERER", CURLOPT_AUTOREFERER);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_COOKIESESSION", CURLOPT_COOKIESESSION);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_CRLF", CURLOPT_CRLF);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_DNS_USE_GLOBAL_CACHE", CURLOPT_DNS_USE_GLOBAL_CACHE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FAILONERROR", CURLOPT_FAILONERROR);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FILETIME", CURLOPT_FILETIME);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FOLLOWLOCATION", CURLOPT_FOLLOWLOCATION);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FORBID_REUSE", CURLOPT_FORBID_REUSE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FRESH_CONNECT", CURLOPT_FRESH_CONNECT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FTP_USE_EPRT", CURLOPT_FTP_USE_EPRT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FTP_USE_EPSV", CURLOPT_FTP_USE_EPSV);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FTPAPPEND", CURLOPT_FTPAPPEND);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FTPLISTONLY", CURLOPT_FTPLISTONLY);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_HEADER", CURLOPT_HEADER);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_HEADER_OUT", CURLINFO_HEADER_OUT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_HTTPGET", CURLOPT_HTTPGET);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_HTTPPROXYTUNNEL", CURLOPT_HTTPPROXYTUNNEL);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_NETRC", CURLOPT_NETRC);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_NOBODY", CURLOPT_NOBODY);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_NOPROGRESS", CURLOPT_NOPROGRESS);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_NOSIGNAL", CURLOPT_NOSIGNAL);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_POST", CURLOPT_POST);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_PUT", CURLOPT_PUT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_TRANSFERTEXT", CURLOPT_TRANSFERTEXT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_UNRESTRICTED_AUTH", CURLOPT_UNRESTRICTED_AUTH);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_UPLOAD", CURLOPT_UPLOAD);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_VERBOSE", CURLOPT_VERBOSE);

		// curl options with a integer value
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_BUFFERSIZE", CURLOPT_BUFFERSIZE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_CLOSEPOLICY", CURLOPT_CLOSEPOLICY);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_CONNECTTIMEOUT", CURLOPT_CONNECTTIMEOUT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_CONNECTTIMEOUT_MS", CURLOPT_CONNECTTIMEOUT_MS);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_DNS_CACHE_TIMEOUT", CURLOPT_DNS_CACHE_TIMEOUT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FTPSSLAUTH", CURLOPT_FTPSSLAUTH);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_HTTP_VERSION", CURLOPT_HTTP_VERSION);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_HTTPAUTH", CURLOPT_HTTPAUTH);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_INFILESIZE", CURLOPT_INFILESIZE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_LOW_SPEED_LIMIT", CURLOPT_LOW_SPEED_LIMIT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_LOW_SPEED_TIME", CURLOPT_LOW_SPEED_TIME);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_MAXCONNECTS", CURLOPT_MAXCONNECTS);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_MAXREDIRS", CURLOPT_MAXREDIRS);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_PORT", CURLOPT_PORT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_PROTOCOLS", CURLOPT_PROTOCOLS);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_PROXYPORT", CURLOPT_PROXYPORT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_PROXYTYPE", CURLOPT_PROXYTYPE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_REDIR_PROTOCOLS", CURLOPT_REDIR_PROTOCOLS);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_RESUME_FROM", CURLOPT_RESUME_FROM);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSL_VERIFYHOST", CURLOPT_SSL_VERIFYHOST);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSLVERSION", CURLOPT_SSLVERSION);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_TIMECONDITION", CURLOPT_TIMECONDITION);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_TIMEOUT", CURLOPT_TIMEOUT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_TIMEOUT_MS", CURLOPT_TIMEOUT_MS);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_TIMEVALUE", CURLOPT_TIMEVALUE);

		// curl options with a string value
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_CAINFO", CURLOPT_CAINFO);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_CAPATH", CURLOPT_CAPATH);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_COOKIE", CURLOPT_COOKIE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_COOKIEFILE", CURLOPT_COOKIEFILE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_COOKIEJAR", CURLOPT_COOKIEJAR);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_CUSTOMREQUEST", CURLOPT_CUSTOMREQUEST);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_EGDSOCKET", CURLOPT_EGDSOCKET);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_ENCODING", CURLOPT_ENCODING);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_FTPPORT", CURLOPT_FTPPORT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_INTERFACE", CURLOPT_INTERFACE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_KRB4LEVEL", CURLOPT_KRB4LEVEL);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_POSTFIELDS", CURLOPT_POSTFIELDS);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_PROXY", CURLOPT_PROXY);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_PROXYUSERPWD", CURLOPT_PROXYUSERPWD);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_RANDOM_FILE", CURLOPT_RANDOM_FILE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_RANGE", CURLOPT_RANGE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_REFERER", CURLOPT_REFERER);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSL_CIPHER_LIST", CURLOPT_SSL_CIPHER_LIST);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSLCERT", CURLOPT_SSLCERT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSLCERTPASSWD", CURLOPT_SSLCERTPASSWD);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSLCERTTYPE", CURLOPT_SSLCERTTYPE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSLENGINE", CURLOPT_SSLENGINE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSLENGINE_DEFAULT", CURLOPT_SSLENGINE_DEFAULT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSLKEY", CURLOPT_SSLKEY);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSLKEYPASSWD", CURLOPT_SSLKEYPASSWD);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_SSLKEYTYPE", CURLOPT_SSLKEYTYPE);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_URL", CURLOPT_URL);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_USERAGENT", CURLOPT_USERAGENT);
		CFunctions::createCurlGlobal(luaVM, "CURLOPT_USERPWD", CURLOPT_USERPWD);

		// curl auth variables.
		CFunctions::createCurlGlobal(luaVM, "CURLAUTH_BASIC", CURLAUTH_BASIC);
		CFunctions::createCurlGlobal(luaVM, "CURLAUTH_DIGEST", CURLAUTH_DIGEST);
		CFunctions::createCurlGlobal(luaVM, "CURLAUTH_GSSNEGOTIATE", CURLAUTH_GSSNEGOTIATE);
		CFunctions::createCurlGlobal(luaVM, "CURLAUTH_NTLM", CURLAUTH_NTLM);
		CFunctions::createCurlGlobal(luaVM, "CURLAUTH_ANY", CURLAUTH_ANY);
		CFunctions::createCurlGlobal(luaVM, "CURLAUTH_ANYSAFE", CURLAUTH_ANYSAFE);

		// curl close policy variables
		CFunctions::createCurlGlobal(luaVM, "CURLCLOSEPOLICY_LEAST_RECENTLY_USED", CURLCLOSEPOLICY_LEAST_RECENTLY_USED);
		CFunctions::createCurlGlobal(luaVM, "CURLCLOSEPOLICY_LEAST_TRAFFIC", CURLCLOSEPOLICY_LEAST_TRAFFIC);
		CFunctions::createCurlGlobal(luaVM, "CURLCLOSEPOLICY_SLOWEST", CURLCLOSEPOLICY_SLOWEST);
		CFunctions::createCurlGlobal(luaVM, "CURLCLOSEPOLICY_CALLBACK", CURLCLOSEPOLICY_CALLBACK);
		CFunctions::createCurlGlobal(luaVM, "CURLCLOSEPOLICY_OLDEST", CURLCLOSEPOLICY_OLDEST);

		// curl info variables
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_PRIVATE", CURLINFO_PRIVATE);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_EFFECTIVE_URL", CURLINFO_EFFECTIVE_URL);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_HTTP_CODE", CURLINFO_HTTP_CODE);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_HEADER_OUT", CURLINFO_HEADER_OUT);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_HEADER_SIZE", CURLINFO_HEADER_SIZE);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_REQUEST_SIZE", CURLINFO_REQUEST_SIZE);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_TOTAL_TIME", CURLINFO_TOTAL_TIME);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_NAMELOOKUP_TIME", CURLINFO_NAMELOOKUP_TIME);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_CONNECT_TIME", CURLINFO_CONNECT_TIME);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_PRETRANSFER_TIME", CURLINFO_PRETRANSFER_TIME);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_SIZE_UPLOAD", CURLINFO_SIZE_UPLOAD);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_SIZE_DOWNLOAD", CURLINFO_SIZE_DOWNLOAD);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_SPEED_DOWNLOAD", CURLINFO_SPEED_DOWNLOAD);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_SPEED_UPLOAD", CURLINFO_SPEED_UPLOAD);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_FILETIME", CURLINFO_FILETIME);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_SSL_VERIFYRESULT", CURLINFO_SSL_VERIFYRESULT);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_CONTENT_LENGTH_DOWNLOAD", CURLINFO_CONTENT_LENGTH_DOWNLOAD);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_CONTENT_LENGTH_UPLOAD", CURLINFO_CONTENT_LENGTH_UPLOAD);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_STARTTRANSFER_TIME", CURLINFO_STARTTRANSFER_TIME);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_CONTENT_TYPE", CURLINFO_CONTENT_TYPE);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_REDIRECT_TIME", CURLINFO_REDIRECT_TIME);
		CFunctions::createCurlGlobal(luaVM, "CURLINFO_REDIRECT_COUNT", CURLINFO_REDIRECT_COUNT);

		// girl timecond variables
		CFunctions::createCurlGlobal(luaVM, "CURL_TIMECOND_IFMODSINCE", CURL_TIMECOND_IFMODSINCE);
		CFunctions::createCurlGlobal(luaVM, "CURL_TIMECOND_IFUNMODSINCE", CURL_TIMECOND_IFUNMODSINCE);
		CFunctions::createCurlGlobal(luaVM, "CURL_TIMECOND_LASTMOD", CURL_TIMECOND_LASTMOD);

		// curl version variables
		CFunctions::createCurlGlobal(luaVM, "CURL_VERSION_IPV6", CURL_VERSION_IPV6);
		CFunctions::createCurlGlobal(luaVM, "CURL_VERSION_KERBEROS4", CURL_VERSION_KERBEROS4);
		CFunctions::createCurlGlobal(luaVM, "CURL_VERSION_SSL", CURL_VERSION_SSL);
		CFunctions::createCurlGlobal(luaVM, "CURL_VERSION_LIBZ", CURL_VERSION_LIBZ);
		CFunctions::createCurlGlobal(luaVM, "CURLVERSION_NOW", CURLVERSION_NOW);

		// curl E variables (easy package used here)
		CFunctions::createCurlGlobal(luaVM, "CURLE_OK", CURLE_OK);
		CFunctions::createCurlGlobal(luaVM, "CURLE_UNSUPPORTED_PROTOCOL", CURLE_UNSUPPORTED_PROTOCOL);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FAILED_INIT", CURLE_FAILED_INIT);
		CFunctions::createCurlGlobal(luaVM, "CURLE_URL_MALFORMAT", CURLE_URL_MALFORMAT);
		CFunctions::createCurlGlobal(luaVM, "CURLE_URL_MALFORMAT_USER", CURLE_URL_MALFORMAT_USER);
		CFunctions::createCurlGlobal(luaVM, "CURLE_COULDNT_RESOLVE_PROXY", CURLE_COULDNT_RESOLVE_PROXY);
		CFunctions::createCurlGlobal(luaVM, "CURLE_COULDNT_RESOLVE_HOST", CURLE_COULDNT_RESOLVE_HOST);
		CFunctions::createCurlGlobal(luaVM, "CURLE_COULDNT_CONNECT", CURLE_COULDNT_CONNECT);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_WEIRD_SERVER_REPLY", CURLE_FTP_WEIRD_SERVER_REPLY);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_ACCESS_DENIED", CURLE_FTP_ACCESS_DENIED);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_USER_PASSWORD_INCORRECT", CURLE_FTP_USER_PASSWORD_INCORRECT);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_WEIRD_PASS_REPLY", CURLE_FTP_WEIRD_PASS_REPLY);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_WEIRD_USER_REPLY", CURLE_FTP_WEIRD_USER_REPLY);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_WEIRD_PASV_REPLY", CURLE_FTP_WEIRD_PASV_REPLY);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_WEIRD_227_FORMAT", CURLE_FTP_WEIRD_227_FORMAT);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_CANT_GET_HOST", CURLE_FTP_CANT_GET_HOST);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_CANT_RECONNECT", CURLE_FTP_CANT_RECONNECT);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_COULDNT_SET_BINARY", CURLE_FTP_COULDNT_SET_BINARY);
		CFunctions::createCurlGlobal(luaVM, "CURLE_PARTIAL_FILE", CURLE_PARTIAL_FILE);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_COULDNT_RETR_FILE", CURLE_FTP_COULDNT_RETR_FILE);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_WRITE_ERROR", CURLE_FTP_WRITE_ERROR);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_QUOTE_ERROR", CURLE_FTP_QUOTE_ERROR);
		CFunctions::createCurlGlobal(luaVM, "CURLE_HTTP_NOT_FOUND", CURLE_HTTP_NOT_FOUND);
		CFunctions::createCurlGlobal(luaVM, "CURLE_WRITE_ERROR", CURLE_WRITE_ERROR);
		CFunctions::createCurlGlobal(luaVM, "CURLE_MALFORMAT_USER", CURLE_MALFORMAT_USER);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_COULDNT_STOR_FILE", CURLE_FTP_COULDNT_STOR_FILE);
		CFunctions::createCurlGlobal(luaVM, "CURLE_READ_ERROR", CURLE_READ_ERROR);
		CFunctions::createCurlGlobal(luaVM, "CURLE_OUT_OF_MEMORY", CURLE_OUT_OF_MEMORY);
		CFunctions::createCurlGlobal(luaVM, "CURLE_OPERATION_TIMEOUTED", CURLE_OPERATION_TIMEOUTED);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_COULDNT_SET_ASCII", CURLE_FTP_COULDNT_SET_ASCII);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_PORT_FAILED", CURLE_FTP_PORT_FAILED);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_COULDNT_USE_REST", CURLE_FTP_COULDNT_USE_REST);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_COULDNT_GET_SIZE", CURLE_FTP_COULDNT_GET_SIZE);
		CFunctions::createCurlGlobal(luaVM, "CURLE_HTTP_RANGE_ERROR", CURLE_HTTP_RANGE_ERROR);
		CFunctions::createCurlGlobal(luaVM, "CURLE_HTTP_POST_ERROR", CURLE_HTTP_POST_ERROR);
		CFunctions::createCurlGlobal(luaVM, "CURLE_SSL_CONNECT_ERROR", CURLE_SSL_CONNECT_ERROR);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_BAD_DOWNLOAD_RESUME", CURLE_FTP_BAD_DOWNLOAD_RESUME);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FILE_COULDNT_READ_FILE", CURLE_FILE_COULDNT_READ_FILE);
		CFunctions::createCurlGlobal(luaVM, "CURLE_LDAP_CANNOT_BIND", CURLE_LDAP_CANNOT_BIND);
		CFunctions::createCurlGlobal(luaVM, "CURLE_LDAP_SEARCH_FAILED", CURLE_LDAP_SEARCH_FAILED);
		CFunctions::createCurlGlobal(luaVM, "CURLE_LIBRARY_NOT_FOUND", CURLE_LIBRARY_NOT_FOUND);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FUNCTION_NOT_FOUND", CURLE_FUNCTION_NOT_FOUND);
		CFunctions::createCurlGlobal(luaVM, "CURLE_ABORTED_BY_CALLBACK", CURLE_ABORTED_BY_CALLBACK);
		CFunctions::createCurlGlobal(luaVM, "CURLE_BAD_FUNCTION_ARGUMENT", CURLE_BAD_FUNCTION_ARGUMENT);
		CFunctions::createCurlGlobal(luaVM, "CURLE_BAD_CALLING_ORDER", CURLE_BAD_CALLING_ORDER);
		CFunctions::createCurlGlobal(luaVM, "CURLE_HTTP_PORT_FAILED", CURLE_HTTP_PORT_FAILED);
		CFunctions::createCurlGlobal(luaVM, "CURLE_BAD_PASSWORD_ENTERED", CURLE_BAD_PASSWORD_ENTERED);
		CFunctions::createCurlGlobal(luaVM, "CURLE_TOO_MANY_REDIRECTS", CURLE_TOO_MANY_REDIRECTS);
		CFunctions::createCurlGlobal(luaVM, "CURLE_UNKNOWN_TELNET_OPTION", CURLE_UNKNOWN_TELNET_OPTION);
		CFunctions::createCurlGlobal(luaVM, "CURLE_TELNET_OPTION_SYNTAX", CURLE_TELNET_OPTION_SYNTAX);
		CFunctions::createCurlGlobal(luaVM, "CURLE_OBSOLETE", CURLE_OBSOLETE);
		CFunctions::createCurlGlobal(luaVM, "CURLE_SSL_PEER_CERTIFICATE", CURLE_SSL_PEER_CERTIFICATE);
		CFunctions::createCurlGlobal(luaVM, "CURLE_GOT_NOTHING", CURLE_GOT_NOTHING);
		CFunctions::createCurlGlobal(luaVM, "CURLE_SSL_ENGINE_NOTFOUND", CURLE_SSL_ENGINE_NOTFOUND);
		CFunctions::createCurlGlobal(luaVM, "CURLE_SSL_ENGINE_SETFAILED", CURLE_SSL_ENGINE_SETFAILED);
		CFunctions::createCurlGlobal(luaVM, "CURLE_SEND_ERROR", CURLE_SEND_ERROR);
		CFunctions::createCurlGlobal(luaVM, "CURLE_RECV_ERROR", CURLE_RECV_ERROR);
		CFunctions::createCurlGlobal(luaVM, "CURLE_SHARE_IN_USE", CURLE_SHARE_IN_USE);
		CFunctions::createCurlGlobal(luaVM, "CURLE_SSL_CERTPROBLEM", CURLE_SSL_CERTPROBLEM);
		CFunctions::createCurlGlobal(luaVM, "CURLE_SSL_CIPHER", CURLE_SSL_CIPHER);
		CFunctions::createCurlGlobal(luaVM, "CURLE_SSL_CACERT", CURLE_SSL_CACERT);
		CFunctions::createCurlGlobal(luaVM, "CURLE_BAD_CONTENT_ENCODING", CURLE_BAD_CONTENT_ENCODING);
		CFunctions::createCurlGlobal(luaVM, "CURLE_LDAP_INVALID_URL", CURLE_LDAP_INVALID_URL);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FILESIZE_EXCEEDED", CURLE_FILESIZE_EXCEEDED);
		CFunctions::createCurlGlobal(luaVM, "CURLE_FTP_SSL_FAILED", CURLE_FTP_SSL_FAILED);

		// Curl proxy variables
		CFunctions::createCurlGlobal(luaVM, "CURLPROXY_HTTP", CURLPROXY_HTTP);
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
		CFunctions::createCurlGlobal(luaVM, "CURLM_INTERNAL_ERROR", CURLM_INTERNAL_ERROR);

		// Curl message variable
		CFunctions::createCurlGlobal(luaVM, "CURLMSG_DONE", CURLMSG_DONE);
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
