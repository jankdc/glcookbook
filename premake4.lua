solution "glcookbook"
    configurations {"Release", "Debug"}
        language "C++"

        defines {
            "GLFW_STATIC",
            "GLEW_STATIC"
        }

        libdirs {
            os.findlib("glfw3"),
            os.findlib("glew")
        }

    configuration {"gmake"}
        buildoptions {
            "-Wall",
            "-std=c++11"
        }

    configuration {"macosx"}
        links {
            "glfw3",
            "GLEW",
            "Cocoa.framework",
            "OpenGL.framework",
            "IOKit.framework",
            "CoreVideo.framework"
        }

    configuration "Release"
        defines {"NDEBUG"}
        objdir "out/release/obj"
        targetdir "out/release"
        kind "WindowedApp"

    configuration "Debug"
        defines {"DEBUG"}
        targetdir "out/debug"
        objdir "out/debug/obj"
        kind "ConsoleApp"

    project "window_creation"
        location "build/window_creation"
        files {
            "src/window_creation/**.cpp", 
            "src/window_creation/**.h"
        }
