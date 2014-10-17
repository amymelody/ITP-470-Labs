class TommyCoinClient : public TommyCoin
{
public:
	static	GameObjectPtr	StaticCreate()		{ return GameObjectPtr( new TommyCoinClient() ); }

protected:
	TommyCoinClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};