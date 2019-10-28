#include "stdafx.h"
#include "SceneSetter.h"
#include "GameScene.h"
#include "SceneManager.h"
#include "Destroyer.h"
#include "SoundManager.h"

GameScene* SceneSetter::m_pCurrentGame = nullptr;

void SceneSetter::SetScene(GameScene* pScene)
{
	if (pScene)
	{
		SceneManager* pManager = SceneManager::GetInstance();

		//Remove Old GameScene
		//********************
		if (m_pCurrentGame)
		{
			pManager->RemoveGameScene(m_pCurrentGame);
			Destroyer::GetInstance().Destroy(m_pCurrentGame);
		}

		//Set New GameScene
		//*****************
		pManager->AddGameScene(pScene);
		pManager->SetActiveGameScene(pScene->GetSceneName());
		m_pCurrentGame = pScene;
	}
}

void SceneSetter::RestartScene()
{
	if (m_pCurrentGame)
	{
		SoundManager::GetInstance()->StopAll();
		SetScene(m_pCurrentGame->Clone());
	}
}
