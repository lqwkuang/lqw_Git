#include "WindowsWindow.h"
namespace RGS
{
	Window::Window(std::string title, const int width, const int height)
		:m_Title(title), m_Width(width), m_height(height) 
	{
		memset(m_Keys, RGS_RELEASE, RGS_KEY_MAX_COUNT);
	}
	Window::Window()
	{
		this->m_Width = 0;
		this->m_height = 0;
		memset(m_Keys, RGS_RELEASE, RGS_KEY_MAX_COUNT);
	}
	void Window::Init()
	{
		WindowsWindow::Init();
	}
	void Window::Terminate()
	{
		WindowsWindow::Terminate();
	}
	Window* Window::Create(const std::string tile, const int width, const int height)
	{
		return new WindowsWindow(tile, width, height);
	}
	void Window::PollInputEvents()
	{
		WindowsWindow::PollInputEvents();
	}
}