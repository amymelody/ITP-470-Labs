#include <TrojanBlastClientPCH.h>

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <DirectXColors.h>

using namespace DirectX;

std::unique_ptr< HUD >	HUD::sInstance;


HUD::HUD() :
mScoreBoardOrigin( 50.f, 60.f ),
mBandwidthOrigin( 50.f, 10.f ),
mScoreOffset( 0.f, 50.f )
{
    mSpriteBatch.reset( new SpriteBatch( GraphicsDriver::sInstance->GetDeviceContext() ) );

	mFont.reset( new SpriteFont( GraphicsDriver::sInstance->GetDevice(), L"../Assets/italic.spritefont" ) );
}


void HUD::StaticInit()
{
	sInstance.reset( new HUD() );
}

void HUD::Render(  )
{
	mSpriteBatch->Begin( SpriteSortMode_Deferred );
	
	RenderBandWidth();
	RenderScoreBoard();

	mSpriteBatch->End();

}

void HUD::RenderBandWidth()
{
	wstring bandwidth = StringUtils::Sprintf( L"In: %d  Bps, Out: %d Bps",
												static_cast< int >( NetworkManagerClient::sInstance->GetBytesReceivedPerSecond().GetValue() ),
												static_cast< int >( NetworkManagerClient::sInstance->GetBytesSentPerSecond().GetValue() ) );
	mFont->DrawString( mSpriteBatch.get(), bandwidth.c_str(), mBandwidthOrigin, Colors::White );
}

void HUD::RenderScoreBoard()
{
	const vector< ScoreBoardManager::Entry >& entries = ScoreBoardManager::sInstance->GetEntries();
	XMFLOAT2 offset = mScoreBoardOrigin;

	for( const auto& entry: entries )
	{
		mFont->DrawString( mSpriteBatch.get(), entry.GetFormattedNameScore().c_str(), offset, entry.GetColor() );
		offset.x += mScoreOffset.x;
		offset.y += mScoreOffset.y;
	}

}
