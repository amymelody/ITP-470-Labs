#include <TrojanBlastServerPCH.h>

ShipServer::ShipServer()
{}

void ShipServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}

void ShipServer::Update()
{
	Ship::Update();
	
	float deltaTime = Timing::sInstance.GetDeltaTime();

	ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy( GetPlayerId() );
	if( client )
	{
		const InputState& currentState = client->GetInputState();

		ProcessInput( deltaTime, currentState );
	}

	SimulateMovement( deltaTime );

	
}