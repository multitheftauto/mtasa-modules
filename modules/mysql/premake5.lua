
project "MySQL"

    language "C++"
    kind "SharedLib"
    targetname "mysql"
    targetprefix "ml_"

    vpaths {
        ["Headers/*"] = "**.h",
        ["Sources"] = "**.cpp"
    }

    files {
        "**.h",
        "**.cpp"
    }

    includedirs {
        ".",
        "include/mysql",
        "../../includes"
    }

    -- libmysql: mysql-connector-c-6.1.6

    filter "system:windows"
        removefiles { "luaimports/luaimports.cpp" }
        defines { "LUA_BUILD_AS_DLL" }
        links { "Ws2_32" }

    -- Link to libmysql library
    filter { "system:windows", "platforms:x32" }
        links { "libs/x32/libmysql" }

    filter { "system:windows", "platforms:x64" }
        links { "libs/x64/libmysql" }

    -- Link to platform and configuration dependent lua5.1 library 
    filter { "system:windows", "platforms:x32", "configurations:Release" }
        links { "../../libs/x32/lua5.1" }

    filter { "system:windows", "platforms:x32", "configurations:Debug" }
        links { "../../libs/x32/lua5.1_d" }

    filter { "system:windows", "platforms:x64", "configurations:Release" }
        links { "../../libs/x64/lua5.1" }

    filter { "system:windows", "platforms:x64", "configurations:Debug" }
        links { "../../libs/x64/lua5.1_d" }

    -- Set platform dependent target directory
    filter "platforms:x32"
        binpath "x32"

    filter "platforms:x64"
        binpath "x64"
