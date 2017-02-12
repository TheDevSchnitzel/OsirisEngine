#ifndef MOVINGACTOR_HPP_
#define MOVINGACTOR_HPP_

namespace OsirisI {
	namespace Graphics {
		namespace Renderer {
			class IRenderer;
		}

		namespace Actors {
			class MovingActor : public IActor {
				private:
					OsirisI::OVector3 currentRotationSpeedInGradPerSecond, currentMovmentSpeedInMeterPerSecondParted;
					float rotationAccelerationInGradPerSecond, movementAccelerationInMeterPerSecond, currentMovmentSpeedInMeterPerSecond;
					float maxRotationSpeedInGrad, maxMovementSpeedInMeter;
					bool relativisticMode;
					KeyboardKeys keyAccelerate = KeyboardKeys::ESC, keyDeccelerate = KeyboardKeys::ESC;
					KeyboardKeys rotateYawPos = KeyboardKeys::ArrowRight, rotateYawNeg = KeyboardKeys::ArrowLeft;
					KeyboardKeys rotatePitchPos = KeyboardKeys::ArrowUp, rotatePitchNeg = KeyboardKeys::ArrowDown;
					KeyboardKeys rotateRollPos = KeyboardKeys::ESC, rotateRollNeg = KeyboardKeys::ESC;
					KeyboardKeys moveForward = KeyboardKeys::W, moveBackward = KeyboardKeys::S;
					KeyboardKeys strafeLeft = KeyboardKeys::A, stafeRight = KeyboardKeys::D;
					KeyboardKeys moveUp = KeyboardKeys::ESC, moveDown = KeyboardKeys::ESC;

					std::queue<std::shared_ptr<OsirisI::Events::IEvent>> events;

					void DoPos(float delta);
				protected:

				public:
					MovingActor(OsirisI::Utilities::HashedString name);
					~MovingActor();

					ReturnState Initialize();
					ReturnState Update(float delta);
					ReturnState Recover();
					ReturnState Release();
					OsirisI::Graphics::Renderer::RenderInfo* GetRenderInfo();

					ReturnState HandleUserInput(std::shared_ptr<OsirisI::Events::IEvent> e);

					OsirisI::OVector3 GetCurrentRotationSpeedInGradPerSecond() {
						return this->currentRotationSpeedInGradPerSecond;
					}
					void SetCurrentRotationSpeedInGradPerSecond(OsirisI::OVector3 val) {
						this->currentRotationSpeedInGradPerSecond = val;
					}

					OsirisI::OVector3 GetCurrentMovmentSpeedInMeterPerSecond() {
						return this->currentMovmentSpeedInMeterPerSecondParted;
					}
					void SetCurrentMovmentSpeedInMeterPerSecond(OsirisI::OVector3 val) {
						this->currentMovmentSpeedInMeterPerSecondParted = val;
					}

					float GetRotationAccelerationInGradPerSecond() {
						return this->rotationAccelerationInGradPerSecond;
					}
					void SetRotationAccelerationInGradPerSecond(float val) {
						this->rotationAccelerationInGradPerSecond = val;
					}

					float GetMovementAccelerationInMeterPerSecond() {
						return this->movementAccelerationInMeterPerSecond;
					}
					void SetMovementAccelerationInMeterPerSecond(float val) {
						this->movementAccelerationInMeterPerSecond = val;
					}

					float GetMaxRotationSpeedInGrad() {
						return this->maxRotationSpeedInGrad;
					}
					void SetMaxRotationSpeedInGrad(float val) {
						this->maxRotationSpeedInGrad = val;
					}

					float GetMaxMovementSpeedInMeter() {
						return this->maxMovementSpeedInMeter;
					}
					void SetMaxMovementSpeedInMeter(float val) {
						this->maxMovementSpeedInMeter = val;
					}

					bool IsRelativisticMode() {
						return this->relativisticMode;
					}
					void SetRelativisticMode(bool val) {
						this->relativisticMode = val;
					}
			};
		}
	}
}

#endif /* MOVINGACTOR_HPP_ */