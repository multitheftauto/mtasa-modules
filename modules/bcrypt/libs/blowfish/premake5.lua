
project "blowfish"

    language "C++"
    kind "StaticLib"
    targetname "blowfish"
    targetprefix ""

    pic "On"

    vpaths { 
        ["Headers"] = "**.h",
        ["Sources"] = { "**.c", "**.S" },
    }
    
    files {
        "*.h",
        "*.c",
        "*.S"
    }
