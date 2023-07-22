--baseName = path.getbasename(os.getcwd());
project ("fmt_lib")
    kind "StaticLib"
    location "../_build"
    targetdir "../_bin/%{cfg.buildcfg}"
    includedirs { "./", "./include"}

    vpaths 
    {
        ["Header Files/*"] = { "include/**.h", "include/**.hpp", "**.h", "**.hpp"},
        ["Source Files/*"] = { "src/**.cpp", "src/**.c", "**.cpp","**.c", "**.cc"},
    }
    files {"**.c", "**.cpp", "**.h", "**.hpp", "**.cc"}
	
	include_raylib()
