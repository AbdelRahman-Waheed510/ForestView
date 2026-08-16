project "Core"
   kind "StaticLib"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",                 
      "Source/tree",            
      "Source/Folder Explorer",
      "Source/Bookmarks",
      "source/Renderer",
      "source/Tasks Manager",

      "../Vendor/stb_image",
      "../Vendor/imgui-docking",
      "../Vendor/imgui-docking/backends",
      "../Vendor/imgui-docking/misc/cpp",
      "../Vendor/imgui-docking/misc/freetype",
      "../Vendor/HashForge/SHA256"
   }

   links
   {
      "ImGui" 
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { }

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