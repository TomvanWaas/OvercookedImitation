#pragma once
#include "../../Helpers/Timer.h"
class ShakeComponent;
class ColorFlickerComponent;
enum class eMealType;
#include "../../Structs/Recipe.h"
class SpriteComponent;
class LerpComponent;
class RecipeComponent final : public BaseComponent
{
public:
	explicit RecipeComponent(const Recipe& recipe, float time, eMealType meal);
	virtual ~RecipeComponent() = default;

	float GetRemainingTime() const;
	const Timer& GetTimer() const;
	float GetWidth() const;
	const Recipe& GetRecipe() const { return m_Recipe; }
	void Remove(const std::function<void()>& onDestroy = nullptr);
	void Activate();

protected:
	virtual void Update(const GameContext& gameContext) override;
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
private:
	Recipe m_Recipe;
	Timer m_Accu;
	float m_Speed;
	eMealType m_Meal;
	bool m_Activated = false;

	LerpComponent* m_pRecipeLerpComponent = nullptr;
	SpriteComponent* m_pTimerSprite = nullptr;
	ShakeComponent* m_pShakeComponent = nullptr;
	ColorFlickerComponent* m_pFlickerComponent = nullptr;


	DirectX::XMFLOAT4 Lerp(DirectX::XMFLOAT4 a, DirectX::XMFLOAT4 b, float t) const;

	
};

