#pragma once

#ifdef GLN_PLATFORM_WINDOWS
	#ifdef GLN_BUILD_DLL
		#define GLN_API __declspec(dllexport)
	#else
		#define GLN_API __declspec(dllimport)
	#endif
#else
	#error Glen only supports Windows for now!
#endif