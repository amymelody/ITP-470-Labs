#include <TrojanBlastClientPCH.h>
#include "HUD.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <DirectXColors.h>
#include "GraphicsDriver.h"
#include "ScoreBoardManager.h"

using namespace DirectX;

std::unique_ptr< HUD >	HUD::sInstance;


HUD::HUD() :
mScoreBoardOrigin( 50.f, 10.f ),
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
	
	const vector< ScoreBoardManager::Entry >& entries = ScoreBoardManager::sInstance->GetEntries();
	XMFLOAT2 offset = mScoreBoardOrigin;

	for( const auto& entry: entries )
	{
		mFont->DrawString( mSpriteBatch.get(), entry.GetFormattedNameScore().c_str(), offset, entry.GetColor() );
		offset.x += mScoreOffset.x;
		offset.y += mScoreOffset.y;
	}


	mSpriteBatch->End();


	
}
