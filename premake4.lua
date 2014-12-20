newaction {
    trigger     = "setup",
    description = "Setup resource files within generated files.",
    execute     = function ()
        if os.is("macosx") or os.is("linux") then
            local base = path.getabsolute(".")
            local res  = base .. "/res"
            local rel  = base .. "/out/release"
            local deb  = base .. "/out/debug"

            os.mkdir(rel)
            os.mkdir(deb)
            os.execute(table.concat({"cp -r", res, deb}, " "))
            os.execute(table.concat({"cp -r", res, rel}, " "))
        end
   end
}

solution "glcookbook"
    configurations {"Debug", "Release"}
        language "C++"

        defines {
            "GLFW_STATIC",
            "GLEW_STATIC",
            "GLM_FORCE_RADIANS"
        }

        libdirs {
            os.findlib("glfw3"),
            os.findlib("glew"),
            os.findlib("freeimageplus")
        }

        links {
            "freeimageplus"
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

    configuration {"linux"}
        links {
            "glfw3",
            "GLEW",
            "GL",
            "X11",
            "Xxf86vm",
            "Xrandr",
            "Xinerama",
            "Xi",
            "Xcursor",
            "pthread"
        }

    configuration "Release"
        defines {"NDEBUG"}
        objdir "out/release/obj"
        targetdir "out/release"
        kind "WindowedApp"
        flags {"OptimizeSpeed"}

    configuration "Debug"
        defines {"DEBUG"}
        targetdir "out/debug"
        objdir "out/debug/obj"
        kind "ConsoleApp"
        flags {"Symbols"}

    project "window_creation"
        location "build/window_creation"
        files {
            "src/window_creation/**.cpp",
            "src/window_creation/**.h"
        }

    project "triangle"
        location "build/triangle"
        files {
            "src/triangle/**.cpp",
            "src/triangle/**.h"
        }

    project "rectangle"
        location "build/rectangle"
        files {
            "src/rectangle/**.cpp",
            "src/rectangle/**.h"
        }

    project "basic_shader"
        location "build/basic_shader"
        files {
            "src/basic_shader/**.cpp",
            "src/basic_shader/**.h"
        }

    project "textures"
        location "build/textures"
        files {
            "src/textures/**.cpp",
            "src/textures/**.h"
        }

    project "transform"
        location "build/transform"
        files {
            "src/transform/**.cpp",
            "src/transform/**.h"
        }

    project "coordinates"
        location "build/coordinates"
        files {
            "src/coordinates/**.cpp",
            "src/coordinates/**.h"
        }

    project "camera"
        location "build/camera"
        files {
            "src/camera/**.cpp",
            "src/camera/**.h"
        }

    project "color"
        location "build/color"
        files {
            "src/color/**.cpp",
            "src/color/**.h"
        }