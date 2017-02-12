#include "stdafx.h"

using namespace OsirisI::Events;
using namespace OsirisI::Resources;
using namespace OsirisI::Utilities;
using namespace OsirisI::Graphics::Actors;

MovingActor::MovingActor(OsirisI::Utilities::HashedString name) : IActor(name) {
	this->currentMovmentSpeedInMeterPerSecondParted = OVector3(0.f, 0.f, 0.f);
	this->currentRotationSpeedInGradPerSecond = OVector3(0.f, 0.f, 0.f);
	this->currentMovmentSpeedInMeterPerSecond = 0.f;
	this->rotationAccelerationInGradPerSecond = 0.f;
	this->movementAccelerationInMeterPerSecond = 0.f;
	this->maxMovementSpeedInMeter = 0.f;
	this->maxRotationSpeedInGrad = 0.f;
	relativisticMode = false;

	keyAccelerate = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "keyAccelerate"));
	keyDeccelerate = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "keyDeccelerate"));
	rotateYawPos = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "rotateYawPos"));
	rotateYawNeg = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "rotateYawNeg"));
	rotatePitchPos = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "rotatePitchPos"));
	rotatePitchNeg = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "rotatePitchNeg"));
	rotateRollPos = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "rotateRollPos"));
	rotateRollNeg = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "rotateRollNeg"));
	moveForward = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "moveForward"));
	moveBackward = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "moveBackward"));
	strafeLeft = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "strafeLeft"));
	stafeRight = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "stafeRight"));
	moveUp = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "moveUp"));
	moveDown = (KeyboardKeys)Converter::StringToInt(EngineConfiguration::GetProperty("KeyMapping", "moveDown"));
}

MovingActor::~MovingActor() {}

ReturnState MovingActor::Initialize() {
	return OS_OK;
}

void MovingActor::DoPos(float delta) {
	this->currentMovmentSpeedInMeterPerSecondParted.x += (sin(this->rotation.y)*(sin(this->rotation.z) - cos(this->rotation.z)) * this->currentMovmentSpeedInMeterPerSecond);
	this->currentMovmentSpeedInMeterPerSecondParted.z += (cos(this->rotation.y)*(sin(this->rotation.z) - cos(this->rotation.z)) * this->currentMovmentSpeedInMeterPerSecond);
	this->currentMovmentSpeedInMeterPerSecondParted.y += (sin(this->rotation.z) * this->currentMovmentSpeedInMeterPerSecond);
	this->currentMovmentSpeedInMeterPerSecondParted.y -= (cos(this->rotation.z) * this->currentMovmentSpeedInMeterPerSecond);

	this->position.x += this->currentMovmentSpeedInMeterPerSecondParted.x * delta;
	this->position.y += this->currentMovmentSpeedInMeterPerSecondParted.y * delta;
	this->position.z += this->currentMovmentSpeedInMeterPerSecondParted.z * delta;
}

