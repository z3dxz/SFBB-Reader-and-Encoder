#include <windows.h>
#include <dwmapi.h>
#include "resource.h"
#include <gdiplusenums.h>
#include <stdio.h>
#include <math.h>

LRESULT CALLBACK WndProc(HWND hnnd, UINT msg, WPARAM wparam, LPARAM lparam);

void* bmpw;
int imgwidth; int imgheight;

int viewimage(int img_width, int img_height, void* bmp){
	bmpw = bmp;
	imgwidth = img_width;
	imgheight = img_height;

	const wchar_t* class_name = L"class";

	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(wc);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = class_name;
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wc);


	CreateWindowEx(0, class_name, L"View Image", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 50, 50, 768, 512, 0, 0, 0, 0);

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {

	case WM_CREATE: {
		BOOL enable = 1;

		DwmSetWindowAttribute(hwnd, 20, &enable, sizeof(enable));

		break;
	}

	case WM_PAINT: {

		PAINTSTRUCT ps = { 0 };
		HDC hdc = BeginPaint(hwnd, &ps);


		RECT cli;

		if (!GetClientRect(hwnd, &cli)) {
			printf("sorry, can not find the rect of the window\n");
			return -1;
		}
		int w_width = cli.right - cli.left;
		int w_height = cli.bottom - cli.top;

		int rectS = 10;

		float fnumx = (float)w_width / (float)rectS;

		float fnumy = (float)w_height / (float)rectS;

		int numx = (int)ceil(fnumx);
		int numy = (int)ceil(fnumy);


		for (int y = 0; y < numy; y++) {
			for (int x = 0; x < numx; x++) {
				if ((x+(y%2)) % 2 > .1) {
					//graphics.FillRectangle(&ForegroundPen, x * rectS, y * rectS, rectS, rectS);
				}
			}
		}


		// prepare BITMAPINFO
		BITMAPINFO bminfo = {};
		bminfo.bmiHeader.biSize = sizeof(BITMAPINFO);
		bminfo.bmiHeader.biWidth = imgwidth;
		bminfo.bmiHeader.biHeight = -imgheight;
		bminfo.bmiHeader.biBitCount = 32;
		bminfo.bmiHeader.biCompression = BI_RGB;
		bminfo.bmiHeader.biPlanes = 1;
		bminfo.bmiHeader.biSizeImage = imgwidth * imgheight * 4; // 4 stands for 32bpp
		// select stretch mode
		// draw
		int offset_x = 0;
		int offset_y = 0;

		int d_width = imgwidth;
		int d_height = imgheight;

		float asp_wnd = (float) w_width / (float) w_height;
		float asp_img = (float)imgwidth / (float)imgheight;

		if (asp_wnd > asp_img) {
			d_height = w_height;
			d_width = (w_height * imgwidth) / imgheight;
			offset_x = (w_width / 2) - (d_width / 2);
			offset_y = 0;
		}
		else {
			d_height = (w_width * imgheight) / imgwidth;
			d_width = w_width;
			offset_x = 0;
			offset_y = (w_height / 2) - (d_height / 2);
		}
		

		SetStretchBltMode(hdc, HALFTONE);
		StretchDIBits(hdc, offset_x, offset_y, d_width, d_height, 0, 0, imgwidth, imgheight, bmpw, &bminfo, DIB_RGB_COLORS, SRCCOPY);



		EndPaint(hwnd, &ps);
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);

	}
	return 0;
}

