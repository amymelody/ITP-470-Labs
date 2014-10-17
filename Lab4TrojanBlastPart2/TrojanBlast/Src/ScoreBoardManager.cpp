#include "TrojanBlastPCH.h"
#include <DirectXColors.h>

std::unique_ptr< ScoreBoardManager >	ScoreBoardManager::sInstance;


void ScoreBoardManager::StaticInit()
{
	sInstance.reset( new ScoreBoardManager() );
}

ScoreBoardManager::ScoreBoardManager()
{
	mDefaultColors.push_back( DirectX::Colors::LightYellow );
	mDefaultColors.push_back( DirectX::Colors::LightBlue );
	mDefaultColors.push_back( DirectX::Colors::LightPink );
	mDefaultColors.push_back( DirectX::Colors::LightGreen );
}

ScoreBoardManager::Entry::Entry( uint32_t inPlayerId, const wstring& inPlayerName, const XMVECTORF32& inColor ) :
mPlayerId( inPlayerId ),
mPlayerName( inPlayerName ),
mColor( inColor )
{
	SetScore( 0 );
}

void ScoreBoardManager::Entry::SetScore( int32_t inScore )
{
	mScore = inScore;

	wchar_t	buffer[ 256 ];
	_snwprintf_s( buffer, 256, _TRUNCATE, L"%ls: %i", mPlayerName.c_str(), mScore );
	mFormattedNameScore = wstring( buffer );

}


ScoreBoardManager::Entry* ScoreBoardManager::GetEntry( uint32_t inPlayerId )
{
	for ( Entry &entry: mEntries )
	{
		if( entry.GetPlayerId() == inPlayerId )
		{
			return &entry;
		}
	}

	return nullptr;
}

bool ScoreBoardManager::RemoveEntry( uint32_t inPlayerId )
{
	for( auto eIt = mEntries.begin(), endIt = mEntries.end(); eIt != endIt; ++eIt )
	{
		if( ( *eIt ).GetPlayerId() == inPlayerId )
		{
			mEntries.erase( eIt );
			return true;
		}
	}

	return false;
}

void ScoreBoardManager::AddEntry( uint32_t inPlayerId, const wstring& inPlayerName )
{
	//if this player id exists already, remove it first- it would be crazy to have two of the same id
	RemoveEntry( inPlayerId );
	
	mEntries.emplace_back( inPlayerId, inPlayerName, mDefaultColors[ inPlayerId % mDefaultColors.size() ] );
}

void ScoreBoardManager::IncScore( uint32_t inPlayerId, int inAmount )
{
	Entry* entry = GetEntry( inPlayerId );
	if( entry )
	{
		entry->SetScore( entry->GetScore() + inAmount );
	}
}



bool ScoreBoardManager::Write( OutgoingPacketBuffer& inPacket ) const
{
	bool didSucceed = false;
	int entryCount = mEntries.size();
	
	//we don't know our player names, so it's hard to check for remaining space in the packet...
	//not really a concern now though
	if( inPacket.Write( entryCount ) )
	{
		didSucceed = true;
		for( const Entry& entry: mEntries )
		{
			didSucceed = didSucceed && entry.Write( inPacket );
		}
	}
	
	return didSucceed;
}



bool ScoreBoardManager::Read( IncomingPacketBuffer& inPacket )
{
	bool didSucceed = false;
	int entryCount;
	if( inPacket.Read( entryCount ) )
	{
		didSucceed = true;
		//just replace everything that's here, it don't matter...
		mEntries.resize( entryCount );
		for( Entry& entry: mEntries )
		{
			didSucceed = didSucceed && entry.Read( inPacket );
		}
	}

	return didSucceed;
}


bool ScoreBoardManager::Entry::Write( OutgoingPacketBuffer& inPacket ) const
{
	bool didSucceed = true;

	didSucceed = didSucceed & inPacket.Write( mColor );
	didSucceed = didSucceed & inPacket.Write( mPlayerId );
	didSucceed = didSucceed & inPacket.Write( mPlayerName );
	didSucceed = didSucceed & inPacket.Write( mScore );

	return didSucceed;
}

bool ScoreBoardManager::Entry::Read( IncomingPacketBuffer& inPacket )
{
	bool didSucceed = true;

	didSucceed = didSucceed & inPacket.Read( mColor );
	didSucceed = didSucceed & inPacket.Read( mPlayerId );

	didSucceed = didSucceed & inPacket.Read( mPlayerName );

	int score;
	didSucceed = didSucceed & inPacket.Read( score );
	if( didSucceed )
	{
		SetScore( score );
	}


	return didSucceed;
}



