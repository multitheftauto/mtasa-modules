if _ACTION ~= "vs2022" then
    print("This project needs to be compiled for VisualStudio 2022")
end

local SOLUTION_DIR
local PLATFORM_TARGET
local CONFIGURATION
local BUILD_PATH
local INTERMEDIATE_PATH

if os.host() == 'windows' then
    SOLUTION_DIR = '$(SolutionDir)'
    PLATFORM_TARGET = '$(PlatformTarget)'
    CONFIGURATION = '$(Configuration)'
    INTERMEDIATE_PATH = '!$(OutDir)Intermediate/'
else
    SOLUTION_DIR = path.getabsolute('.')..'/'
    PLATFORM_TARGET = '%{cfg.platform}'
    CONFIGURATION = '%{cfg.buildcfg}'
    INTERMEDIATE_PATH = ('%sIntermediate/'):format(BUILD_PATH)
end

BUILD_PATH = ('%s%s/%s/'):format(SOLUTION_DIR, PLATFORM_TARGET, CONFIGURATION)

workspace "ml_basic"
	configurations { "Debug", "Release" }
    platforms { "x86", "x64" }

    project "ml_basic"
        kind "SharedLib"
        cppdialect "C++20"
        cdialect "C17"
        language "C++"

        vpaths {
            ["Headers/*"] = { "hpp/**.hpp", "include/**.h", "include/**.hpp" },
            ["Sources/*"] = { "src/**.cpp", "lib/**.c", "lib/**.cpp" },
            ["*"] = "premake5.lua"
        }

        files {
            "**.c",
            "**.cpp",
            "**.h",
            "**.hpp",
        }

        includedirs {
            SOLUTION_DIR.."hpp",
            SOLUTION_DIR.."include"
        }

        targetdir (BUILD_PATH)
        objdir (INTERMEDIATE_PATH)

        staticruntime "off"
        flags {
            "MultiProcessorCompile",
        }

        filter ""
            functionlevellinking "On"

        filter "configurations:Debug"
            defines { "_DEBUG" }
            symbols "On"
            optimize "Off"
            runtime "Debug"

        filter "configurations:Release"
            defines { "NDEBUG" }
            symbols "Off"
            optimize "Full"
            runtime "Release"

            flags {
                "LinkTimeOptimization",
                "NoIncrementalLink"
            }

	    filter "system:windows"
            defines { "_WIN32" }
        