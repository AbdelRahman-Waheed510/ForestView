-- premake5.lua
workspace "ForestView"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   flags { "MultiProcessorCompile" }
   
   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Dependencies"
   include "Vendor/imgui-docking/ImGui_Build"
group ""

group "Core"
   include "Core/Build-Core.lua"
group ""

include "App/Build-App.lua"