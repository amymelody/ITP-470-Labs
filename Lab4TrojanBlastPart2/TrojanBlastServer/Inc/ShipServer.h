class ShipServer : public Ship
{
	public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new ShipServer() ); }
	virtual void HandleDying() override;

	virtual void Update();


protected:
	ShipServer();

};