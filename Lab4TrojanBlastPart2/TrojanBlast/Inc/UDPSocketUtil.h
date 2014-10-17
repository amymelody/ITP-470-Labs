class UDPSocketUtil
{
public:


	static UDPSocketPtr	CreateSocket( uint16_t inPort );
	static bool			StaticInit();
	static void			CleanUp();

	static void			ReportError( const wchar_t* inOperationDesc );
	static int			GetLastError();
};