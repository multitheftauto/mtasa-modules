
-- Create helper function for output
cwd = os.getcwd()
binpath = function (path) targetdir(os.realpath(("%s/bin/%s"):format(cwd, path))) end

workspace "MTA Modules"

    configurations { "Debug", "Release" }
    platforms { "x32", "x64" }
    
    location "build"
    startproject "System"

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "_SCL_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE"
	}

    filter "platforms:x32"
        architecture "x32"

    filter "platforms:x64"
        architecture "x64"

    filter "system:windows"
        defines { "WIN32", "_WIN32" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        flags { "C++14" }
        symbols "On"
        targetsuffix "_d"

    filter "configurations:Release"
        defines { "NDEBUG" }
        flags { "C++14", "Optimize" }
        optimize "On"

    include "modules/sockets"
    include "modules/curl"
    include "modules/mysql"
    include "modules/system"
    include "modules/bcrypt"

    -- Enable the project for Visual Studio
    -- include "modules/texturizer"
