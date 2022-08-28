#pragma once
#include "Components/GameObjectComponent.h"
#include <Server/ClientHandler.h>
#include <Graphics/OpenGL/Shader.h>

#include "PlayerInputState.h"

namespace Skel
{
	enum PlayerAnim // Order matters, use this as Server can't access anim names
	{
		Stand, TPose, WalkBackward,WalkForward,WalkLeft,WalkRight
	};
	class PlayerComponent : public GameObjectComponent
	{
	public:
		PlayerComponent();
		void ProcessInput(const PlayerInputState& input, double dt);
		void ApplySnapshotState(const struct PlayerSnapshotState& state);
		void Server_PostProcessInput(Net::ClientHandler* clientHandler, GameObject* playerList[Net::MAX_PLAYERS]);
		bool IsLocalClient() const;
		const uint8 GetCurrentAnimationIndex() const;
		const uint8 GetOverlayAnimationIndex() const;
		const Vector3 GetVelocity() const;
		void SetVelocity(Vector3);

		virtual void OnCreated() override;
		virtual void Update() override;
		virtual void PostUpdate() override;
		virtual void Draw() override;

		RTTR_ENABLE(GameObjectComponent)
		RTTR_REGISTRATION_FRIEND
	private:
		class NetworkComponent* m_Network;
		class ModelAnimationComponent* m_Animation;
		class RigidBodyComponent* m_RigidBody;
		class ColliderComponent* m_Collider;
		ShaderPtr				m_Shader;
		ModelPtr				m_HeadModel;
		ShaderPtr				m_HeadShader;
	};
}
