#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h>

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"ddraw.lib")

#define WINDOW_CLASS_NAME "WINCLASS1"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

HWND main_window_handle = NULL;
HINSTANCE hinstance_app = NULL;

char buffer[80];

LPDIRECTDRAW lpdd = NULL;
LPDIRECTDRAW7 lpdd7 = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam){
	PAINTSTRUCT ps;
	HDC hdc;
	char buffer[80];
	switch (msg){
		case WM_CREATE:
			
			return (0);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			
			EndPaint(hwnd, &ps);
			return (0);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return (0);
			break;
		default:break;
	}
	
	return (DefWindowProc(hwnd, msg, wparam, lparam));	
}


int Game_Main(void *params = NULL, int num_params = 0){

	if (KEYDOWN(VK_ESCAPE)){
		SendMessage(main_window_handle, WM_CLOSE, 0, 0);
	}

	return (1);
}

int Game_Init(void *params = NULL, int num_params = 0){
	if (FAILED(DirectDrawCreateEx(NULL, (void**)&lpdd, IID_IDirectDraw7, NULL))){

		return 0;
	}

	lpdd->SetCooperativeLevel(main_window_handle,DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE);

	return (1);
}

int Game_Shutdown(void *params = NULL, int num_params = 0){
	if (lpdd){
		lpdd->Release();
		lpdd = NULL;
	}

	return (1);
}


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow){
	WNDCLASSEX	winclass;
	HWND		hwnd;
	MSG			msg;
	HDC			hdc;

	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	hinstance_app = hinstance;
	if (!RegisterClassEx(&winclass)) { return (0); }

	hwnd = CreateWindowEx(NULL,
						  WINDOW_CLASS_NAME,
						  "T3D Game Console Version 1.0",
						  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
						  0,0,
						  400,300,
						  NULL,
						  NULL,
						  hinstance,
						  NULL);
	if (!hwnd){ return (0); }

	main_window_handle = hwnd;

	Game_Init();

	while (TRUE){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT){break;}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Game_Main();
	}

	Game_Shutdown();
	
	return(msg.wParam);
}