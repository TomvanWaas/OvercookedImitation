#pragma once
#include "GameScene.h"

class TextComponent;
class ScoreScene final : public GameScene
{
public:
	explicit ScoreScene();
	virtual ~ScoreScene() = default;
	virtual GameScene* Clone() const override;
	void SetScoreData(int totalScore, int finishedRecipes, int failedRecipes);
protected:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
private:

	TextComponent* m_pTotalScoreText;
	TextComponent* m_pFinishedRecipesText;
	TextComponent* m_pFailedRecipesText;
};

