#pragma once


class FirePostMaterial;
class PostProcessingManagerComponent final : public BaseComponent
{
public:
	explicit PostProcessingManagerComponent(FirePostMaterial* pFirePostMaterial);
	virtual ~PostProcessingManagerComponent() = default;
	
	void SetIntensity(float intensity, float speed);
protected:
	virtual void Initialize(const GameContext&) override {}
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext&) override {}
private:
	float m_Target;
	float m_Speed;
	bool m_IsActive;
	FirePostMaterial* m_pFirePostMaterial;


};

