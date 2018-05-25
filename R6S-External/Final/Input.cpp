#pragma once

#include "Input.h"
#include "Globals.h"

Input* Input::m_pInstance;

Input::Input()
{

}

Input::~Input()
{

}

void Input::StartThread()
{
	m_hThread = CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(MenuKeyMonitor), NULL, NULL, NULL);
}

void Input::StopThread()
{
	TerminateThread(m_hThread, 0);
}

void Input::MenuKeyMonitor()
{
	HWND gameWindow = FindWindowA(NULL, "Rainbow Six");

	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			if (Global::Menu == true ) {
				Helpers::Disable("Menu Disabled");
			}
			else {
				Helpers::Enable("Menu Enabled");
			}

			Global::Menu = !Global::Menu;

			std::this_thread::sleep_for(
				std::chrono::milliseconds(250));
		}
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			if (Global::Box == true) {
				Helpers::Disable("ESP Box Disabled");
			}
			else {
				Helpers::Enable("ESP Box Enabled");
			}

			Global::Box = !Global::Box;

			std::this_thread::sleep_for(
				std::chrono::milliseconds(250));
		}
		if (GetAsyncKeyState(VK_F2) & 1)
		{
			if (Global::Health == true) {
				Helpers::Disable("ESP Health Disabled");
			}
			else {
				Helpers::Enable("ESP Health Enabled");
			}
			Global::Health = !Global::Health;

			std::this_thread::sleep_for(
				std::chrono::milliseconds(250));
		}
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			if (Global::Name == true) {
				Helpers::Disable("ESP Name Disabled");
			}
			else {
				Helpers::Enable("ESP Name Enabled");
			}

			Global::Name = !Global::Name;

			std::this_thread::sleep_for(
				std::chrono::milliseconds(250));
		}
		if (GetAsyncKeyState(VK_F4) & 1)
		{
			if (Global::Snapline == true) {
				Helpers::Disable("ESP Snapline Disabled");
			}
			else {
				Helpers::Log("<->");
				Helpers::Enable("ESP Snapline Enabled");
			}

			Global::Snapline = !Global::Snapline;

			std::this_thread::sleep_for(
				std::chrono::milliseconds(250));
		}
		if (GetAsyncKeyState(VK_F5) & 1)
		{
			if (Global::Head == true) {
				Helpers::Disable("ESP Head Disabled");
			}
			else {
				Helpers::Enable("ESP Head Enabled");
			}

			Global::Head = !Global::Head;

			std::this_thread::sleep_for(
				std::chrono::milliseconds(250));
		}
		if (GetAsyncKeyState(VK_F6) & 1)
		{
			if (Global::Fov == true) {
				Helpers::Disable("Fov Disabled");
			}
			else {
				Helpers::Enable("Fov Enabled");
			}

			Global::Fov = !Global::Fov;

			std::this_thread::sleep_for(
				std::chrono::milliseconds(250));
		}
		if (GetAsyncKeyState(VK_F7) & 1)
		{
			if (Global::Aimbot == true) {
				Helpers::Disable("Aimbot Disabled");
			}
			else {
				Helpers::Enable("Aimbot Enabled");
			}

			Global::Aimbot = !Global::Aimbot;

			std::this_thread::sleep_for(
				std::chrono::milliseconds(250));
		}
	}
}

Input* Input::GetInstance()
{
	if (!m_pInstance)
		m_pInstance = new Input();

	return m_pInstance;
}