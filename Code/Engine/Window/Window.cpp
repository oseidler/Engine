#include "Engine/Window/Window.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/backends/imgui_impl_win32.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#pragma comment(lib, "Comdlg32.lib")


//static variable initialization
Window* Window::s_mainWindow = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//
//this big thing
//
LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(windowHandle, wmMessageCode, wParam, lParam))
		return true;
	
	Window* windowContext = Window::GetWindowContext();
	GUARANTEE_OR_DIE(windowContext != nullptr, "WindowContext was null!");
	InputSystem* input = windowContext->GetConfig().m_inputSystem;
	GUARANTEE_OR_DIE(input != nullptr, "Input system was null!");

	switch (wmMessageCode)
	{
		// App close requested via "X" button, or right-click "Close Window" on task bar, or "Close" from system menu, or Alt-F4
		case WM_CLOSE:
		{
			FireEvent("quit");
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			unsigned char keyCode = KEYCODE_LMB;
			bool wasConsumed = false;
			if (input != nullptr)
			{
				wasConsumed = input->HandleKeyPressed(keyCode);
				return 0;
			}
			
			break;
		}

		case WM_LBUTTONUP:
		{
			unsigned char keyCode = KEYCODE_LMB;
			bool wasConsumed = false;
			if (input != nullptr)
			{
				wasConsumed = input->HandleKeyReleased(keyCode);
				return 0;
			}

			break;
		}

		case WM_RBUTTONDOWN:
		{
			unsigned char keyCode = KEYCODE_RMB;
			bool wasConsumed = false;
			if (input != nullptr)
			{
				wasConsumed = input->HandleKeyPressed(keyCode);
				return 0;
			}

			break;
		}

		case WM_RBUTTONUP:
		{
			unsigned char keyCode = KEYCODE_RMB;
			bool wasConsumed = false;
			if (input != nullptr)
			{
				wasConsumed = input->HandleKeyReleased(keyCode);
				return 0;
			}

			break;
		}

		// Raw physical keyboard "key-was-just-depressed" event (case-insensitive, not translated)
		case WM_KEYDOWN:
		{
			EventArgs args;
			args.SetValue("KeyCode", Stringf("%d", (unsigned char)wParam));
			FireEvent("KeyPressed", args);
			return 0;
			
			/*unsigned char keyCode = (unsigned char)wParam;
			bool wasConsumed = false;
			if (input != nullptr)
			{
				wasConsumed = input->HandleKeyPressed(keyCode);
				return 0;
			}

			break;*/
		}

		// Raw physical keyboard "key-was-just-released" event (case-insensitive, not translated)
		case WM_KEYUP:
		{
			EventArgs args;
			args.SetValue("KeyCode", Stringf("%d", (unsigned char)wParam));
			FireEvent("KeyReleased", args);
			return 0;
			
			/*unsigned char keyCode = (unsigned char)wParam;
			bool wasConsumed = false;
			if (input != nullptr)
			{
				wasConsumed = input->HandleKeyReleased(keyCode);
				return 0;
			}

			break;*/
		}

		case WM_CHAR:
		{
			EventArgs args;
			args.SetValue("CharCode", Stringf("%d", (unsigned char)wParam));
			FireEvent("CharInput", args);
			return 0;
		}
	}

	// Send back to Windows any unhandled/unconsumed messages we want other apps to see (e.g. play/pause in music apps, etc.)
	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}


//
//constructor
//
Window::Window(WindowConfig const& config)
	:m_config(config)
{
	s_mainWindow = this;
}


//
//public game flow functions
//
void Window::Startup()
{
	CreateOSWindow();
}


void Window::BeginFrame()
{
	RunMessagePump();
}


void Window::EndFrame()
{
}


void Window::Shutdown()
{
}


//
//accessors
//
Vec2 Window::GetCursorNormalizedPos() const
{
	InputSystem* input = Window::GetWindowContext()->GetConfig().m_inputSystem;

	if (input != nullptr)
	{
		return input->GetCursorNormalizedPosition();
	}

	return Vec2();
}


bool Window::HasFocus() const
{
	HWND focusWindow = GetFocus();
	if (focusWindow == m_hWnd)
	{
		return true;
	}

	return false;
}


std::string Window::GetXMLFileNameFromDialog(unsigned char keyRelease)
{
	//get current directory
	TCHAR currentDirectory[MAX_PATH];
	LPSTR currentDirectoryLP = (LPSTR)currentDirectory;
	DWORD gotDirectory = GetCurrentDirectoryA(MAX_PATH, currentDirectoryLP);
	if (!gotDirectory)
	{
		ERROR_AND_DIE("Couldn't get current directory!");
	}

	//set mouse mode to visible
	Window* windowContext = Window::GetWindowContext();
	GUARANTEE_OR_DIE(windowContext != nullptr, "WindowContext was null!");
	InputSystem* input = windowContext->GetConfig().m_inputSystem;
	GUARANTEE_OR_DIE(input != nullptr, "Input system was null!");
	while (ShowCursor(true) < 0);
	
	//open file dialog
	char fileNameBuffer[MAX_PATH];

	OPENFILENAMEA fileStruct = { 0 };
	fileStruct.lStructSize = sizeof(fileStruct);
	fileStruct.hwndOwner = (HWND)m_hWnd;
	fileStruct.lpstrFile = fileNameBuffer;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	fileStruct.lpstrFile[0] = '\0';
	fileStruct.nMaxFile = MAX_PATH;
	fileStruct.lpstrFilter = "XML Files\0*.XML\0";
	fileStruct.nFilterIndex = 1;
	fileStruct.lpstrFileTitle = NULL;
	fileStruct.nMaxFileTitle = 0;
	fileStruct.lpstrInitialDir = "Data\\Models";
	fileStruct.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	LPOPENFILENAMEA fileStructPointer = &fileStruct;
	bool gotFile = GetOpenFileNameA(fileStructPointer);

	//set directory back
	bool setDirectory = SetCurrentDirectoryA(currentDirectoryLP);
	if (!setDirectory)
	{
		ERROR_AND_DIE("Couldn't set current directory!");
	}

	//fire key released event
	EventArgs args;
	args.SetValue("KeyCode", Stringf("%d", keyRelease));
	FireEvent("KeyReleased", args);

	std::string fileName;

	//test code just to see if file name was gotten successfully
	if (gotFile && fileStructPointer != nullptr)
	{
		fileName = fileStructPointer->lpstrFile;
		//ERROR_RECOVERABLE(fileName);
	}
	/*else
	{
		ERROR_RECOVERABLE("No file name gotten!");
	}*/

	input->SetCursorMode(true, true);

	return fileName;
}


