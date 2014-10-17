class ScoreBoardManager
{
public:

	static void StaticInit();
	static std::unique_ptr< ScoreBoardManager >	sInstance;

	class Entry
	{
	public:
		Entry() {};

		Entry( uint32_t inPlayerID, const wstring& inPlayerName, const XMVECTORF32& inColor );

		XMVECTORF32		GetColor()		const	{ return mColor; }
		uint32_t		GetPlayerId()	const	{ return mPlayerId; }
		const wstring&	GetPlayerName()	const	{ return mPlayerName; }
		const wstring&	GetFormattedNameScore()	const	{ return mFormattedNameScore; }
		int				GetScore()		const	{ return mScore; }

		void			SetScore( int inScore );

		bool			Write( OutgoingPacketBuffer& inPacket ) const;
		bool			Read( IncomingPacketBuffer& inPacket );
		static uint32_t	GetSerializedSize();
	private:
		XMVECTORF32		mColor;
		
		uint32_t		mPlayerId;
		wstring			mPlayerName;
		
		int				mScore;

		wstring			mFormattedNameScore;
	};

	Entry*	GetEntry( uint32_t inPlayerId );
	bool	RemoveEntry( uint32_t inPlayerId );
	void	AddEntry( uint32_t inPlayerId, const wstring& inPlayerName );
	void	IncScore( uint32_t inPlayerId, int inAmount );

	bool	Write( OutgoingPacketBuffer& inPacket ) const;
	bool	Read( IncomingPacketBuffer& inPacket );

	const vector< Entry >&	GetEntries()	const	{ return mEntries; }

private:

	ScoreBoardManager();

	vector< Entry >	mEntries;

	vector< XMVECTORF32 >	mDefaultColors;


};