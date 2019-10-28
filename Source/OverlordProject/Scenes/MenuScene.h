#pragma once
#include "GameScene.h"

class MenuScene final : public GameScene
{
public:
	explicit MenuScene();
	virtual ~MenuScene() = default;
	virtual GameScene* Clone() const override;
protected:
	virtual void Initialize() override;
	virtual void Update() override {}
	virtual void Draw() override {}
	virtual void SceneActivated(GameScene* pOldScene) override;
private:
};