ReturnState MovingActor::Update(float delta) {
	while (events.size() > 0) {
		std::shared_ptr<IEvent> e = events.front();
		events.pop();

		if (HelperFunctions::IsTypeOf_SharedPtr<OsirisI_KeyboardInputEvent>(e)) {
			std::shared_ptr<OsirisI_KeyboardInputEvent> ek = std::dynamic_pointer_cast<OsirisI_KeyboardInputEvent>(e);

			if (ek->GetKey1() == KeyboardKeys::F5) {
				this->currentRotationSpeedInGradPerSecond = OVector3(0.f, 0.f, 0.f);
			}

			if (relativisticMode) { //If Non-Relativistic => Use MoveForward/-backward
				if (ek->GetKey1() == keyAccelerate) {
					if (this->maxMovementSpeedInMeter > (this->currentMovmentSpeedInMeterPerSecond + (this->currentMovmentSpeedInMeterPerSecond * delta)) || relativisticMode) {
						this->currentMovmentSpeedInMeterPerSecond += this->movementAccelerationInMeterPerSecond * delta;
					}
				}
				else if (ek->GetKey1() == keyDeccelerate) {
					if (this->maxMovementSpeedInMeter > (-1)* (this->currentMovmentSpeedInMeterPerSecond - (this->currentMovmentSpeedInMeterPerSecond * delta)) || relativisticMode) {
						this->currentMovmentSpeedInMeterPerSecond -= this->movementAccelerationInMeterPerSecond * delta;
					}
				}
				DoPos(delta);
			}
			else {
				if (ek->GetKey1() == moveForward) {
					if (this->maxMovementSpeedInMeter > (this->currentMovmentSpeedInMeterPerSecond + (this->currentMovmentSpeedInMeterPerSecond * delta)) || relativisticMode) {
						this->currentMovmentSpeedInMeterPerSecond += this->movementAccelerationInMeterPerSecond * delta;
					}
				}
				else if (ek->GetKey1() == moveBackward) {
					if (this->maxMovementSpeedInMeter > -1*((this->currentMovmentSpeedInMeterPerSecond + (this->currentMovmentSpeedInMeterPerSecond * delta))) || relativisticMode) {
						this->currentMovmentSpeedInMeterPerSecond -= this->movementAccelerationInMeterPerSecond * delta;
					}
				}
				DoPos(delta);
			}



			if (ek->GetKey1() == rotateYawPos) {
				if (this->maxRotationSpeedInGrad > (this->currentRotationSpeedInGradPerSecond.y + (this->rotationAccelerationInGradPerSecond * delta)) || relativisticMode) {
					this->currentRotationSpeedInGradPerSecond.y += (this->rotationAccelerationInGradPerSecond * delta);
				}
			}
			else if (ek->GetKey1() == rotateYawNeg) {
				if (this->maxRotationSpeedInGrad > (this->currentRotationSpeedInGradPerSecond.y + (this->rotationAccelerationInGradPerSecond * delta)) || relativisticMode) {
					this->currentRotationSpeedInGradPerSecond.y -= (this->rotationAccelerationInGradPerSecond * delta);
				}
			}
			else if (ek->GetKey1() == rotatePitchPos) {
				if (this->maxRotationSpeedInGrad > (this->currentRotationSpeedInGradPerSecond.x + (this->rotationAccelerationInGradPerSecond * delta)) || relativisticMode) {
					this->currentRotationSpeedInGradPerSecond.x += (this->rotationAccelerationInGradPerSecond * delta);
				}
			}
			else if (ek->GetKey1() == rotatePitchNeg) {
				if (this->maxRotationSpeedInGrad > (this->currentRotationSpeedInGradPerSecond.x + (this->rotationAccelerationInGradPerSecond * delta)) || relativisticMode) {
					this->currentRotationSpeedInGradPerSecond.x -= (this->rotationAccelerationInGradPerSecond * delta);
				}
			}
			else if (ek->GetKey1() == rotateRollPos) {
				if (this->maxRotationSpeedInGrad > (this->currentRotationSpeedInGradPerSecond.z + (this->rotationAccelerationInGradPerSecond * delta)) || relativisticMode) {
					this->currentRotationSpeedInGradPerSecond.z += (this->rotationAccelerationInGradPerSecond * delta);
				}
			}
			else if (ek->GetKey1() == rotateRollNeg) {
				if (this->maxRotationSpeedInGrad > (this->currentRotationSpeedInGradPerSecond.z + (this->rotationAccelerationInGradPerSecond * delta)) || relativisticMode) {
					this->currentRotationSpeedInGradPerSecond.z -= (this->rotationAccelerationInGradPerSecond * delta);
				}
			}


			else if (ek->GetKey1() == strafeLeft) {}
			else if (ek->GetKey1() == stafeRight) {}
			else if (ek->GetKey1() == moveUp) {}
			else if (ek->GetKey1() == moveDown) {}
		}
		else if (HelperFunctions::IsTypeOf_SharedPtr<OsirisI_MouseInputEvent>(e)) {
			return OS_OK;
		}
		else {
			LOG_DEBUG("UNKNOWN EVENT");
		}
	}



	this->rotation.x = this->rotation.x + this->currentRotationSpeedInGradPerSecond.x * delta;
	this->rotation.y = this->rotation.y + this->currentRotationSpeedInGradPerSecond.y * delta;
	this->rotation.z = this->rotation.z + this->currentRotationSpeedInGradPerSecond.z * delta;

	this->rotation.x = ((this->rotation.x > 360) ? this->rotation.x - 360 : this->rotation.x);
	this->rotation.y = ((this->rotation.y > 360) ? this->rotation.y - 360 : this->rotation.y);
	this->rotation.z = ((this->rotation.z > 360) ? this->rotation.z - 360 : this->rotation.z);

	/*
	int winkelGes = abs(this->rotation.x) + abs(this->rotation.y) + abs(this->rotation.z);
	float partialSpeed = this->currentMovmentSpeedInMeterPerSecond / winkelGes;

	this->position.x += partialSpeed * this->rotation.x;
	this->position.y += partialSpeed * this->rotation.y;
	this->position.z += partialSpeed * this->rotation.z;

	this->rotation.x = this->rotation.x + this->currentRotationSpeedInGradPerSecond.x;
	this->rotation.y = this->rotation.y + this->currentRotationSpeedInGradPerSecond.y;
	this->rotation.z = this->rotation.z + this->currentRotationSpeedInGradPerSecond.z;

	D3DXVECTOR3 dxa = D3DXVECTOR3((float*)&this->rotation);
	D3DXVec3Normalize(&dxa, &dxa);
	this->position += OVector3(dxa.x, dxa.y, dxa.z) * 0.0001f;
	*/
	return OS_OK;
}


ReturnState MovingActor::Recover() {
	return OS_OK;
}

ReturnState MovingActor::Release() {
	return OS_OK;
}

OsirisI::Graphics::Renderer::RenderInfo* MovingActor::GetRenderInfo() {
	return nullptr;
}

ReturnState MovingActor::HandleUserInput(std::shared_ptr<OsirisI::Events::IEvent> e) {
	events.push(e);
	return OS_OK;
}
