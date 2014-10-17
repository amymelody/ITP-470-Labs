#include <TrojanBlastPCH.h>
#include <SpriteBatch.h>

TommyCoin::TommyCoin()
{
	SetScale( GetScale() * 0.5f );
	SetCollisionRadius( 0.25f );
}


bool TommyCoin::HandleCollisionWithShip( Ship* inShip )
{
	//kill yourself!
	SetDoesWantToDie( true );

	ScoreBoardManager::sInstance->IncScore( inShip->GetPlayerId(), 1 );

	return false;
}



uint32_t TommyCoin::Write( OutgoingPacketBuffer& inPacket, uint32_t inDirtyState ) const 
{
	//lab4 part 2...change this to only write the dirty state
	uint32_t writtenState = 0;

	uint32_t remainingBytes = inPacket.GetRemainingBytes();

	if( remainingBytes >= 3 * sizeof( float ) + sizeof( XMVECTOR ) )
	{
		XMVECTORF32 location; location.v = GetLocation();
		inPacket.Write( location.f[ 0 ] );
		inPacket.Write( location.f[ 1 ] );

		inPacket.Write( GetRotation() );
		inPacket.Write( GetColor() );

		writtenState |= inDirtyState;
	}

	return writtenState;
}

void TommyCoin::Read( IncomingPacketBuffer& inPacket )
{
	//lab4 part 2...change this to match the new Write
	XMVECTORF32 location = { 0 };
	inPacket.Read( location.f[ 0 ] );
	inPacket.Read( location.f[ 1 ] );
	SetLocation( location );

	float rotation;
	inPacket.Read( rotation );
	SetRotation( rotation );

	XMVECTOR color;
	inPacket.Read( color );
	SetColor( color );
}

