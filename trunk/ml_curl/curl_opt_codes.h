#pragma once

#include "curl/curl.h"

#include <utility>
#include <map>

using namespace std;

class curl_opt_codes
{
public:
	curl_opt_codes(void);
	~curl_opt_codes(void);

	typedef pair<const char*,enum DataType> mypair;//define a pair 
    map<const char*,enum DataType> obj;//create an object of Map 
};

