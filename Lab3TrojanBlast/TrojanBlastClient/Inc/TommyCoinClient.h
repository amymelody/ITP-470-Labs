#include "TommyCoin.h"
#include "SpriteComponent.h"

class TommyCoinClient : public TommyCoin
{
public:
	static	GameObject*	StaticCreate() { return new TommyCoinClient(); }

protected:
	TommyCoinClient();

	SpriteComponentPtr	mSpriteComponent;
};

