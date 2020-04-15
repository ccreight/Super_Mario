#include "MoveComponent.h"
#include "Actor.h"
#include <iostream>

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
	
	Vector2 position = Vector2((float)(.5 * mOwner->GetForward().x * mForwardSpeed * (((double)deltaTime) * 5)), (float)(.5 * mOwner->GetForward().y * mForwardSpeed * (((double)deltaTime) * 5)));
	mOwner->SetPosition(mOwner->GetPosition() + position);

}
