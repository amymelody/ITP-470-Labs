
class InputManager
{
public:

	static void StaticInit();
	static unique_ptr< InputManager >	sInstance;

	void HandleInput( EInputAction inInputAction, int inKeyCode );

	const InputState& GetState()	const	{ return mCurrentState; }

private:

	InputState							mCurrentState;

	InputManager();


};