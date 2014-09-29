#include "Ship.h"

class ShipServer : public Ship
{
public:
	static	GameObject*	StaticCreate()		{ return new ShipServer(); }
	void Update();
	void SimulateMovement(float inDeltaTime);
	void ProcessCollisions();
	void ProcessCollisionsWithScreenWalls();

protected:
	ShipServer() : Ship() {};
	InputState* GetInputState(int playerID);
	void ProcessInput(float inDeltaTime, const InputState& inInputState);
};

