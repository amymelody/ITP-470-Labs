
class GraphicsDriver
{
public:

	static HRESULT StaticInit( HWND inHWnd );

	static std::unique_ptr< GraphicsDriver >		sInstance;

	ID3D11DeviceContext*	GetDeviceContext()		const	{ return mDeviceContext; }
	ID3D11Device*			GetDevice()				const	{ return mDevice; }
	ID3D11RenderTargetView*	GetRenderTargetView()	const	{ return mRenderTargetView; }
	ID3D11DepthStencilView*	GetDepthStencilView()	const	{ return mDepthStencilView; }

	void					Present();

	~GraphicsDriver();

private:

	GraphicsDriver();
	HRESULT Init( HWND inHWnd );

	ID3D11DeviceContext*				mDeviceContext;
	ID3D11Device*						mDevice;

	ID3D11RenderTargetView*             mRenderTargetView;
	ID3D11DepthStencilView*             mDepthStencilView;

};
