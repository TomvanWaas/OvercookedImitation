#pragma once
class CameraLightComponent final : public BaseComponent
{
public:
	explicit CameraLightComponent(const DirectX::XMFLOAT3& translation, const DirectX::XMFLOAT3& rotation);
	virtual ~CameraLightComponent() = default;
	CameraLightComponent(const CameraLightComponent& other) = delete;
	CameraLightComponent(CameraLightComponent&& other)noexcept = delete;
	CameraLightComponent& operator=(const CameraLightComponent& other) = delete;
	CameraLightComponent& operator=(CameraLightComponent&& other)noexcept = delete;
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
private:
	DirectX::XMFLOAT3 m_Translation;
	DirectX::XMFLOAT3 m_Rotation;
	TransformComponent* m_pPivot;
};

