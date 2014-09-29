#include <TrojanBlastPCH.h>
#include "ScoreBoardManager.h"
#include <DirectXColors.h>
#include "StringUtils.h"

std::unique_ptr< ScoreBoardManager >	ScoreBoardManager::sInstance;


void ScoreBoardManager::StaticInit()
{
	sInstance.reset( new ScoreBoardManager() );
}

ScoreBoardManager::Entry::Entry( uint32_t inPlayerID, const wstring& inPlayerName, const XMVECTORF32& inColor ) :
mPlayerID( inPlayerID ),
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


ScoreBoardManager::Entry* ScoreBoardManager::GetEntry( uint32_t inPlayerID )
{
	for ( Entry &entry: mEntries )
	{
		if( entry.GetPlayerID() == inPlayerID )
		{
			return &entry;
		}
	}

	return nullptr;
}

ScoreBoardManager::ScoreBoardManager()
{
	mDefaultColors.push_back(DirectX::Colors::LightYellow);
	mDefaultColors.push_back(DirectX::Colors::LightBlue);
	mDefaultColors.push_back(DirectX::Colors::LightPink);
	mDefaultColors.push_back(DirectX::Colors::LightGreen);
}

bool ScoreBoardManager::Entry::Write(PacketBuffer* inPacketBuffer) const {
	uint32_t score = mScore;
	return inPacketBuffer->WriteInt(score);
}

bool ScoreBoardManager::Entry::Read(PacketBuffer* inPacketBuffer) {
	uint32_t score;
	if (inPacketBuffer->ReadInt(score)) {
		SetScore(score);
	}
	return false;
}

bool ScoreBoardManager::RemoveEntry( uint32_t inPlayerID )
{
	for( auto eIt = mEntries.begin(), endIt = mEntries.end(); eIt != endIt; ++eIt )
	{
		if( ( *eIt ).GetPlayerID() == inPlayerID )
		{
			mEntries.erase( eIt );
			return true;
		}
	}

	return false;
}

void ScoreBoardManager::AddEntry( uint32_t inPlayerID, const wstring& inPlayerName )
{
	//if this player id exists already, remove it first- it would be crazy to have two of the same id
	RemoveEntry( inPlayerID );
	
	mEntries.emplace_back( inPlayerID, inPlayerName, mDefaultColors[ inPlayerID % mDefaultColors.size() ] );
}

void ScoreBoardManager::IncScore( uint32_t inPlayerID, int inAmount )
{
	Entry* entry = GetEntry( inPlayerID );
	if( entry )
	{
		entry->SetScore( entry->GetScore() + inAmount );
	}
}



