class TCPSocketUtil
{
public:
	static shared_ptr<TCPSocket> CreateSocket(USHORT inPort);
	static int StartUp();
	static void CleanUp();
	static int GetLastError();
//	static void ReportError(const wchar_t* inOperationDesc);
};

