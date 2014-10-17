class ShipClient : public Ship
{
public:
	static	GameObjectPtr	StaticCreate()		{ return GameObjectPtr( new ShipClient() ); }

	virtual void Update();

protected:
	ShipClient();

private:
	TexturePtr			mInactiveTexture;
	TexturePtr			mThrustingTexture;

	SpriteComponentPtr	mSpriteComponent;
};