#include "stdafx.h"
#include "ControlsScene.h"
#include "../Components/Interface/ButtonManagerComponent.h"
#include "OverlordGame.h"
#include "../Singletons/AssetFiles.h"
#include "../Components/Interface/InterfaceComponent.h"
#include "TransformComponent.h"
#include "../Components/Interface/ButtonComponent.h"
#include "TextureData.h"
#include "SceneManager.h"

ControlsScene::ControlsScene()
	: GameScene(L"ControlsScene")
{
}

GameScene* ControlsScene::Clone() const
{
	return new ControlsScene();
}

void ControlsScene::Initialize()
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
	if (AssetFiles::GetInstance().Get(eAssetId::Texture_Menu_ControlsMenu, asset))
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
	pContinueObject->GetTransform()->Translate(width*0.5f, height*0.9f, 0);
	AddChild(pContinueObject);


	//Button Manager
	//**************
	GameObject* pButtonManagerObject = new GameObject();
	pButtonManagerObject->AddComponent(new ButtonManagerComponent(pContinueComponent, 0));
	AddChild(pButtonManagerObject);


	UNREFERENCED_PARAMETER(gameContext);
}

void ControlsScene::SceneActivated(GameScene* pOldScene)
{
	m_pPreviousScene = pOldScene;
}
