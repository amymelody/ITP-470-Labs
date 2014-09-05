class TCPSocketUtil
{
public:
	static shared_ptr<TCPSocket> CreateSocket(USHORT inPort);
	static int StartUp();
	static void CleanUp();
	static int GetLastError();
	static int Select(fd_set* readfds, fd_set* writefds, fd_set* exceptfds);
};

