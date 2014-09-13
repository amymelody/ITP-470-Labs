#include "Ship.h"
#include "InputState.h"
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
	void ProcessInput(float inDeltaTime, const InputState& inInputState);

	TexturePtr			mInactiveTexture;
	TexturePtr			mThrustingTexture;

	SpriteComponentPtr	mSpriteComponent;
};

