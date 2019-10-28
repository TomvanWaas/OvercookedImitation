#include "stdafx.h"
#include "ScoreScene.h"
#include "../Components/Interface/InterfaceComponent.h"
#include "OverlordGame.h"
#include "TextureData.h"
#include "TransformComponent.h"
#include "../Components/Interface/ButtonComponent.h"
#include "SceneManager.h"
#include "../Components/Interface/ButtonManagerComponent.h"
#include "TextComponent.h"
#include "../Singletons/AssetFiles.h"


ScoreScene::ScoreScene()
	: GameScene(L"ScoreScene")
{
}

GameScene* ScoreScene::Clone() const
{
	return new ScoreScene();
}

void ScoreScene::SetScoreData(int totalScore, int finishedRecipes, int failedRecipes)
{
	if (m_pTotalScoreText)
	{
		m_pTotalScoreText->SetText(std::to_wstring(totalScore));
	}
	if (m_pFinishedRecipesText)
	{
		m_pFinishedRecipesText->SetText(std::to_wstring(finishedRecipes));
	}
	if (m_pFailedRecipesText)
	{
		m_pFailedRecipesText->SetText(std::to_wstring(failedRecipes));
	}
}

void ScoreScene::Initialize()
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
	if (AssetFiles::GetInstance().Get(eAssetId::Texture_Menu_ScoreMenu, asset))
	{
		GameObject* pBackgroundObject = new GameObject();
		InterfaceComponent* pBackgroundInterface = new InterfaceComponent(asset);
		const DirectX::XMFLOAT2& dimensions = pBackgroundInterface->GetTexture()->GetDimension();
		pBackgroundObject->GetTransform()->Scale(width / dimensions.x, height / dimensions.y, 1);
		pBackgroundObject->GetTransform()->Translate(width*0.5f, height*0.5f, 0);
		pBackgroundObject->AddComponent(pBackgroundInterface);
		AddChild(pBackgroundObject);
	}

	//Continue Button
	//***************
	GameObject* pButtonObject = new GameObject();
	ButtonComponent* pButtonComponent = new ButtonComponent{L"Main Menu", {1,1,1,1}, {0.5f, 0.5f, 0.5f, 1}, [](ButtonComponent*)
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"MenuScene");
	} };
	pButtonObject->AddComponent(pButtonComponent);
	pButtonObject->GetTransform()->Translate(width*0.5f, height* 0.75f, 0);
	AddChild(pButtonObject);


	//Button Manager
	//**************
	GameObject* pButtonManagerObject = new GameObject();
	pButtonManagerObject->AddComponent(new ButtonManagerComponent(pButtonComponent, 0));
	AddChild(pButtonManagerObject);

	//Texts
	//*****
	std::wstring font{};
	if (AssetFiles::GetInstance().Get(eAssetId::Font_Menu_ScoreText, font))
	{
		const float columnOneX = width*0.325f;
		const float columnTwoX = width*0.625f;
		const float scoreY = height*0.2f;
		const float finishedY = height*0.30f;
		const float failedY = height*0.4f;
		const DirectX::XMFLOAT4 color{ 0.15f, 0.15f, 0.05f, 1 };

		//TotalScore
		//**********
		GameObject* pTSNumberObject = new GameObject();
		m_pTotalScoreText = new TextComponent(font, L"0");
		m_pTotalScoreText->SetColor(color);
		pTSNumberObject->AddComponent(m_pTotalScoreText);
		AddChild(pTSNumberObject);
		pTSNumberObject->GetTransform()->Translate(columnTwoX, scoreY, 0);

		GameObject* pTSSentenceObject = new GameObject();
		TextComponent* pTSSentenceComp = new TextComponent(font, L"Total Score:");
		pTSSentenceComp->SetColor(color);
		pTSSentenceObject->AddComponent(pTSSentenceComp);
		AddChild(pTSSentenceObject);
		pTSSentenceObject->GetTransform()->Translate(columnOneX, scoreY, 0);

		//FinishedRecipes
		//***************
		GameObject* pFRNumberObject = new GameObject();
		m_pFinishedRecipesText = new TextComponent(font, L"0");
		m_pFinishedRecipesText->SetColor(color);
		pFRNumberObject->AddComponent(m_pFinishedRecipesText);
		AddChild(pFRNumberObject);
		pFRNumberObject->GetTransform()->Translate(columnTwoX, finishedY, 0);

		GameObject* pFRSentenceObject = new GameObject();
		TextComponent* pFRSentenceComp = new TextComponent(font, L"Finished Recipes:");
		pFRSentenceComp->SetColor(color);
		pFRSentenceObject->AddComponent(pFRSentenceComp);
		AddChild(pFRSentenceObject);
		pFRSentenceObject->GetTransform()->Translate(columnOneX, finishedY, 0);

		//FailedRecipes
		//*************
		GameObject* pFaRNumberObject = new GameObject();
		m_pFailedRecipesText = new TextComponent(font, L"0");
		m_pFailedRecipesText->SetColor(color);
		pFaRNumberObject->AddComponent(m_pFailedRecipesText);
		AddChild(pFaRNumberObject);
		pFaRNumberObject->GetTransform()->Translate(columnTwoX, failedY, 0);

		GameObject* pFaRSentenceObject = new GameObject();
		TextComponent* pFaRSentenceComp = new TextComponent(font, L"Failed Recipes:");
		pFaRSentenceComp->SetColor(color);
		pFaRSentenceObject->AddComponent(pFaRSentenceComp);
		AddChild(pFaRSentenceObject);
		pFaRSentenceObject->GetTransform()->Translate(columnOneX, failedY, 0);

	}
	UNREFERENCED_PARAMETER(gameContext);
}
void ScoreScene::Update()
{
}
void ScoreScene::Draw()
{
}