class Texture
{
public:
	Texture( uint32_t inWidth, uint32_t inHeight, ID3D11ShaderResourceView* inShaderResourceView );
	~Texture();

	uint32_t	GetWidth()	const	{ return mWidth; }
	uint32_t	GetHeight()	const	{ return mHeight; }

	ID3D11ShaderResourceView*	GetShaderResourceView()	const	{ return mShaderResourceView; }

private:
	uint32_t	mWidth;			
	uint32_t	mHeight;			

	ID3D11ShaderResourceView* mShaderResourceView;
};


typedef shared_ptr< Texture >	TexturePtr;