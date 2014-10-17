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
	//if (currentState.GetDesiredHorizontalDelta() != 0 ||
	//	currentState.GetDesiredVerticalDelta() != 0) {
	//	LOG(L"Right: %f", currentState.GetDesiredHorizontalDelta());
	//	LOG(L"Forward: %f", currentState.GetDesiredVerticalDelta());
	//}

		ProcessInput( deltaTime, currentState );
	}

	SimulateMovement( deltaTime );

}