#include "TommyCoin.h"

class TommyCoinServer : public TommyCoin
{
public:
	static	GameObject*	StaticCreate() { return new TommyCoinServer(); }

protected:
	TommyCoinServer() : TommyCoin() {};
};

