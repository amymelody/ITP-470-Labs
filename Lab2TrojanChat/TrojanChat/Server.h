
class Server
{
public:
	void Run(USHORT inPort);

private:
	std::vector<ClientProxy*> clientProxies;
	fd_set readSet;
	fd_set writeSet;
};