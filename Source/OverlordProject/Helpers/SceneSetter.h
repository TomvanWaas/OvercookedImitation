#pragma once

class GameScene;
class SceneSetter final
{
public:
	virtual ~SceneSetter() = default;
	SceneSetter(const SceneSetter& other) = delete;
	SceneSetter(SceneSetter&& other) noexcept = delete;
	SceneSetter& operator=(const SceneSetter& other) = delete;
	SceneSetter& operator=(SceneSetter&& other) noexcept = delete;

	static void SetScene(GameScene* pScene);
	static void RestartScene();
private:
	static GameScene* m_pCurrentGame;

	SceneSetter() = default;

};