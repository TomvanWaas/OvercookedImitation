#pragma once
#include "GameScene.h"
class LoadgameScene final : public GameScene
{
public:
	explicit LoadgameScene();
	virtual ~LoadgameScene() = default;
	virtual GameScene* Clone() const override;
protected:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw() override;
private:
	void MakeTemplatesFood();
	void MakeTemplatesHolder();
};

