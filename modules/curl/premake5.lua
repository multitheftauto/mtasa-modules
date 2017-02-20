
project "Curl"

    language "C++"
    kind "SharedLib"
    targetname "curl"
    targetprefix "ml_"

    vpaths {
        ["Headers"] = "**.h",
        ["Sources"] = "**.cpp"
    }

    files {
        "**.h",
        "**.cpp"
    }

    includedirs {
        ".", 
        "../../includes"
    }

    -- libcurl: curl-7.50.3

    filter "system:windows"
        defines { "LUA_BUILD_AS_DLL", "CURL_STATICLIB" }
        linkoptions { "legacy_stdio_definitions.lib" }
        links { "Ws2_32", "Wldap32" }

    filter { "system:windows", "configurations:Debug" }
        ignoredefaultlibraries { "MSVCRT" }

    -- Link to platform and configuration dependent lua5.1 and libcurl library 
    filter { "system:windows", "platforms:x32", "configurations:Release" }
        links { "libs/x32/libcurl", "../../libs/x32/lua5.1" }

    filter { "system:windows", "platforms:x32", "configurations:Debug" }
        links { "libs/x32/libcurl_d", "../../libs/x32/lua5.1_d" }

    filter { "system:windows", "platforms:x64", "configurations:Release" }
        links { "libs/x64/libcurl", "../../libs/x64/lua5.1" }

    filter { "system:windows", "platforms:x64", "configurations:Debug" }
        links { "libs/x64/libcurl_d", "../../libs/x64/lua5.1_d" }

    -- Set platform dependent target directory
    filter "platforms:x32"
        binpath "x32"

    filter "platforms:x64"
        binpath "x64"
