#include "stdafx.h"
#include "ShipScene.h"
#include "../Prototype/PrototypeObject.h"
#include "TransformComponent.h"
#include "PhysxManager.h"
#include "../Components/Recipe/RecipeManagerComponent.h"
#include "../Helpers/Helpers.h"
#include "ModelAnimator.h"
#include "../Components/Box/BoxComponentBase.h"
#include "../Components/Food/FoodInfo.h"
#include "../Structs/ModelInfo.h"
#include "../Singletons/AssetFiles.h"
#include "../Components/Holder/HolderInfo.h"
#include "ModelComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "../Components/Other/FireComponent.h"
#include "../Components/Other/MoverComponent.h"
#include "SceneManager.h"
#include "OverlordGame.h"
#include "../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "../Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"
#include "ScoreScene.h"
#include "../Components/Other/ScoreManagerComponent.h"
#include "../Components/Other/EventManagerComponent.h"
#include "../Materials/WaterMaterial.h"
#include "../Materials/PostProcessing/FirePostMaterial.h"
#include "../Components/Visuals/PostProcessingManagerComponent.h"
#include "SoundManager.h"
#include "../Helpers/SoundID.h"
#include "PauseScene.h"

ShipScene::ShipScene(size_t playerCount)
	: GameScene(L"ShipScene")
	, m_PlayerCount(playerCount)
{
	Clamp<size_t>(m_PlayerCount, 4, 1);
}

GameScene* ShipScene::Clone() const
{
	return new ShipScene(m_PlayerCount);
}

void ShipScene::Initialize()
{
	//Helpers::EnableDebugRendering(GetPhysxProxy(), true);

	const auto gameContext = GetGameContext();

	//LIGHT DIRECTION
	//***************
	const float lightOffset = 20.0f;
	DirectX::XMFLOAT3 lightDir{ -0.05f, -5.0f, 1.0f };
	DirectX::XMFLOAT3 lightPos{ lightOffset * -lightDir.x, lightOffset * -lightDir.y, lightOffset * -lightDir.z };
	ModelInfo::SetLightDirection(lightDir);
	gameContext.pShadowMapper->SetLight(lightPos, lightDir);

	//CAMERA
	//******
	auto* pCamera = Helpers::MakeCamera(gameContext, this);
	auto* pTransform = pCamera->GetTransform();
	pTransform->Translate(0, 90, -45);
	pTransform->Rotate(60, 0, 0);

	//CHARACTERS
	//**********
	GameObject* pCharacter = Helpers::MakeCharacter(gameContext, 0, 1.25f, 5, 35, 10, {1,0,0,1});
	pCharacter->GetTransform()->Translate(-6, 0, 2);
	AddChild(pCharacter);
	if (m_PlayerCount > 1)
	{
		pCharacter = Helpers::MakeCharacter(gameContext, 1, 1.25f, 5, 35, 10, { 0,1,0,1 });
		pCharacter->GetTransform()->Translate(6, 0, 2);
		AddChild(pCharacter);
	}
	if (m_PlayerCount > 2)
	{
		pCharacter = Helpers::MakeCharacter(gameContext, 2, 1.25f, 5, 35, 10, { 0,0,1,1 });
		pCharacter->GetTransform()->Translate(6, 0, -2);
		AddChild(pCharacter);
	}
	if (m_PlayerCount > 3)
	{
		pCharacter = Helpers::MakeCharacter(gameContext, 3, 1.25f, 5, 35, 10, { 1,1,0,1 });
		pCharacter->GetTransform()->Translate(-6, 0, -2);
		AddChild(pCharacter);
	}

	//MAP & SCENE
	//***********
	GameObject* pKinematicObjects = MakeMap();
	AddChild(MakeScene());

	const auto& children = pKinematicObjects->GetChildren();
	for (auto* pChild : children)
	{
		pChild->AddComponent(new MoverComponent());
	}

	GameObject* pEventObject = new GameObject();
	pEventObject->AddComponent(new EventManagerComponent(30.0f));
	AddChild(pEventObject);

	//WATER
	//*****
	GameObject* pWaterObject = new GameObject();
	ModelComponent* pWaterModel = new ModelComponent(L"./Resources/Models/Default_PlaneHigh.ovm");
	WaterMaterial* pWaterMaterial = new WaterMaterial();
	UINT waterID = gameContext.pMaterialManager->GetFreeID();
	gameContext.pMaterialManager->AddMaterial(pWaterMaterial, waterID);
	pWaterModel->SetMaterial(waterID);
	pWaterMaterial->SetBumpScale(5.0f);
	pWaterMaterial->SetTimeSpeed(0.25f);
	pWaterObject->AddComponent(pWaterModel);
	AddChild(pWaterObject);

	pWaterObject->GetTransform()->Scale(20, 20, 20);
	pWaterObject->GetTransform()->Translate(0, -5, 0);

	//POST (FIRE EFFECT)
	//******************
	auto* pFirePostMaterial = new FirePostMaterial();
	AddPostProcessingEffect(pFirePostMaterial);
	GameObject* pFirePostManager = new GameObject();
	PostProcessingManagerComponent* pFirePostManagerComp = new PostProcessingManagerComponent(pFirePostMaterial);
	pFirePostManager->AddComponent(pFirePostManagerComp);
	AddChild(pFirePostManager);


}

