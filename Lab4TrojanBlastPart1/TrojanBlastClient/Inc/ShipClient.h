class ShipClient : public Ship
{
public:
	static	GameObject*	StaticCreate()		{ return new ShipClient(); }

	virtual void Update();

protected:
	ShipClient();

private:
	TexturePtr			mInactiveTexture;
	TexturePtr			mThrustingTexture;

	SpriteComponentPtr	mSpriteComponent;
};