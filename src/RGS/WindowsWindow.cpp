#include "WindowsWindow.h"
#include "Base.h"
#include<iostream>
#include<Windows.h>

#define RGS_WINDOW_ENTRY_NAME "Entry"
#define RGS_WINDOW_CLASS_NAME "Class"
namespace RGS
{
	bool WindowsWindow::s_Inited = false;
	void WindowsWindow::Init()
	{
		ASSERT(!s_Inited);
		Register();
		s_Inited = true;
	}
	void WindowsWindow::Terminate()
	{
		ASSERT(s_Inited);
		UnRegister();
		s_Inited = false;
	}
	void WindowsWindow::Register()
	{
		ATOM atom;
		WNDCLASS wc = { 0 };
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hbrBackground = (HBRUSH)(WHITE_BRUSH);	//背景色
		wc.hCursor = NULL;							//默认光标
		wc.hIcon = NULL;							//默认图标
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpfnWndProc = WindowsWindow::WndProC;	//窗口处理函数
		wc.lpszClassName = RGS_WINDOW_CLASS_NAME;
		wc.style = CS_HREDRAW | CS_VREDRAW;			//拉伸时重绘
		wc.lpszMenuName = NULL;						//不要菜单
		atom = RegisterClass(&wc);					//注册窗口

	}
	void WindowsWindow::UnRegister()
	{
		UnregisterClass(RGS_WINDOW_CLASS_NAME, GetModuleHandle(NULL));
	}
	void WindowsWindow::KeyPressImp(WindowsWindow* window, const WPARAM wParam, const char keyState)
	{
		if (wParam >= '0' && wParam <= '9')
		{
			window->m_Keys[wParam] = keyState;
			return;
		}

		if (wParam >= 'A' && wParam <= 'Z')
		{
			window->m_Keys[wParam] = keyState;
			return;
		}

		switch (wParam)
		{
		case VK_SPACE:
			window->m_Keys[RGS_KEY_SPACE] = keyState;
			break;
		case VK_SHIFT:
			window->m_Keys[RGS_KEY_LEFT_SHIFT] = keyState;
			break;
		default:
			break;
		}

	}
	LRESULT WindowsWindow::WndProC(const HWND hwnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam)
	{
		WindowsWindow* window = (WindowsWindow*)GetProp(hwnd, RGS_WINDOW_ENTRY_NAME);
		if (window == nullptr)
			return DefWindowProc(hwnd, msgID, wParam, lParam);
		switch (msgID)
		{
		case WM_DESTROY:
			window->m_Closed = true;
			return 0;
		case WM_KEYDOWN:
			KeyPressImp(window, wParam, RGS_PRESS);
			break;
		case WM_KEYUP:
			KeyPressImp(window, wParam, RGS_RELEASE);
			break;
		default:
			break;
		}
		return DefWindowProc(hwnd, msgID, wParam, lParam);
	}
	void WindowsWindow::PollInputEvents()
	{
		MSG message;
		while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
	WindowsWindow::WindowsWindow(const std::string title, const int width, const int height)
		:Window(title,width,height)
	{
		ASSERT((s_Inited), "未初始化");
		DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.bottom = (long)height;
		rect.right = (long)width;
		AdjustWindowRect(&rect, style, false);
		m_Handle = CreateWindow(RGS_WINDOW_CLASS_NAME, m_Title.c_str(), style,
			CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, GetModuleHandle(NULL), NULL);
		m_Closed = false;
		SetProp(m_Handle, RGS_WINDOW_ENTRY_NAME, this);
		HDC windowDC = GetDC(m_Handle);
		m_MemoryDC = CreateCompatibleDC(windowDC);

		BITMAPINFOHEADER biHeader = {};
		HBITMAP newBitmap;
		HBITMAP oldBitmap;

		biHeader.biSize = sizeof(BITMAPINFOHEADER);
		biHeader.biWidth = ((long)m_Width);
		biHeader.biHeight = -((long)m_height);
		biHeader.biPlanes = 1;
		biHeader.biBitCount = 24;
		biHeader.biCompression = BI_RGB;
		newBitmap = CreateDIBSection(m_MemoryDC, (BITMAPINFO*)&biHeader, DIB_RGB_COLORS, (void**)&m_Buffer, nullptr, 0);
		ASSERT(newBitmap != nullptr);
		constexpr uint32_t channelCount = 3;
		size_t size = m_Width * m_height* channelCount * sizeof(unsigned char);
		memset(m_Buffer, 0, size);
		oldBitmap = (HBITMAP)SelectObject(m_MemoryDC, newBitmap);

		DeleteObject(oldBitmap);
		ReleaseDC(m_Handle, windowDC);
		Show();
	}
	WindowsWindow::~WindowsWindow()
	{
		ShowWindow(m_Handle, SW_HIDE);
		RemoveProp(m_Handle, RGS_WINDOW_ENTRY_NAME);
		DeleteDC(m_MemoryDC);
		DestroyWindow(m_Handle);
	}
	void WindowsWindow::Show() const
	{
		HDC windowDC = GetDC(m_Handle);
		BitBlt(windowDC, 0, 0, (int)m_Width, (int)m_height, m_MemoryDC, 0, 0, SRCCOPY);
		ShowWindow(m_Handle, SW_SHOW);
		ReleaseDC(m_Handle, windowDC);

	}
	void WindowsWindow::DrawFramebuffer(const Framebuffer& framebuffer)
	{
		const int fWidth = framebuffer.GetWidth();
		const int fHeight = framebuffer.GetHeight();
		const int width = m_Width < fWidth ? m_Width : fWidth;
		const int height = m_height < fHeight ? m_height : fHeight;

		//std::cout << "fwidth" << fWidth << "fheight" << fHeight << "width" << width << "height" << height <<std::endl;
		for (int i = 0; i < height; i++)
		{
			for (int j= 0; j < width; j++)
			{
				//翻转RGB显示
				constexpr int channelCount = 3;
				constexpr int rChannel = 2;
				constexpr int gChannel = 1;
				constexpr int bChannel = 0;

				Vec3 color = framebuffer.GetColor(j, fHeight - 1 - i);
				const int pixStart = (i * m_Width + j) * channelCount;
				const int rIndex = pixStart + rChannel;
				const int gIndex = pixStart + gChannel;
				const int bIndex = pixStart + bChannel;
				
				m_Buffer[rIndex] = Float2UChar(color.X);
				m_Buffer[gIndex] = Float2UChar(color.Y);
				m_Buffer[bIndex] = Float2UChar(color.Z);
				//std::cout << "更新"<< m_Buffer[rIndex] << std::endl;
			}
		}
		//std::cout << "更新完毕" << std::endl;
		Show();
	}
}