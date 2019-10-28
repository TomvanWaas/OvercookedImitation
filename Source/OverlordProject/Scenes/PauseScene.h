#pragma once
#include "GameScene.h"
class InterfaceComponent;


class PauseScene final : public GameScene
{
public:
	explicit PauseScene();
	virtual ~PauseScene() = default;
	virtual GameScene* Clone() const override;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SceneActivated(GameScene* pOldScene) override;
	virtual void SceneDeactivated(GameScene* pNewScene) override;
private:
	GameScene* m_pPreviousScene = nullptr;
};

