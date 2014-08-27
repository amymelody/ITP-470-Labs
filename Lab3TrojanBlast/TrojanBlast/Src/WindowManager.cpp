#include <TrojanBlastPCH.h>

std::unique_ptr< WindowManager >	WindowManager::sInstance;

#include "..\resource.h"

namespace
{
	//--------------------------------------------------------------------------------------
	// Called every time the application receives a message
	//--------------------------------------------------------------------------------------
	LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
	{
		PAINTSTRUCT ps;
		HDC hdc;

		switch( message )
		{
			case WM_PAINT:
				hdc = BeginPaint( hWnd, &ps );
				EndPaint( hWnd, &ps );
				break;

			case WM_KEYDOWN:
				{
					EInputAction action = ( lParam & 0x40000000 ) != 0 ? EIA_Repeat : EIA_Pressed;
					InputManager::sInstance->HandleInput( action, wParam );
				}
				break;

			case WM_KEYUP:
				InputManager::sInstance->HandleInput( EIA_Released, wParam );
				break;

			case WM_DESTROY:
				PostQuitMessage( 0 );
				break;

			default:
				return DefWindowProc( hWnd, message, wParam, lParam );
		}

		return 0;
	}
}


HRESULT WindowManager::StaticInit( HINSTANCE hInstance, int inCmdShow )
{
	// Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_SAMPLE1 );
    wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"MainWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_SAMPLE1 );
    if( RegisterClassEx( &wcex ) == 0 )
	{
		return E_FAIL;
	}

    // Create window
    RECT rc = { 0, 0, 1280, 720 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    HWND hwnd = CreateWindow( L"MainWindowClass", L"Trojan Blast", WS_OVERLAPPEDWINDOW,
                           20, 20, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
                           nullptr );
	if( !hwnd )
	{
		return E_FAIL;
	}

    ShowWindow( hwnd, inCmdShow );

	sInstance.reset( new WindowManager( hwnd ) );

	return S_OK;
}


WindowManager::WindowManager( HWND inMainWindow )
{
	mMainWindow = inMainWindow;	

}
