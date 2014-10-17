#include <TrojanBlastServerPCH.h>

ShipServer::ShipServer()
{
	SetNetworkId( NetworkManagerServer::sInstance->GetNewNetworkId() );
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