#include "main.h"

int main()
{
	int32_t exitCode = 0;

	HWND hWnd = NULL;
	hWnd = CreateWindowInstance(L"Test Window", WindowProc);
	
	InitializeVulkan(hWnd, "Test Window");

	MSG	msg = {0};
	while (GetAndDispatchWindowMessage(hWnd, &msg) == true)
	{
		Draw();
	}

	exitCode = (uint32_t) msg.wParam;

	DestroyVulkan(hWnd);

	return exitCode;
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(CLOSE_UI_EXIT_CODE);
		return 0;
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}