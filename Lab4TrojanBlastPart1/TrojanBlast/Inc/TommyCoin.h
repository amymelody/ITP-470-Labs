class TommyCoin : public GameObject
{
public:
	static	GameObject*	StaticCreate() { return new TommyCoin(); }

	virtual uint32_t GetFourCCName() const override { return 'COIN'; }

	virtual bool	HandleCollisionWithShip( Ship* inShip ) override;

	virtual void		Write( MemoryOutputStream& inStream ) const override;
	virtual void		Read( MemoryInputStream& inStream ) override;

protected:
	TommyCoin();

};