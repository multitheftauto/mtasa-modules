
project "Texturizer"

    language "C++"
    kind "SharedLib"
    targetname "texturizer"
    targetprefix "ml_"

    vpaths {
        ["Headers"] = "**.h",
        ["Headers/TextureWriter"] = { "TextureWriter/**.h", "TextureWriter/**.hpp" },
        ["Sources"] = "**.cpp",
        ["Sources/TextureWriter"] = "TextureWriter/**.cpp"
    }

    files {
        "src/**.h",
        "src/**.cpp",
        "TextureWriter/**.h",
        "TextureWriter/**.hpp",
        "TextureWriter/**.cpp",
    }

    -- https://code.google.com/archive/p/libsquish/
    -- libsquish: squish-1.11

    -- https://github.com/libgd/libgd/releases/tag/gd-2.2.3
    -- libgd: gd-2.2.3

    includedirs {
        "src",
        "src/extra",
        "src/include",
        "TextureWriter",
        "../../includes"
    }

    removefiles {
        "TextureWriter/AppMain.cpp"
    }

    -- Note: This project needs more love to be compiled

    filter "system:windows"
        flags { "ExcludeFromBuild" }
        defines { "LUA_BUILD_AS_DLL" }

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
