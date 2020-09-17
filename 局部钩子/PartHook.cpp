#include <Windows.h>
#include <strsafe.h>

HHOOK g_hMouseHook;
HHOOK g_hKeyboardHook;
HWND g_hHwnd;

LRESULT CALLBACK MainWinProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//钩子处理函数
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	static TCHAR szAppName[] = TEXT("局部钩子");
	MSG msg;

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = MainWinProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;

	if (!RegisterClass(&wndClass))
	{
		MessageBox(NULL, TEXT("注册窗口类失败"), szAppName, MB_OK);
		return 0;
	}

	g_hHwnd = CreateWindow(
		szAppName,
		TEXT("局部钩子"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(g_hHwnd, nCmdShow);
	UpdateWindow(g_hHwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
	//return 0;
}

LRESULT CALLBACK MainWinProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam)
{
	int x = LOWORD(lParam);
	int y = HIWORD(lParam);
	size_t cchMouseCoordinate = 20;
	TCHAR pszMouseCoordinate[20];
	LPCTSTR pszText = TEXT("x = %d, y = %d");

	switch (uMsg)
	{
	case WM_CREATE:
		g_hMouseHook = SetWindowsHookEx(WH_MOUSE, MouseProc, NULL, GetCurrentThreadId());
		g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, GetCurrentThreadId());
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:
		//MessageBox(NULL, TEXT("鼠标按下"), TEXT("左键测试"), MB_OK);
		return 0;

	case WM_MOUSEMOVE:
		StringCchPrintf(pszMouseCoordinate, cchMouseCoordinate, pszText, x, y);
		SetWindowText(g_hHwnd, pszMouseCoordinate);
		return 0;

	case WM_KEYDOWN:
		MessageBox(NULL, TEXT("键盘按下"), TEXT("测试键盘"), MB_OK);
		return 0;

		//case WM_QUIT:
			//MessageBox(NULL, TEXT("点击关闭按钮"), TEXT("测试"), MB_OK);
			//return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_LBUTTONDOWN)
	{
		MessageBox(NULL, TEXT("鼠标按下"), TEXT("左键测试"), MB_OK);
	}
	return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == VK_SPACE) //屏蔽空格键
	{
		return 1;
	}
	else if (wParam == VK_RETURN)  //卸载钩子
	{
		UnhookWindowsHookEx(g_hMouseHook);
		UnhookWindowsHookEx(g_hKeyboardHook);
		return 1;
	}
	else
	{
		return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
	}
}