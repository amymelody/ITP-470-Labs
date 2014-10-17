//I take care of rendering things!

namespace DirectX
{
	class SpriteBatch;
	class CommonStates;
}

using DirectX::SpriteBatch;

class RenderManager
{
public:

	static void StaticInit();
	static std::unique_ptr< RenderManager >	sInstance;

	void*	operator new( size_t inSize )	{ return _aligned_malloc( inSize, 16 ); }
	void	operator delete( void * inP )	{ _aligned_free( inP ); }


	void Render();
	void RenderComponents();

	SpriteBatch*	GetSpriteBatch()	{ return mSpriteBatch.get(); }

	//vert inefficient method of tracking scene graph...
	void AddComponent( SpriteComponent* inComponent );
	void RemoveComponent( SpriteComponent* inComponent );
	int	 GetComponentIndex( SpriteComponent* inComponent ) const;

private:

	RenderManager();

	unique_ptr< SpriteBatch >				mSpriteBatch;

	unique_ptr< CommonStates >				mStateObjects;

	//this can't be only place that holds on to component- it has to live inside a GameObject in the world
	vector< SpriteComponent* >					mComponents;

	uint32_t									mPadding1;
	uint32_t									mPadding2;

	XMVECTOR									mViewPoint;
	XMVECTOR									mHalfViewPortSize;
	XMVECTOR									mZoomScale;


};

