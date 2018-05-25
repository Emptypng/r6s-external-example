#include "Globals.h"
#include "Vectors.h"
#include "Memory.h"
#include "Overlay.h"
#include <iostream>
#include <time.h>
#include <Psapi.h>
#include "Helpers.h"
#include "Input.h"

HWND gameWindow = FindWindow(NULL, L"Rainbow Six");

//Defining our globals
namespace Global {
	HANDLE GameHandle = 0x0;

	LPVOID BaseAddress = NULL;

	std::string LocalName = "Name";

	Memory Mem = Memory();

	Overlay Over = Overlay();

	BOOL Box = false;
	BOOL Health = false;
	BOOL Name = false;
	BOOL Head = false;
	BOOL Snapline = false;
	BOOL Fov = false;
	BOOL Aimbot = false;

	BOOL Menu = true;
}

DWORD FindProcessId(const std::wstring& processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}

void OpenHandle() {
	DWORD processID = FindProcessId(L"RainbowSix.exe");
	Global::GameHandle = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, processID);
}

static const char consoleNameAlphanum[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

int consoleNameLength = sizeof(consoleNameAlphanum) - 1;

char genRandomConsoleName()
{
	return consoleNameAlphanum[rand() % consoleNameLength];
}

int main()
{
	srand(time(0));
	std::wstring ConsoleNameStr;
	for (unsigned int i = 0; i < 20; ++i)
	{
		ConsoleNameStr += genRandomConsoleName();

	}
	SetConsoleTitle(ConsoleNameStr.c_str());
	Helpers::Enable("Siege External by Yao");
	std::cout << "<------------------->" << std::endl;
	if (gameWindow != 0) {
		std::cout << "Game window found" << std::endl;

		OpenHandle();

		std::cout << "Local name: " << std::flush;
		std::cin >> Global::LocalName;

		Sleep(1000);

		if (Global::GameHandle == INVALID_HANDLE_VALUE || Global::GameHandle == NULL || Global::GameHandle == (HANDLE)0x0) {
			std::cout << "Invalid handle to R6" << std::endl;
			system("pause");
			return 1;
		}

		Global::Mem.SetBaseAddress();
		std::cout << "Base address: " << std::hex << Global::Mem.GetBaseAddress() << std::dec << std::endl;

		Global::Over.SetupWindow();
		std::cout << "Overlay window set" << std::endl;

		Input::GetInstance()->StartThread();

		//Start the main loop
		Global::Over.Loop();

		system("pause");
		return EXIT_SUCCESS;

		return 0;
	}
	else {
		std::cout << "Game must be running to continue." << std::endl;
		std::cout << "" << std::endl;
		system("pause");
		return 0;
	}
}

void Global::GetResolution(UINT horizontal, UINT vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = FindWindowA(NULL, "Rainbow Six");
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
