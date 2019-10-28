#include "stdafx.h"
#include "PauseScene.h"
#include "../Components/Interface/InterfaceComponent.h"
#include "OverlordGame.h"
#include "TransformComponent.h"
#include "TextureData.h"
#include "FixedCamera.h"
#include "../Components/Interface/ButtonComponent.h"
#include "../Components/Interface/ButtonManagerComponent.h"
#include "SceneManager.h"
#include "../Singletons/AssetFiles.h"
#include "ControlsScene.h"
#include "../Helpers/SceneSetter.h"

PauseScene::PauseScene()
	: GameScene(L"PauseScene")
{
}

GameScene* PauseScene::Clone() const
{
	return new PauseScene();
}

void PauseScene::Initialize()
{
	const GameContext& gameContext = GetGameContext();
	const GameSettings& gameSettings = OverlordGame::GetGameSettings();
	float width = float(gameSettings.Window.Width);
	float height = float(gameSettings.Window.Height);

	//Fixed Camera
	//************
	GameObject* pCameraObject = new GameObject();
	CameraComponent* pCameraComponent = new CameraComponent();
	pCameraObject->AddComponent(pCameraComponent);
	AddChild(pCameraObject);
	SetActiveCamera(pCameraComponent);

	//Background
	//**********
	std::wstring asset{};
	if (AssetFiles::GetInstance().Get(eAssetId::Texture_Menu_PauseMenu, asset))
	{
		GameObject* pBackgroundObject = new GameObject();
		InterfaceComponent* pBackgroundInterface = new InterfaceComponent(asset);
		const DirectX::XMFLOAT2& dimensions = pBackgroundInterface->GetTexture()->GetDimension();
		pBackgroundObject->GetTransform()->Scale(width / dimensions.x, height / dimensions.y, 1);
		pBackgroundObject->GetTransform()->Translate(width*0.5f, height*0.5f, 0);
		pBackgroundObject->AddComponent(pBackgroundInterface);
		AddChild(pBackgroundObject);
	}

	//Buttons
	//*******
	DirectX::XMFLOAT4 selectColor{ 1,1,1,1 };
	DirectX::XMFLOAT4 unselectColor{ 0.5f, 0.5f, 0.5f, 1 };
	const float size = 128.0f;
	const float offset = size * 1.1f;
	const float baseX = 0.5f * width;
	const float baseY = 0.5f * height;

	//Continue Button
	//***************
	GameObject* pContinueObject = new GameObject();
	ButtonComponent* pContinueComponent = new ButtonComponent(L"Return", selectColor, unselectColor, [this](ButtonComponent*)
	{
		if (m_pPreviousScene)
		{
			SceneManager::GetInstance()->SetActiveGameScene(m_pPreviousScene->GetSceneName());
		}
	});
	pContinueObject->AddComponent(pContinueComponent);
	pContinueObject->GetTransform()->Translate(baseX, baseY - offset, 0);
	AddChild(pContinueObject);

	//Restart Button
	//***************
	GameObject* pRestartObject = new GameObject();
	ButtonComponent* pRestartComponent = new ButtonComponent(L"Restart", selectColor, unselectColor, [](ButtonComponent*)
	{
		SceneSetter::RestartScene();
	});
	pRestartObject->AddComponent(pRestartComponent);
	pRestartObject->GetTransform()->Translate(baseX, baseY, 0);
	AddChild(pRestartObject);

	//Controls Button
	//***************
	GameObject* pControlsObject = new GameObject();
	ButtonComponent* pControlsComponent = new ButtonComponent(L"Controls", selectColor, unselectColor, [](ButtonComponent*)
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"ControlsScene");
	});
	pControlsObject->AddComponent(pControlsComponent);
	pControlsObject->GetTransform()->Translate(baseX, baseY + offset, 0);
	AddChild(pControlsObject);

	//Main Menu Button
	//***************
	GameObject* pMainButton = new GameObject();
	ButtonComponent* pMainButtonComponent = new ButtonComponent{ L"Main Menu", selectColor, unselectColor, [](ButtonComponent*)
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"MenuScene");
	} };
	pMainButton->AddComponent(pMainButtonComponent);
	pMainButton->GetTransform()->Translate(baseX, baseY + 2*offset, 0);
	AddChild(pMainButton);

	//HIERARCHY
	//*********
	pContinueComponent->SetDown(pRestartComponent);
	pContinueComponent->SetUp(pMainButtonComponent);
	pRestartComponent->SetDown(pControlsComponent);
	pRestartComponent->SetUp(pContinueComponent);
	pControlsComponent->SetDown(pMainButtonComponent);
	pControlsComponent->SetUp(pRestartComponent);
	pMainButtonComponent->SetDown(pContinueComponent);
	pMainButtonComponent->SetUp(pControlsComponent);


	//Button Manager
	//**************
	GameObject* pButtonManagerObject = new GameObject();
	pButtonManagerObject->AddComponent(new ButtonManagerComponent(pContinueComponent, 0));
	AddChild(pButtonManagerObject);

	
	UNREFERENCED_PARAMETER(gameContext);
}

void PauseScene::Update()
{
}

void PauseScene::Draw()
{
	if (m_pPreviousScene)
	{
		DrawScene(m_pPreviousScene);
	}
}

void PauseScene::SceneActivated(GameScene* pOldScene)
{
	//Don't allow: 
	//- ControlScene
	//...
	if (dynamic_cast<ControlsScene*>(pOldScene) == nullptr)
	{
		m_pPreviousScene = pOldScene;
		SetActiveCamera(m_pPreviousScene->GetActiveCamera());
	}
	else if (m_pPreviousScene)
	{
		SetActiveCamera(m_pPreviousScene->GetActiveCamera());
	}
}

void PauseScene::SceneDeactivated(GameScene*)
{
	//Return camera
	SetActiveCamera(GetObjectWith<CameraComponent>());
}
