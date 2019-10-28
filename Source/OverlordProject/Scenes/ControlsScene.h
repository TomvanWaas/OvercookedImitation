#pragma once
#include "GameScene.h"

class ControlsScene final : public GameScene
{
public:
	explicit ControlsScene();
	virtual ~ControlsScene() = default;
	virtual GameScene* Clone() const override;
protected:
	virtual void Initialize() override;
	virtual void Update() override{};
	virtual void Draw() override{};
	virtual void SceneActivated(GameScene* pOldScene) override;
private:
	GameScene* m_pPreviousScene = nullptr;
};

