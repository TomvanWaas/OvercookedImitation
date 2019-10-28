#pragma once

class TextComponent;
class TimerComponent final : public BaseComponent
{
public:
	explicit TimerComponent(float time, const std::function<void()>& onDeplete = nullptr);
	virtual ~TimerComponent() = default;
protected:
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
private:
	std::function<void()> m_OnDeplete;
	float m_Time;
	TextComponent* m_pTextComponent;


	std::wstring TimeString() const;
};

