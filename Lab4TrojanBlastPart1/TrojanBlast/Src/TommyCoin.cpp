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



void TommyCoin::Write( MemoryOutputStream& inStream ) const 
{
	if( inStream.GetRemainingBytes() >= 3 * sizeof( float ) + sizeof( XMVECTOR ) )
	{
		XMVECTORF32 location; location.v = GetLocation();
		inStream.Write( location.f[ 0 ] );
		inStream.Write( location.f[ 1 ] );

		inStream.Write( GetRotation() );
		inStream.Write( GetColor() );
	}
}

void TommyCoin::Read( MemoryInputStream& inStream )
{
	if( inStream.GetRemainingBytes() >= 3 * sizeof( float ) + sizeof( XMVECTOR )  )
	{
		XMVECTORF32 location = { 0 };
		inStream.Read( location.f[ 0 ] );
		inStream.Read( location.f[ 1 ] );
		SetLocation( location );

		float rotation;
		inStream.Read( rotation );
		SetRotation( rotation );

		XMVECTOR color;
		inStream.Read( color );
		SetColor( color );

	}
}

