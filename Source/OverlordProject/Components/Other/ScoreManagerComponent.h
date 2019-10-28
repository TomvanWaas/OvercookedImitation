#pragma once
class Timer;
struct Recipe;
class TextComponent;
class ScoreManagerComponent final : public BaseComponent
{
public:
	explicit ScoreManagerComponent() = default;
	virtual ~ScoreManagerComponent() = default;

	void FailRecipe();
	void SucceedRecipe(const Timer& timer, const Recipe& recipe);

	int GetScore() const { return m_Score; }
	int GetNrFinishedRecipes() const { return m_NrFinishedRecipes; }
	int GetNrFailedRecipes() const { return m_NrFailedRecipes; }
protected:
	virtual void Update(const GameContext& gameContext) override;
	virtual void Draw(const GameContext& gameContext) override;
	virtual void Initialize(const GameContext& gameContext) override;
private:
	int m_Score = 0;
	int m_NrFinishedRecipes = 0;
	int m_NrFailedRecipes = 0;
	TextComponent* m_pScoreText = nullptr;

	void AddScore(int difference);
};

