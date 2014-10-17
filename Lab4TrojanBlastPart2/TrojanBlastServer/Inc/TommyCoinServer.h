class TommyCoinServer : public TommyCoin
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn( new TommyCoinServer() ); }
	void HandleDying() override;

protected:
	TommyCoinServer();

};