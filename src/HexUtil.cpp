#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "Def_List.h"

HINSTANCE hinst;

HWND DECIMAL_STATIC;
HWND HEXADECIMAL_STATIC;

HWND DECIMAL_EDIT;
HWND HEXADECIMAL_EDIT;

char DECIMAL_STRING[128];
char HEXADECIMAL_STRING[128];

bool CHECK = 0;

INT DEC_2_HEX();
INT HEX_2_DEC();


LRESULT WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	static HFONT hFont = (HFONT)NULL;

	hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_SWISS, "Segoe UI");

	switch (iMsg) {
	case WM_CREATE:
		AllocConsole();
		freopen("CON", "w", stdout);

		DECIMAL_STATIC = CreateWindow("static", "DECIMAL", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_SIMPLE, 7, 13, 60, 25, hWnd, NULL, hinst, NULL);
		SendMessage(DECIMAL_STATIC, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));
		DECIMAL_EDIT = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | ES_LEFT | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 90, 10, 335, 25, hWnd, (HMENU)DECIMAL_MENU, hinst, NULL);

		HEXADECIMAL_STATIC = CreateWindow("static", "HEX + 00", WS_CHILD | WS_VISIBLE | SS_LEFT | SS_SIMPLE, 7, 41, 60, 25, hWnd, NULL, hinst, NULL);
		SendMessage(HEXADECIMAL_STATIC, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));
		HEXADECIMAL_EDIT = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | ES_LEFT | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 90, 35, 335, 25, hWnd, (HMENU)HEXADECIMAL_MENU, hinst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case DECIMAL_MENU:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				if (CHECK == 0) {
					GetWindowText(DECIMAL_EDIT, DECIMAL_STRING, 128);
					DEC_2_HEX();
					//printf("D : %s\n", DECIMAL_STRING);
					CHECK = 1;
					SetWindowText(HEXADECIMAL_EDIT, HEXADECIMAL_STRING);

					//printf("H : %s\n", HEXADECIMAL_STRING);
				}
				else {
					CHECK = 0;
				}
				break;
			}
			break;
		case HEXADECIMAL_MENU:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				if (CHECK == 0) {
					GetWindowText(HEXADECIMAL_EDIT, HEXADECIMAL_STRING, 128);
					HEX_2_DEC();
					CHECK = 1;
					SetWindowText(DECIMAL_EDIT, DECIMAL_STRING);
				}
				else {
					CHECK = 0;
				}
				break;
			}
			break;
		}
		return 0;
	case WM_DESTROY:
		FreeConsole();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
INT WINAPI WinMain(HINSTANCE hinstance, HINSTANCE HinstPrev, PSTR szCmdLine, int iCmdShow) {
	WNDCLASS WC;
	MSG msg;

	hinst = hinstance;

	WC.style = CS_HREDRAW | CS_VREDRAW;
	WC.lpfnWndProc = WndProc;
	WC.cbClsExtra = 0;
	WC.cbWndExtra = 0;
	WC.hInstance = hinstance;
	WC.hIcon = LoadIcon(hinst, IDI_APPLICATION);
	WC.hCursor = LoadCursor(hinst, IDC_ARROW);
	WC.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(240, 240, 240));
	WC.lpszClassName = "MAIN";

	RegisterClass(&WC);

	HWND MAIN = CreateWindow("MAIN", "HexUtil", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, 450, 200, NULL, NULL, hinst, NULL);

	ShowWindow(MAIN, iCmdShow);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
INT DEC_2_HEX() {
	char LIST_HEX[] = "0123456789ABCDEF";
	int dec = atoi(DECIMAL_STRING);
	int m;
	int n;
	int k = 0;
	memset(HEXADECIMAL_STRING, 0, sizeof(HEXADECIMAL_STRING));
	while (1)
	{
		m = dec / 16;
		n = dec - (m * 16);
		HEXADECIMAL_STRING[k++] = LIST_HEX[n];
		if (m <= 0) break;
		dec = m;
	}
	_strrev(HEXADECIMAL_STRING);
	if (HEXADECIMAL_STRING[0] == 48) {
		HEXADECIMAL_STRING[0] = '\0';
	}
	return 0;
}
INT HEX_2_DEC() {
	int dec = strtol(HEXADECIMAL_STRING, NULL, 16);
	sprintf(DECIMAL_STRING, "%d", dec);
	
	if (HEXADECIMAL_STRING[0] == 48) {
		HEXADECIMAL_STRING[0] = '\0';
	}

	return 0;
}