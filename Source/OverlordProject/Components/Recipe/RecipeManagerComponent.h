#pragma once
#include "../../Structs//Recipe.h"

struct RecipeData final
{
	Recipe recipe;
	float time;
	eMealType meal;
};

struct CountData final
{
	UINT minimum;
	UINT average;
	UINT maximum;
	float delay;
};

class ScoreManagerComponent;
class RecipeComponent;
class RecipeManagerComponent final : public BaseComponent
{
public:
	explicit RecipeManagerComponent(CountData recipeCount, const std::vector<RecipeData>& recipeTemplateData, const DirectX::XMFLOAT3& spawnPos);
	virtual ~RecipeManagerComponent() = default;

	void Deliver(const Recipe& recipe);
	const std::vector<RecipeData>& GetRecipes() const { return m_RecipeTemplateData; }

protected:
	virtual void Draw(const GameContext& gameContext) override;
	virtual void Initialize(const GameContext& gameContext) override;
	virtual void Update(const GameContext& gameContext) override;
	virtual void OnNotify(ObservedID observedID) override;
private:
	std::vector<RecipeData> m_RecipeTemplateData;
	std::vector<RecipeComponent*> m_SpawnedRecipes;
	CountData m_RecipeCountData;

	DirectX::XMFLOAT3 m_SpawnPosition;
	float m_Accu = 0;

	ScoreManagerComponent* m_pScoreObject = nullptr;

	//Helpers
	GameObject* SpawnRecipe();
	float TargetTime() const;
	void UpdatePositions();
};

