#include "window.h"

void DestroyWindowInstance(HWND hWnd)
{
	if (DestroyWindow(hWnd) == FALSE)
	{
		error("Failed to Destroy Window");
	}
}

BOOL GetAndDispatchWindowMessage(HWND hWnd, MSG* msgContainer)
{
	if (PeekMessageW(msgContainer, hWnd, 0, 0, PM_REMOVE) == FALSE)
		return TRUE;

	if (msgContainer->message == WM_QUIT)
		return FALSE;

	TranslateMessage(msgContainer);
	DispatchMessageW(msgContainer);

	return TRUE;
}

HWND CreateWindowInstance(wchar_t* windowName, WNDPROC wndProc)
{
	HMODULE procHandle = GetModuleHandleA(NULL);

	if (procHandle == NULL)
	{
		error("Failed to get Process Handle");
		return NULL;
	}

	WNDCLASSEXW wndClassA = { 0 };
	wndClassA.cbSize = sizeof(WNDCLASSEXA);
	wndClassA.lpszClassName = windowName;
	wndClassA.lpfnWndProc = wndProc;
	wndClassA.hInstance = GetModuleHandleA(NULL);

	if (RegisterClassExW(&wndClassA) == FALSE)
	{
		error("Failed to Register Window Class.");
		return NULL;
	}

	HWND hWnd = CreateWindowExW(0, windowName, windowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, procHandle, NULL);

	if (hWnd == NULL)
	{
		error("Failed to create Window.");
		return NULL;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);

	return hWnd;
}