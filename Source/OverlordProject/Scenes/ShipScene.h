#pragma once
#include "GameScene.h"
struct RecipeData;


class ShipScene final : public GameScene
{
public:
	explicit ShipScene(size_t playerCount);
	virtual ~ShipScene() = default;

	ShipScene(const ShipScene& other) = delete;
	ShipScene(ShipScene&& other) noexcept = delete;
	ShipScene& operator=(const ShipScene& other) = delete;
	ShipScene& operator=(ShipScene&& other) noexcept = delete;

	virtual GameScene* Clone() const override;
protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SceneActivated(GameScene* pOldScene) override;
	void SceneDeactivated(GameScene* pNewScene) override;
private:	
	size_t m_PlayerCount;


	//Map Building
	GameObject* MakeMap();
	GameObject* MakeScene();
	std::vector<RecipeData> MakeRecipeTemplates(const GameContext& gameContext);

};