//
//protected window utilities
//
void Window::CreateOSWindow()
{
	// Define a window style/class
	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>(WindowsMessageHandlingProcedure); // Register our Windows message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	const DWORD fullscreenStyleFlags = WS_POPUP;

	// Get desktop rect, dimensions, aspect
	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);
	float desktopWidth = (float)(desktopRect.right - desktopRect.left);
	float desktopHeight = (float)(desktopRect.bottom - desktopRect.top);
	float desktopAspect = desktopWidth / desktopHeight;

	if (m_config.m_isFullscreen)
	{
		m_config.m_clientAspect = desktopAspect;
		m_config.m_clientFraction = 1.0f;
	}

	// Calculate maximum client size (as some % of desktop size)
	float maxClientFractionOfDesktop = m_config.m_clientFraction;
	float clientWidth = desktopWidth * maxClientFractionOfDesktop;
	float clientHeight = desktopHeight * maxClientFractionOfDesktop;
	if (m_config.m_clientAspect > desktopAspect)
	{
		// Client window has a wider aspect than desktop; shrink client height to match its width
		clientHeight = clientWidth / m_config.m_clientAspect;
	}
	else
	{
		// Client window has a taller aspect than desktop; shrink client width to match its height
		clientWidth = clientHeight * m_config.m_clientAspect;
	}

	// Calculate client rect bounds by centering the client area
	float clientMarginX = 0.5f * (desktopWidth - clientWidth);
	float clientMarginY = 0.5f * (desktopHeight - clientHeight);
	RECT clientRect;

	//if a window size is provided, override the aspect ratio calculations and use window size instead
	if (m_config.m_windowSize != IntVec2())
	{
		clientRect.left = m_config.m_windowPosition.x;
		clientRect.right = clientRect.left + m_config.m_windowSize.x;
		clientRect.top = m_config.m_windowPosition.y;
		clientRect.bottom = clientRect.top + m_config.m_windowSize.y;
		clientWidth = static_cast<float>(m_config.m_windowSize.x);
		clientHeight = static_cast<float>(m_config.m_windowSize.y);
	}
	else
	{
		clientRect.left = (int)clientMarginX + m_config.m_windowPosition.x;
		clientRect.right = clientRect.left + (int)clientWidth;
		clientRect.top = (int)clientMarginY + m_config.m_windowPosition.y;
		clientRect.bottom = clientRect.top + (int)clientHeight;
	}

	// Calculate the outer dimensions of the physical window, including frame et. al.
	if (m_config.m_isFullscreen)
	{
		RECT windowRect = clientRect;
		AdjustWindowRectEx(&windowRect, fullscreenStyleFlags, FALSE, windowStyleExFlags);

		WCHAR windowTitle[1024];
		MultiByteToWideChar(GetACP(), 0, m_config.m_windowTitle.c_str(), -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
		HWND g_hWnd = CreateWindowEx(
			windowStyleExFlags,
			windowClassDescription.lpszClassName,
			windowTitle,
			fullscreenStyleFlags,
			windowRect.left,
			windowRect.top,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL,
			NULL,
			//(HINSTANCE)applicationInstanceHandle,
			(HINSTANCE)NULL,
			NULL);

		ShowWindow(g_hWnd, SW_SHOW);
		SetForegroundWindow(g_hWnd);
		SetFocus(g_hWnd);

		//g_displayDeviceContext = GetDC(g_hWnd);

		HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(cursor);

		//set member variables
		m_hWnd = g_hWnd;
		m_clientDimensions = IntVec2(static_cast<int>(clientWidth), static_cast<int>(clientHeight));
	}
	else
	{
		RECT windowRect = clientRect;
		AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

		WCHAR windowTitle[1024];
		MultiByteToWideChar(GetACP(), 0, m_config.m_windowTitle.c_str(), -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
		HWND g_hWnd = CreateWindowEx(
			windowStyleExFlags,
			windowClassDescription.lpszClassName,
			windowTitle,
			windowStyleFlags,
			windowRect.left,
			windowRect.top,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL,
			NULL,
			//(HINSTANCE)applicationInstanceHandle,
			(HINSTANCE)NULL,
			NULL);

		ShowWindow(g_hWnd, SW_SHOW);
		SetForegroundWindow(g_hWnd);
		SetFocus(g_hWnd);

		//g_displayDeviceContext = GetDC(g_hWnd);

		HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
		SetCursor(cursor);

		//set member variables
		m_hWnd = g_hWnd;
		m_clientDimensions = IntVec2(static_cast<int>(clientWidth), static_cast<int>(clientHeight));
	}
}


void Window::RunMessagePump()
{
	MSG queuedMessage;
	for (;; )
	{
		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage); // This tells Windows to call our "WindowsMessageHandlingProcedure" (a.k.a. "WinProc") function
	}
}
