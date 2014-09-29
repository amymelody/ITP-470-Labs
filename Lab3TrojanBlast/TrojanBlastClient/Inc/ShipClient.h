#include "Ship.h"
#include "Texture.h"
#include "SpriteComponent.h"

class ShipClient : public Ship
{
public:
	static	GameObject*	StaticCreate()		{ return new ShipClient(); }
	void Update();

protected:
	ShipClient();

private:

	TexturePtr			mInactiveTexture;
	TexturePtr			mThrustingTexture;

	SpriteComponentPtr	mSpriteComponent;
};

