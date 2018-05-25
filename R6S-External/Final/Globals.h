#pragma once

#include <Windows.h>
#include <string>
#include "Memory.h"
#include "Overlay.h"

//Global variables to be used throughout the project
namespace Global {
	//Handle to the game
	extern HANDLE GameHandle;

	//Base address of the game
	extern LPVOID BaseAddress;

	//Local player name, used for finding local entity by comparing names
	extern std::string LocalName;

	//Instance of memory class
	extern Memory Mem;

	//Instance of Overlay class
	extern Overlay Over;

	void GetResolution(UINT horizontal, UINT vertical);

	extern BOOL Box;
	extern BOOL Health;
	extern BOOL Name;
	extern BOOL Head;
	extern BOOL Snapline;
	extern BOOL Fov;
	extern BOOL Aimbot;

	extern BOOL Menu;
}