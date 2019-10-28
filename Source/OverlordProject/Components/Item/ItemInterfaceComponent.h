#pragma once
class ItemInterfaceComponent : public BaseComponent
{
public:
	explicit ItemInterfaceComponent() = default;
	virtual ~ItemInterfaceComponent() = default;
	virtual bool Use(const GameContext& gameContext) = 0;
	virtual void Highlight(const DirectX::XMFLOAT4& color = { 1.25f, 1.25f, 1.25f, 1.0f }) = 0;
};

