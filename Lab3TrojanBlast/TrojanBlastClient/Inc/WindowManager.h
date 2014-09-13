
class WindowManager
{

public:
	
	static HRESULT StaticInit( HINSTANCE hInstance, int inCmdShow );
	static std::unique_ptr< WindowManager >	sInstance;

	HWND	GetMainWindow()	const	{ return mMainWindow; }

private:
	WindowManager( HWND inMainWindow);

	HWND mMainWindow;
};