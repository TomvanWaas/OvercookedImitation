#pragma once
class CameraEventComponent;
class MoverComponent;
class EventManagerComponent final : public BaseComponent
{
public:
	explicit EventManagerComponent(float delay, bool startLeft = true);
	virtual ~EventManagerComponent() = default;
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	CameraEventComponent* m_pCamereEvent;
	std::vector<MoverComponent*> m_pMoveables;
	float m_Accu;
	float m_Delay;
	bool m_MoveLeft;


};