void ShipScene::Update()
{	
}
void ShipScene::Draw()
{
}

void ShipScene::SceneActivated(GameScene*)
{
	//Music
	//*****
	SoundManager::GetInstance()->PlaySoundByID(int(eSoundID::Main01Looped), int(eChannelID::Background), 0.025f, eSoundMerge::ContinueElseReplay);
	SoundManager::GetInstance()->PauseAllExcept(int(eChannelID::Background), false);
}
void ShipScene::SceneDeactivated(GameScene* pNewScene)
{
	//Next Scene == ScoreScene?
	//*************************
	ScoreScene* pScoreScene = dynamic_cast<ScoreScene*>(pNewScene);
	if (pScoreScene)
	{
		ScoreManagerComponent* pScoreManager = GetDynamicObjectWith<ScoreManagerComponent>(true);
		if (pScoreManager)
		{
			pScoreScene->SetScoreData(pScoreManager->GetScore(), pScoreManager->GetNrFinishedRecipes(), pScoreManager->GetNrFailedRecipes());
		}
		else
		{
			pScoreScene->SetScoreData(0, 0, 0);
		}
	}

	//Music
	//*****
	SoundManager::GetInstance()->PauseAllExcept(int(eChannelID::Background), true);
}





GameObject* ShipScene::MakeMap()
{
	//Values
	DirectX::XMFLOAT3 scale{ 1,0.5f,1 };
	const float offset = 6.0f;
	const auto& gameContext = GetGameContext();
	auto pPhysx = PhysxManager::GetInstance()->GetPhysics();
	auto pMaterial = pPhysx->createMaterial(0, 0, 0);

	GameObject* pObject = nullptr;

	//Make Scene
	GameObject* pKinematicObjects = new GameObject();
	AddChild(pKinematicObjects);


	//Managers
	//********
	{
		const float y = 128.0f;
		const float x = 256.0f;
		const float time = 600.0f;

		GameObject* pScoreManager = Helpers::MakeScoreManager(gameContext);
		pScoreManager->GetTransform()->Translate(0, OverlordGame::GetGameSettings().Window.Height - y, 0);
		AddChild(pScoreManager);

		GameObject* pTimer = Helpers::MakeTimeManager(gameContext, time, L"ScoreScene");
		pTimer->GetTransform()->Translate(OverlordGame::GetGameSettings().Window.Width - x, OverlordGame::GetGameSettings().Window.Height - y, 0);
		AddChild(pTimer);
	}
	GameObject* pRecipeManager = Helpers::MakeRecipeManager(gameContext
	, CountData{1, 2, 4, 15.0f}
	, MakeRecipeTemplates(gameContext));
	pRecipeManager->GetTransform()->Scale(0.3f, 0.3f, 0.3f);
	AddChild(pRecipeManager);


#pragma region Boxes
	
	//  *******
	//     *    *
	//     *    *
	//     *    *
	//     *    *
	//     *
	//************


	//Middle Column (Moveable)
	//************************
	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, false);
	pObject->GetTransform()->Translate(0, 0, 0);
	pObject->GetTransform()->Scale(scale);
	pKinematicObjects->AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, false);
	pObject->GetTransform()->Translate(0, 0, -offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	pKinematicObjects->AddChild(pObject);

	pObject = Helpers::MakeBoxBin(gameContext, pMaterial, false);
	pObject->GetTransform()->Translate(0, 0, -2 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	pKinematicObjects->AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, false);
	pObject->GetTransform()->Translate(0, 0, offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	pKinematicObjects->AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, false);
	pObject->GetTransform()->Translate(0, 0, 2 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	pKinematicObjects->AddChild(pObject);

	//Top Row (Static)
	//****************
	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(0, 0, 3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxFire(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(-offset * scale.x, 0, 3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	Helpers::AddHolder(pObject->GetDynamicComponent<BoxComponentBase>(), eHolderType::Boiler);
	AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(-2 * offset * scale.x, 0, 3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxFire(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(-3 * offset * scale.x, 0, 3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	Helpers::AddHolder(pObject->GetDynamicComponent<BoxComponentBase>(), eHolderType::Pan);
	AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(offset * scale.x, 0, 3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxDelivery(gameContext, pMaterial, pRecipeManager->GetDynamicComponent<RecipeManagerComponent>());
	pObject->GetTransform()->Translate(2.5f * offset * scale.x, 0, 3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	pObject->GetTransform()->Rotate(0, 180, 0);
	AddChild(pObject);

	pObject = Helpers::MakeBoxHolderContainer(gameContext, pMaterial, eHolderType::Plate, true);
	pObject->GetTransform()->Translate(4 * offset * scale.x, 0, 3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	//Bottom Row (Static)
	//*******************
	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(0, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(-offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxChoppingBox(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(-2 * offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(-3 * offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxChoppingBox(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(-4 * offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(-5 * offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxFoodContainer(gameContext, pMaterial, eFoodType::Apple, true);
	pObject->GetTransform()->Translate(offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxFoodContainer(gameContext, pMaterial, eFoodType::Broccoli, true);
	pObject->GetTransform()->Translate(2 * offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxFoodContainer(gameContext, pMaterial, eFoodType::Mushroom, true);
	pObject->GetTransform()->Translate(3 * offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxFoodContainer(gameContext, pMaterial, eFoodType::Pepper, true);
	pObject->GetTransform()->Translate(4 * offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxFoodContainer(gameContext, pMaterial, eFoodType::Potato, true);
	pObject->GetTransform()->Translate(5 * offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, true);
	pObject->GetTransform()->Translate(6 * offset * scale.x, 0, -3 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	auto* pExtinguisher = Helpers::MakeItemExtinguisher(gameContext);
	if (!Helpers::AddItem(pObject->GetDynamicComponent<BoxComponentBase>(), pExtinguisher))
	{
		Logger::LogWarning(L"Scene >> Failed to add Extinguisher");
		SafeDelete(pExtinguisher);
	}
	AddChild(pObject);

	//Right Column (Moveable)
	//***********************
	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, false);
	pObject->GetTransform()->Translate(6 * offset * scale.x, 0, -1 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	pKinematicObjects->AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, false);
	pObject->GetTransform()->Translate(6 * offset * scale.x, 0, 0 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	pKinematicObjects->AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, false);
	pObject->GetTransform()->Translate(6 * offset * scale.x, 0, 1 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	pKinematicObjects->AddChild(pObject);

	pObject = Helpers::MakeBoxBase(gameContext, pMaterial, false);
	pObject->GetTransform()->Translate(6 * offset * scale.x, 0, 2 * offset * scale.z);
	pObject->GetTransform()->Scale(scale);
	pKinematicObjects->AddChild(pObject);

#pragma endregion Boxes

	return pKinematicObjects;
}
GameObject* ShipScene::MakeScene()
{
	GameObject* pScene = new GameObject();

	const auto& gameContext = GetGameContext();
	auto pPhysx = PhysxManager::GetInstance()->GetPhysics();
	physx::PxMaterial* pPxMaterial = pPhysx->createMaterial(0, 0, 1);
	GameObject* pObject = nullptr;
	
	//Ground Plane
	//************
	//************
	pObject = Helpers::MakeGroundPlane(0, 0, 1);
	pScene->AddChild(pObject);

	//Scene
	//*****
	//*****

	//Model
	//*****
	pObject = new GameObject();
	ModelComponent* pModelComponent = new ModelComponent(L"./Resources/Models/Scene_Ship.ovm");
	UINT materialID = gameContext.pMaterialManager->GetFreeID();
	DiffuseMaterial_Shadow* pMaterial = new DiffuseMaterial_Shadow();
	pMaterial->SetTexture(L"./Resources/Textures/Diffuse/Scene_Ship_A.png");
	gameContext.pMaterialManager->AddMaterial(pMaterial, materialID);
	pModelComponent->SetMaterial(materialID);
	pObject->AddComponent(pModelComponent);
	pObject->GetTransform()->Rotate(0, 90, 0, true);
	pScene->AddChild(pObject);

	//Collision
	//*********
	pObject = new GameObject();
	const float height = 10.0f;
	const float depth = 4.0f;
	const float width = 60.0f;
	const float offsetZ = 34.0f;
	const float offsetX = 50.0f;
	
	pObject->AddComponent(new RigidBodyComponent(true));

	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxBoxGeometry(width, height, depth));
	pObject->AddComponent(new ColliderComponent(pGeometry, *pPxMaterial, physx::PxTransform{ physx::PxVec3{0, height*0.5f, offsetZ} })); //Top
	pObject->AddComponent(new ColliderComponent(pGeometry, *pPxMaterial, physx::PxTransform{ physx::PxVec3{0, height*0.5f, -offsetZ} })); //Bottom
	pObject->AddComponent(new ColliderComponent(pGeometry, *pPxMaterial, physx::PxTransform{ physx::PxVec3{-offsetX, height*0.5f, 0}, physx::PxQuat{DirectX::XMConvertToRadians(90.0f), physx::PxVec3{0, 1, 0}} })); //Left
	pObject->AddComponent(new ColliderComponent(pGeometry, *pPxMaterial, physx::PxTransform{ physx::PxVec3{offsetX, height*0.5f, 0}, physx::PxQuat{DirectX::XMConvertToRadians(90.0f), physx::PxVec3{0, 1, 0}} })); //Right
	pScene->AddChild(pObject);

	return pScene;
}
std::vector<RecipeData> ShipScene::MakeRecipeTemplates(const GameContext&)
{
	std::vector<RecipeData> recipeData{};
	{	//RECIPE #1
		//*********
		Recipe recipe{};
		recipe.foodStates[0] = eFoodState::Cut;
		recipe.foodStates[1] = eFoodState::Baked;
		recipe.foodTypes[0] = eFoodType::Broccoli;
		recipe.foodTypes[1] = eFoodType::Potato;
		recipe.Sort();
		recipeData.push_back({ recipe, 35, eMealType::Meal01 });
	}
	{	//RECIPE #2
		//*********
		Recipe recipe{};
		recipe.foodStates[0] = eFoodState::Cooked;
		recipe.foodStates[1] = eFoodState::Cooked;
		recipe.foodStates[2] = eFoodState::Cooked;
		recipe.foodTypes[0] = eFoodType::Apple;
		recipe.foodTypes[1] = eFoodType::Pepper;
		recipe.foodTypes[2] = eFoodType::Mushroom;
		recipe.Sort();
		recipeData.push_back({ recipe, 45, eMealType::Meal02 });
	}
	{	//RECIPE #3
		//*********
		Recipe recipe{};
		recipe.foodStates[0] = eFoodState::Baked;
		recipe.foodStates[1] = eFoodState::Cut;
		recipe.foodStates[2] = eFoodState::Baked;
		recipe.foodStates[3] = eFoodState::Cut;
		recipe.foodTypes[0] = eFoodType::Potato;
		recipe.foodTypes[1] = eFoodType::Apple;
		recipe.foodTypes[2] = eFoodType::Potato;
		recipe.foodTypes[3] = eFoodType::Pepper;
		recipe.Sort();
		recipeData.push_back({ recipe, 55, eMealType::Meal04 });
	}
	return recipeData;
}


