#include <Windows.h>

class UDPSocket;

class Engine
{
public:

	static bool							StaticInit( HINSTANCE hInstance, int inCmdShow );
	static std::unique_ptr< Engine >	sInstance;
	virtual int Run();

protected:

	Engine();
	virtual void DoFrame();
	float timeElapsed;
	shared_ptr<UDPSocket> mSocket;

private:

			void	SetupWorld();
			int		DoRunLoop();



};