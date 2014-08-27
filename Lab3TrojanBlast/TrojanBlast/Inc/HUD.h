//I take care of rendering things!

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

using DirectX::SpriteBatch;
using DirectX::SpriteFont;

class HUD
{
public:

	static void StaticInit();
	static std::unique_ptr< HUD >	sInstance;

	void Render();

	SpriteBatch*	GetSpriteBatch()	{ return mSpriteBatch.get(); }


private:

	HUD();

	ID3D11DeviceContext*						mDeviceContext;
	unique_ptr< SpriteBatch >					mSpriteBatch;
	unique_ptr< SpriteFont >					mFont;

	XMFLOAT2									mScoreBoardOrigin;
	XMFLOAT2									mScoreOffset;



};

