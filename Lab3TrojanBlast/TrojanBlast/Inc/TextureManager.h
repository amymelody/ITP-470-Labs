class TextureManager
{
public:
	static void StaticInit();

	static std::unique_ptr< TextureManager >		sInstance;

	TexturePtr	GetTexture( const string& inTextureName );

private:
	TextureManager();

	HRESULT CacheTexture( string inName, const wchar_t* inFileName );

	unordered_map< string, TexturePtr >	mNameToTextureMap;
};