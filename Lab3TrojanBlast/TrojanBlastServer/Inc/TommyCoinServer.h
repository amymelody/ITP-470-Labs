#include "TommyCoin.h"

class TommyCoinServer : public TommyCoin
{
public:
	static	GameObject*	StaticCreate() { return new TommyCoinServer(); }

	virtual bool	HandleCollisionWithShip(Ship* inShip) override;

protected:
	TommyCoinServer() : TommyCoin() {};
};

