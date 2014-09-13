#include <Windows.h>

class Engine
{
public:

	static bool							StaticInit( HINSTANCE hInstance, int inCmdShow );
	static std::unique_ptr< Engine >	sInstance;

	int Run();

protected:

	Engine();

private:

	virtual void	DoFrame();
			void	SetupWorld();
			int		DoRunLoop();



};