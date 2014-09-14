#include <Windows.h>

class Engine
{
public:

	static bool							StaticInit( HINSTANCE hInstance, int inCmdShow );
	static std::unique_ptr< Engine >	sInstance;

	int Run();

protected:

	Engine();
	virtual void	DoFrame();

private:

			void	SetupWorld();
			int		DoRunLoop();



};