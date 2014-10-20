#include <TrojanBlastServerPCH.h>

ShipServer::ShipServer()
{}

void ShipServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject( this );
}

void ShipServer::Update()
{
	XMVECTORF32 location; 
	location.v = GetLocation();
	float rotation = GetRotation();

	Ship::Update();
	
	float deltaTime = Timing::sInstance.GetDeltaTime();

	ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy( GetPlayerId() );
	if( client )
	{
		const InputState& currentState = client->GetInputState();

		ProcessInput( deltaTime, currentState );
	}

	SimulateMovement( deltaTime );

	XMVECTORF32 newLocation;
	newLocation.v = GetLocation();
	if (location.f[0] != newLocation.f[0] ||
		location.f[1] != newLocation.f[1]) {
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ESRS_Location);
	}

	if (rotation != GetRotation()) {
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ESRS_Rotation);
	}
}