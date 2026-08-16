project "App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "off"

   files 
   { "Source/**.h",
     "Source/**.cpp",
     "../Vendor/stb_image/stb_image.h",
     "../Vendor/stb_image/stb_image.cpp",
     "../Vendor/HashForge/SHA256/SHA256.cpp",
     "../Vendor/HashForge/SHA256/SHA256.h",
    }

   includedirs
   {
      "Source",

      -- Include Core
      "../Core/Source",
      "../Core/Source/tree",
      "../Core/Source/Folder Explorer",
      "../Core/Source/Bookmarks",
      "../Core/Source/Renderer",
      "../Core/Source/Tasks Manager",
      
      "../Vendor/stb_image",
      "../Vendor/imgui-docking",
      "../Vendor/imgui-docking/backends",
      "../Vendor/imgui-docking/misc/cpp",
      "../Vendor/imgui-docking/misc/freetype",
      "../Vendor/HashForge/SHA256"
   }

   links
   {
      "Core",
      "ImGui"
   }

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
    debugdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }
       buildoptions { "/utf-8" }
       postbuildcommands
       {
        "robocopy \"../Vendor/assets\" \"%{cfg.targetdir}/assets\" /E /NJH /NJS /NDL /NFL || exit 0"
       }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"