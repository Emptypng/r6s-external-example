#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx9.h>
#include <Dwmapi.h> 
#include <TlHelp32.h>
#include "Vectors.h"

class Overlay {

public:
	//Constructor
	Overlay();

	int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);

	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	LPDIRECT3D9 d3;
	LPDIRECT3DDEVICE9 d3dev;

	LPD3DXFONT d3Font;

	HWND hWindow;
	HWND tWindow;

	MARGINS margins = { 0, 0, ScreenWidth, ScreenHeight };

	ID3DXLine* d3Line;

	//Initializes our direct3Dx9 instance
	void InitD3D(HWND hWindow);

	//Renders our drawings to the screen
	void Render();

	//Sets up the overlay window
	void SetupWindow();

	//Main loop that calls the esp then renders it
	WPARAM Loop();

	//Esp stuff
	void MainLoop();

	void DrawCircle(int X, int Y, int radius, int numSides, DWORD Color);

	void FillArea(float x, float y, float width, float height, D3DCOLOR color);

	void FillRectangle(double x, double y, double w, double h, D3DCOLOR color);

	void DrawBox(float x, float y, float width, float height, D3DCOLOR color);

	void DrawLine(float x1, float y1, float x2, float y2, D3DCOLOR color);

	void DrawString(int x, int y, DWORD color, LPD3DXFONT g_pFont, const char * fmt, ...);
};