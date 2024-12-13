#pragma once
#include<Windows.h>
#include<string>
#include "Window.h"
namespace RGS
{
	class WindowsWindow :public Window
	{
	public:
		WindowsWindow(const std::string title, const int width, const int height);
		~WindowsWindow();
		void Show()const override;
		virtual void DrawFramebuffer(const Framebuffer& framebuffer) override;
	public:
		static void Init();
		static void Terminate();
		static void PollInputEvents();
	private:
		static void Register();
		static void UnRegister();

		static void KeyPressImp(WindowsWindow* window, const WPARAM wParam, const char state);
		static LRESULT CALLBACK WndProC(const HWND hwnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam);

	private:
		HWND m_Handle;
		HDC m_MemoryDC;
		unsigned char* m_Buffer;

		static bool s_Inited;
	};
}

