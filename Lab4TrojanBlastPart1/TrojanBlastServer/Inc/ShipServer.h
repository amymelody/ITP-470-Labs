class ShipServer : public Ship
{
	public:
	static	GameObject*	StaticCreate()		{ return new ShipServer(); }

	virtual void Update();


protected:
	ShipServer();

};