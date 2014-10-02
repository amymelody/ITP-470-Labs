class Client : public Engine
{
public:

	static bool StaticInit( HINSTANCE hInstance, int inCmdShow );

protected:

	Client();

	virtual void	DoFrame() override;

private:

};