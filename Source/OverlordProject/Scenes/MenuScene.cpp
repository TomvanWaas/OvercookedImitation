#include "stdafx.h"
#include "MenuScene.h"
#include "../Components/Interface/InterfaceComponent.h"
#include "../Singletons/AssetFiles.h"
#include "OverlordGame.h"
#include "TextureData.h"
#include "TransformComponent.h"
#include "../Components/Interface/ButtonComponent.h"
#include "SceneManager.h"
#include "../Components/Interface/ButtonManagerComponent.h"
#include "ShipScene.h"
#include "Destroyer.h"
#include "../Helpers/SceneSetter.h"
#include "SoundManager.h"
#include "../Helpers/SoundID.h"

MenuScene::MenuScene()
	: GameScene(L"MenuScene")
{
}

GameScene* MenuScene::Clone() const
{
	return new MenuScene();
}

void MenuScene::Initialize()
{
	//Camera
	//******
	GameObject* pCamera = new GameObject();
	CameraComponent* pCameraComponent = new CameraComponent();
	pCamera->AddComponent(pCameraComponent);
	SetActiveCamera(pCameraComponent);
	AddChild(pCamera);


	//
	std::wstring asset{};
	const GameSettings& gameSettings = OverlordGame::GetGameSettings();
	const int width = gameSettings.Window.Width;
	const int height = gameSettings.Window.Height;

	//Background
	//**********
	if (AssetFiles::GetInstance().Get(eAssetId::Texture_Menu_MainMenu, asset))
	{
		GameObject* pBackground = new GameObject();
		InterfaceComponent* pBackgroundVisuals = new InterfaceComponent(asset);
		pBackground->AddComponent(pBackgroundVisuals);
		AddChild(pBackground);

		DirectX::XMFLOAT2 dim = pBackgroundVisuals->GetTexture()->GetDimension();
		pBackground->GetTransform()->Scale(width / dim.x, height / dim.y, 1);
		pBackground->GetTransform()->Translate(width*0.5f, height*0.5f, 0);
	}


	//Buttons (TopLeft = {0,0} )
	//*******
	DirectX::XMFLOAT4 selected{ 1,1,1,1 };
	DirectX::XMFLOAT4 unselected{ 1,1,1,0.5f };
	const float size = 128.0f;
	const float offsetY = size * 1.00f;
	const float baseWidth = 0.5f* width;
	const float baseHeight = 0.55f* height;
	
	//Player Count Button
	//*******************
	const float offsetX = 0.20f * width;
	const int maxPlayers = 4;
	ButtonComponent* pPlayerButtons[maxPlayers]{ nullptr };
	for (int count = maxPlayers; count > 0; --count)
	{
		int index = count - 1;

		GameObject* pButton = new GameObject();
		pPlayerButtons[index] = new ButtonComponent(std::to_wstring(count) + L" Players", selected, unselected, [this, count](ButtonComponent*)
		{
			SceneSetter::SetScene(new ShipScene(size_t(count)));
		});
		pButton->AddComponent(pPlayerButtons[index]);
		AddChild(pButton);

		//Transform
		auto* pTransform = pButton->GetTransform();
		pTransform->Scale(0.5f, 1, 1);
		pTransform->Translate(baseWidth - 0.5f*offsetX + (index) % 2 * offsetX, baseHeight - int(count <= 2)*offsetY, 0);
	}
	

	//Controls Button
	//***************
	GameObject* pControlsButton = new GameObject();
	ButtonComponent* pControlsComp = new ButtonComponent(L"Controls", selected, unselected, [](ButtonComponent*)
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"ControlsScene");
	});
	pControlsButton->AddComponent(pControlsComp);
	AddChild(pControlsButton);
	pControlsButton->GetTransform()->Translate(baseWidth, baseHeight + offsetY, 0);

	//Quit Button
	//***********
	GameObject* pQuitButton = new GameObject();
	ButtonComponent* pQuitComp = new ButtonComponent(L"Quit", selected, unselected, [](ButtonComponent*)
	{
		OverlordGame::Quit();
	});
	pQuitButton->AddComponent(pQuitComp);
	AddChild(pQuitButton);
	pQuitButton->GetTransform()->Translate(baseWidth, baseHeight + 2 * offsetY, 0);

	//HIERARCHY
	//*********
	pPlayerButtons[0]->SetDown(pPlayerButtons[2]);
	pPlayerButtons[0]->SetUp(pQuitComp);
	pPlayerButtons[0]->SetLeft(pPlayerButtons[1]);
	pPlayerButtons[0]->SetRight(pPlayerButtons[1]);
	pPlayerButtons[1]->SetDown(pPlayerButtons[3]);
	pPlayerButtons[1]->SetUp(pQuitComp);
	pPlayerButtons[1]->SetLeft(pPlayerButtons[0]);
	pPlayerButtons[1]->SetRight(pPlayerButtons[0]);

	pPlayerButtons[2]->SetDown(pControlsComp);
	pPlayerButtons[2]->SetUp(pPlayerButtons[0]);
	pPlayerButtons[2]->SetLeft(pPlayerButtons[3]);
	pPlayerButtons[2]->SetRight(pPlayerButtons[3]);
	pPlayerButtons[3]->SetDown(pControlsComp);
	pPlayerButtons[3]->SetUp(pPlayerButtons[1]);
	pPlayerButtons[3]->SetLeft(pPlayerButtons[2]);
	pPlayerButtons[3]->SetRight(pPlayerButtons[2]);





	pControlsComp->SetDown(pQuitComp);
	pControlsComp->SetUp(pPlayerButtons[2]);
	pQuitComp->SetDown(pPlayerButtons[0]);
	pQuitComp->SetUp(pControlsComp);

	//Manager
	//*******
	GameObject* pButtonManager = new GameObject();
	ButtonManagerComponent* pManagerComp = new ButtonManagerComponent(pPlayerButtons[0], 0);
	pButtonManager->AddComponent(pManagerComp);
	AddChild(pButtonManager);
}

void MenuScene::SceneActivated(GameScene*)
{
	//Music
	//*****
	SoundManager::GetInstance()->PlaySoundByID(int(eSoundID::Main02Looped), int(eChannelID::Background), 0.025f, eSoundMerge::ContinueElseReplay);

}
