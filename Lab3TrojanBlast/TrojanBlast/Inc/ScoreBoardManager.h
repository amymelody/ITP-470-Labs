#include "PacketBuffer.h"

class ScoreBoardManager
{
public:

	static void StaticInit();
	static std::unique_ptr< ScoreBoardManager >	sInstance;

	class Entry
	{
	public:
		Entry(uint32_t inPlayerID, const wstring& inPlayerName, const XMVECTORF32& inColor);

		XMVECTORF32		GetColor()		const	{ return mColor; }
		uint32_t		GetPlayerID()	const	{ return mPlayerID; }
		const wstring&	GetPlayerName()	const	{ return mPlayerName; }
		const wstring&	GetFormattedNameScore()	const	{ return mFormattedNameScore; }
		int32_t			GetScore()		const	{ return mScore; }
		bool Write(PacketBuffer* inPacketBuffer) const;
		bool Read(PacketBuffer* inPacketBuffer);

		void			SetScore( int32_t inScore );
		void			SetUpdated(bool inUpdated);
		bool			updated;

	private:
		XMVECTORF32		mColor;
		uint32_t		mPlayerID;
		wstring			mPlayerName;
		
		int32_t			mScore;
		wstring			mFormattedNameScore;
	};

	Entry*	GetEntry( uint32_t inPlayerID );
	bool	RemoveEntry( uint32_t inPlayerID );
	void	AddEntry( uint32_t inPlayerID, const wstring& inPlayerName );
	void	IncScore( uint32_t inPlayerID, int inAmount );

	vector< Entry >&GetEntries()	{ return mEntries; }

private:

	ScoreBoardManager();

	vector< Entry >	mEntries;

	vector< XMVECTORF32 >	mDefaultColors;


};