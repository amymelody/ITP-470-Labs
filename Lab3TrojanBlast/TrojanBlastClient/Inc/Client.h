#include "Engine.h"

class Client : public Engine
{
public:
	static bool StaticInit( HINSTANCE hInstance, int inCmdShow );

private:
	void DoFrame();
};