class Server : public Engine
{
public:

	static bool StaticInit();

	virtual void DoFrame() override;

	virtual int Run();

	void HandleNewClient( ClientProxyPtr inClientProxy );
	void HandleLostClient( ClientProxyPtr inClientProxy );

	ShipPtr	GetShipForPlayer( int inPlayerId );

private:
	Server();

	bool	InitNetworkManager();
	void	SetupWorld();

};