class TommyCoinClient : public TommyCoin
{
public:
	static	GameObject*	StaticCreate()		{ return new TommyCoinClient(); }

protected:
	TommyCoinClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};