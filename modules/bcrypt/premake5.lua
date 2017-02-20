
group "bcrypt"

include "libs/blowfish"

project "bcrypt"

    language "C++"
    kind "SharedLib"
    targetname "bcrypt"
    targetprefix "ml_"

    vpaths {
        ["Headers"] = "**.h",
        ["Sources"] = "**.cpp"
    }

    files {
        "*.h",
        "*.cpp",
        "extra/*.h",
        "extra/*.cpp",
        "include/*.h"
    }

    includedirs { 
        "../../includes",
        "include", 
        "lib/blowfish"
    }

    links { "blowfish" }

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

group ""
