#include <TrojanBlastClientPCH.h>
#include "ShipClient.h"
#include <SpriteBatch.h>
#include "TextureManager.h"
#include "Timing.h"
#include "InputManager.h"

ShipClient::ShipClient() : Ship()
{
	
	mInactiveTexture = TextureManager::sInstance->GetTexture("spaceship");
	mThrustingTexture = TextureManager::sInstance->GetTexture("spaceshipWithThrust");

	mSpriteComponent.reset(new SpriteComponent(this));
}


void ShipClient::ProcessInput(float inDeltaTime, const InputState& inInputState)
{
	//process our input....

	//turning...
	float newRotation = GetRotation() + inInputState.GetDesiredHorizontalDelta() * mMaxDrivenRotationSpeed * inDeltaTime;
	SetRotation(newRotation);

	//moving...
	float inputForwardDelta = inInputState.GetDesiredVerticalDelta();
	if (inputForwardDelta != 0.f)
	{
		XMVECTOR forwardVector = GetForwardVector();
		mVelocity += XMVectorScale(forwardVector, inInputState.GetDesiredVerticalDelta() * inDeltaTime * mMaxDrivenLinearAcceleration);
		mIsThrusting = true;
	}
	else
	{
		mIsThrusting = false;
	}
}

void ShipClient::Update()
{
	float deltaTime = Timing::sInstance.GetDeltaTime();

	if (mIsLocallyControlled)
	{
		const InputState& currentState = InputManager::sInstance->GetState();

		ProcessInput(deltaTime, currentState);
	}

	SimulateMovement(deltaTime);

	if (mIsThrusting)
	{
		mSpriteComponent->SetTexture(mThrustingTexture);
	}
	else
	{
		mSpriteComponent->SetTexture(mInactiveTexture);
	}

}