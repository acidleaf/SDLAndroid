newaction {
	trigger = "android",
	description = "Install on Android",
	execute = function ()
		os.execute("(cd android && ndk-build && ant debug && ant debug install)")
	end
}
	
solution "SDLAndroid"
	configurations { "PC", "Android" }
	
	project "SDLAndroid"
		targetname "SDLAndroid"
		kind "ConsoleApp"
		language "C++"
		
		files { "src/**.cc" }
		includedirs { "include", "include/**" }
		links { "SDL2" }
		
		buildoptions { "--std=c++11" }
		
		includedirs { "/usr/local/include" }
		libdirs { "/usr/local/lib"}
		--linkoptions { "-framework OpenGL" }
		
		
		configuration "PC"
			targetdir "bin/PC"
			objdir "obj/PC"
			flags { "Symbols" }
			
			buildoptions { "-Wall" }
			
		configuration "Android"
			targetdir "bin/Android"
			objdir "obj/Android"
			flags { "Optimize" }