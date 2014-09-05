
class ClientProxy
{
public:
	ClientProxy(shared_ptr<TCPSocket> socket) : clientSocket(socket) {};
	~ClientProxy();
	shared_ptr<TCPSocket> GetTCPSocket();

private:
	shared_ptr<TCPSocket> clientSocket;
};

