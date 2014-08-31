class TCPSocketUtil
{
public:
	static TCPSocketPtr CreateSocket(uint16_t inPort);
	static int StartUp();
	static void CleanUp();
	static int GetLastError();
//	static void ReportError(const wchar_t* inOperationDesc);
};

