#include "Ship.h"

class ShipServer : public Ship
{
public:
	static	GameObject*	StaticCreate()		{ return new ShipServer(); }
	void Update() {};

protected:
	ShipServer() : Ship() {};
};

