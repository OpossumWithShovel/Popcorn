#include "framework.h"
#include "Main.h"

// Global Variables:
AsMain_Window Main_Window;

//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	return Main_Window.Main(hInstance, nCmdShow);
}
//------------------------------------------------------------------------------------------------------------




// AsFrame_DC
//------------------------------------------------------------------------------------------------------------
AsFrame_DC::~AsFrame_DC()
{
	if (Bitmap != 0)
		DeleteObject(Bitmap);

	if (DC != 0)
		DeleteObject(DC);
}
//------------------------------------------------------------------------------------------------------------
AsFrame_DC::AsFrame_DC()
	: Width(0), Height(0), DC(0), Bitmap(0)
{
}
//------------------------------------------------------------------------------------------------------------
HDC AsFrame_DC::Get_DC(HWND hwnd, HDC hdc)
{
	int frame_dc_width, frame_dc_height;
	RECT rect;

	GetClientRect(hwnd, &rect);

	frame_dc_width = rect.right - rect.left;
	frame_dc_height = rect.bottom - rect.top;

	if (Width != frame_dc_width && Height != frame_dc_height)
	{
		if (Bitmap != 0)
			DeleteObject(Bitmap);

		if (DC != 0)
			DeleteObject(DC);

		Width = frame_dc_width;
		Height = frame_dc_height;

		DC = CreateCompatibleDC(hdc);
		Bitmap = CreateCompatibleBitmap(hdc, Width, Height);

		SelectObject(DC, Bitmap);

		AsTools::Rect(DC, rect, AsConfig::BG_Color);
	}

	return DC;
}
//------------------------------------------------------------------------------------------------------------




// AsMain_Window
AsMain_Window *AsMain_Window::Self = 0;
//------------------------------------------------------------------------------------------------------------
AsMain_Window::AsMain_Window()
	: Instance(0), szTitle{}, szWindowClass{}
{
	Self = this;
}
//------------------------------------------------------------------------------------------------------------
int APIENTRY AsMain_Window::Main(HINSTANCE instance, int command_show)
{
	MSG message;
	HACCEL accel_table;

	Instance = instance;

	// Initialize global strings
	LoadStringW(instance, IDS_APP_TITLE, szTitle, Max_Load_String);
	LoadStringW(instance, IDC_POPCORN, szWindowClass, Max_Load_String);
	Register_Class();

	// Perform application initialization:
	if (!Init_Instance(command_show) )
		return FALSE;

	accel_table = LoadAccelerators(instance, MAKEINTRESOURCE(IDC_POPCORN) );

	// Main message loop:
	while (GetMessage(&message, nullptr, 0, 0))
		if (!TranslateAccelerator(message.hwnd, accel_table, &message))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

	return (int)message.wParam;
}
//------------------------------------------------------------------------------------------------------------
ATOM AsMain_Window::Register_Class()
{
	WNDCLASSEXW wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Window_Proc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = Instance;
	wcex.hIcon = LoadIcon(Instance, MAKEINTRESOURCE(IDC_POPCORN) );
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = AsConfig::BG_Color.Get_Brush();
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_POPCORN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL) );

	return RegisterClassExW(&wcex);
}
//------------------------------------------------------------------------------------------------------------
BOOL AsMain_Window::Init_Instance(int command_show)
{
	HWND hwnd;
	RECT window_rect{};

	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = 320 * AsConfig::Global_Scale;
	window_rect.bottom = 200 * AsConfig::Global_Scale;

	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

	hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, 0, 0, Instance, 0);

	if (!hwnd)
		return FALSE;

	Engine.Init_Engine(hwnd);

	ShowWindow(hwnd, command_show);
	UpdateWindow(hwnd);

	return TRUE;
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::On_Paint(HWND hwnd)
{
	HDC hdc, frame_dc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hwnd, &ps);
	frame_dc = Frame_DC.Get_DC(hwnd, hdc);
	Engine.Draw_Frame(frame_dc, ps.rcPaint);

	BitBlt(hdc, 0, 0, Frame_DC.Width, Frame_DC.Height, frame_dc, 0, 0, SRCCOPY);

	EndPaint(hwnd, &ps);
}
//------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK AsMain_Window::Window_Proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	int wmId;

	switch (message)
	{
	case WM_COMMAND:
	{
		wmId = LOWORD(wparam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(Self->Instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
			break;

		case IDM_EXIT:
			DestroyWindow(hwnd);
			break;

		default:
			return DefWindowProc(hwnd, message, wparam, lparam);
		}
	}
	break;


	case WM_PAINT:
		Self->On_Paint(hwnd);
		break;


	case WM_TIMER:
		if (wparam == AsEngine::Timer_ID)
			return Self->Engine.On_Timer();
		break;


	case WM_KEYDOWN:
		switch(wparam)
		{
		case VK_LEFT:
			return Self->Engine.On_Key(EKey_Type::Left, true);
			break;

		case VK_RIGHT:
			return Self->Engine.On_Key(EKey_Type::Right, true);
			break;

		case VK_SPACE:
			return Self->Engine.On_Key(EKey_Type::Space, true);
			break;
		}
		break;


	case WM_KEYUP:
		switch(wparam)
		{
		case VK_LEFT:
			return Self->Engine.On_Key(EKey_Type::Left, false);
			break;

		case VK_RIGHT:
			return Self->Engine.On_Key(EKey_Type::Right, false);
			break;

		case VK_SPACE:
			return Self->Engine.On_Key(EKey_Type::Space, false);
			break;
		}
		break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	default:
		return DefWindowProc(hwnd, message, wparam, lparam);
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------
INT_PTR CALLBACK AsMain_Window::About(HWND hdlg, UINT message, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(lparam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wparam) == IDOK || LOWORD(wparam) == IDCANCEL)
		{
			EndDialog(hdlg, LOWORD(wparam) );
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
//------------------------------------------------------------------------------------------------------------
