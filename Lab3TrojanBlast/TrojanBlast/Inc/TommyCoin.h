class TommyCoin : public GameObject
{
public:
	static	GameObject*	StaticCreate() { return new TommyCoin(); }

	virtual uint32_t GetFourCCName() const override { return 'COIN'; }

	virtual bool	HandleCollisionWithShip( Ship* inShip ) override;

protected:
	TommyCoin();

	SpriteComponentPtr	mSpriteComponent;
};