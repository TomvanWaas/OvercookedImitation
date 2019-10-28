#include "stdafx.h"
#include "Helpers.h"
#include "../Singletons/AssetFiles.h"
#include "../Prototype/PrototypeObject.h"
#include "../Singletons/Templates.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"
#include "../Components/Character/CharacterComponent.h"
#include "PhysxManager.h"
#include "PhysxProxy.h"
#include "GameScene.h"
#include "../Components/Other/ScoreManagerComponent.h"
#include "../Components/Recipe/RecipeManagerComponent.h"
#include "../Components/Box/BoxComponentPassiveUpdator.h"
#include "../Components/Box/BoxComponentDeletor.h"
#include "../Components/Box/BoxComponentActiveUpdator.h"
#include "../Components/Box/BoxComponentFoodContainer.h"
#include "../Components/Box/BoxComponentHolderContainer.h"
#include "../Components/Holder/HolderInfo.h"
#include "../Singletons/StaticInfo.h"
#include "Destroyer.h"
#include "../Components/Holder/HolderComponent.h"
#include "../Components/Holder/HolderInterfaceComponent.h"
#include "../Components/Food/FoodInterfaceComponent.h"
#include "../Components/Holder/HolderIconsComponent.h"
#include "../Components/Holder/HolderProgressComponent.h"
#include "../Components/Food/FoodComponent.h"
#include "TransformComponent.h"
#include "../Components/Camera/TopDownCameraComponent.h"
#include "../Components/Other/FireComponent.h"
#include "../Components/Holder/HolderBoxNotifier.h"
#include "../Components/Item/ExtinguisherComponent.h"
#include "../Components/Other/TimerComponent.h"
#include "SceneManager.h"
#include "../Components/Camera/CameraLightComponent.h"
#include "../Components/Camera/CameraEventComponent.h"
#include "ModelComponent.h"

void Helpers::AddAssetFiles()
{
	auto& assets = AssetFiles::GetInstance();
	assets.SetBasePath(L"./Resources/");
	assets.SetDefault(L"Textures/Default.png");

	//Sprites
	//*******
	assets.Add(eAssetId::Sprite_Recipe_Background, L"Textures/Sprites/Recipes_Background.png");
	assets.Add(eAssetId::Sprite_Recipe_FoodBackground, L"Textures/Sprites/Recipes_FoodBackground.png");
	assets.Add(eAssetId::Sprite_Recipe_Timer, L"Textures/Sprites/Recipes_Timer.png");
	assets.Add(eAssetId::Sprite_Icons_Food, L"Textures/Sprites/Icons_Food.png");
	assets.Add(eAssetId::Sprite_Icons_Meals, L"Textures/Sprites/Icons_Meal.png");
	assets.Add(eAssetId::Sprite_Icons_States, L"Textures/Sprites/Icons_State.png");
	assets.Add(eAssetId::Sprite_Scoring_Coin, L"Textures/Sprites/Sprite_Coin.png");
	assets.Add(eAssetId::Sprite_Progress_Background, L"Textures/Sprites/Progress_Background.png");
	assets.Add(eAssetId::Sprite_Progress_Timer, L"Textures/Sprites/Progress_Timer.png");
	assets.Add(eAssetId::Sprite_Progress_States, L"Textures/Sprites/Progress_States.png");
	assets.Add(eAssetId::Sprite_Timer_Hourglass, L"Textures/Sprites/Sprite_Hourglass.png");

	//Fonts
	//*****
	assets.Add(eAssetId::Font_Game_Text, L"Fonts/Rockwell_48.fnt");
	assets.Add(eAssetId::Font_Menu_ButtonText, L"Fonts/Rockwell_48.fnt");
	assets.Add(eAssetId::Font_Menu_ScoreText, L"Fonts/Rockwell_48.fnt");
	//assets.Add(eAssetId::Font_Menu_ButtonText, L"Fonts/Consolas_32.fnt");

	
	
	//Models
	//******
	assets.Add(eAssetId::Model_Default_Plane, L"Models/Default_Plane.ovm");

	assets.Add(eAssetId::Model_Food_Apple, L"Models/Food_Apple.ovm");
	assets.Add(eAssetId::Model_Food_Broccoli, L"Models/Food_Broccoli.ovm");
	assets.Add(eAssetId::Model_Food_Mushroom, L"Models/Food_Mushroom.ovm");
	assets.Add(eAssetId::Model_Food_Pepper, L"Models/Food_Pepper.ovm");
	assets.Add(eAssetId::Model_Food_Potato, L"Models/Food_Potato.ovm");
	assets.Add(eAssetId::Model_Food_Liquid_One, L"Models/Food_Liquid_01.ovm");
	assets.Add(eAssetId::Model_Food_Cut_One, L"Models/Food_Cut_01.ovm");
	assets.Add(eAssetId::Model_Food_Cut_Two, L"Models/Food_Cut_02.ovm");
	assets.Add(eAssetId::Model_Food_Cut_Three, L"Models/Food_Cut_03.ovm");

	assets.Add(eAssetId::Model_Holder_Boiler, L"Models/Holder_Boiler.ovm");
	assets.Add(eAssetId::Model_Holder_Pan, L"Models/Holder_Pan.ovm");
	assets.Add(eAssetId::Model_Holder_Plate, L"Models/Holder_Plate.ovm");

	assets.Add(eAssetId::Model_Box_ChoppingBox, L"Models/Box_ChoppingBox.ovm");
	assets.Add(eAssetId::Model_Box_Container, L"Models/Box_Container.ovm");
	assets.Add(eAssetId::Model_Box_Fire, L"Models/Box_Fire.ovm");
	assets.Add(eAssetId::Model_Box_Bin, L"Models/Box_Bin.ovm");
	assets.Add(eAssetId::Model_Box_Delivery, L"Models/Box_Delivery.ovm");
	assets.Add(eAssetId::Model_Box_Base, L"Models/Box_Base.ovm");

	assets.Add(eAssetId::Model_Item_Extinguisher, L"Models/Item_Extinguisher.ovm");

	//Textures
	//********
	assets.Add(eAssetId::Texture_Diffuse_Food_Apple, L"Textures/Diffuse/Food_Apple_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Food_Broccoli, L"Textures/Diffuse/Food_Broccoli_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Food_Mushroom, L"Textures/Diffuse/Food_Mushroom_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Food_Pepper, L"Textures/Diffuse/Food_Pepper_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Food_Potato, L"Textures/Diffuse/Food_Potato_A.png");

	assets.Add(eAssetId::Texture_Diffuse_Food_Liquid_One, L"Textures/Diffuse/Food_Liquid_01_A.png");

	assets.Add(eAssetId::Texture_Diffuse_Holder_Boiler, L"Textures/Diffuse/Holder_Boiler_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Holder_Pan, L"Textures/Diffuse/Holder_Pan_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Holder_Plate, L"Textures/Diffuse/Holder_Plate_A.png");

	assets.Add(eAssetId::Texture_Diffuse_Box_ChoppingBox, L"Textures/Diffuse/Box_ChoppingBox_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Box_Container, L"Textures/Diffuse/Box_Container_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Box_Fire, L"Textures/Diffuse/Box_Fire_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Box_Bin, L"Textures/Diffuse/Box_Bin_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Box_Delivery, L"Textures/Diffuse/Box_Delivery_A.png");
	assets.Add(eAssetId::Texture_Diffuse_Box_Base, L"Textures/Diffuse/Box_Base_A.png");

	assets.Add(eAssetId::Texture_Diffuse_Item_Extinguisher, L"Textures/Diffuse/Item_Extinguisher_A.png");

	//Colors
	//******
	assets.Add(eAssetId::Color_Mushroom_Brown, L"Textures/Color/Color_Mushroom_Brown.png");
	assets.Add(eAssetId::Color_Mushroom_DarkBrown, L"Textures/Color/Color_Mushroom_DarkBrown.png");
	assets.Add(eAssetId::Color_Broccoli_Green, L"Textures/Color/Color_Broccoli_Green.png");
	assets.Add(eAssetId::Color_Broccoli_DarkGreen, L"Textures/Color/Color_Broccoli_DarkGreen.png");
	assets.Add(eAssetId::Color_Apple_Red, L"Textures/Color/Color_Apple_Red.png");
	assets.Add(eAssetId::Color_Apple_DarkRed, L"Textures/Color/Color_Apple_DarkRed.png");
	assets.Add(eAssetId::Color_Pepper_Red, L"Textures/Color/Color_Pepper_Red.png");
	assets.Add(eAssetId::Color_Pepper_DarkRed, L"Textures/Color/Color_Pepper_DarkRed.png");
	assets.Add(eAssetId::Color_Potato_Yellow, L"Textures/Color/Color_Potato_Yellow.png");
	assets.Add(eAssetId::Color_Potato_DarkYellow, L"Textures/Color/Color_Potato_DarkYellow.png");

	//Animated
	//********
	assets.Add(eAssetId::Animated_Character, L"Models/Animated/Animated_Character.ovm");


	//Particles
	//*********
	assets.Add(eAssetId::Particle_Smoke, L"Textures/Particles/Particle_Smoke.png");
	assets.Add(eAssetId::Particle_Dust, L"Textures/Particles/Particle_Dust.png");

	//Menu
	//****
	assets.Add(eAssetId::Texture_Menu_Button, L"Textures/Menu/Menu_Button.png");
	assets.Add(eAssetId::Texture_Menu_MainMenu, L"Textures/Menu/Menu_MainMenu.png");
	assets.Add(eAssetId::Texture_Menu_PauseMenu, L"Textures/Menu/Menu_PauseMenu.png");
	assets.Add(eAssetId::Texture_Menu_ScoreMenu, L"Textures/Menu/Menu_ScoreMenu.png");
	assets.Add(eAssetId::Texture_Menu_ControlsMenu, L"Textures/Menu/Menu_ControlsMenu.png");
}

void Helpers::MakeTemplateHolders(const std::map<eFoodType, std::map<eFoodState, eFoodState>>& emptyContainments,
	const std::map<eFoodType, std::map<eFoodState, eFoodState>>& plateContainments,
	const std::map<eFoodType, std::map<eFoodState, eFoodState>>& boilerContainments,
	const std::map<eFoodType, std::map<eFoodState, eFoodState>>& panContainments)
{
	//EMPTY
	//*****
	{
		HolderInfo info{ eHolderType::Empty, ModelInfo{eAssetId::Empty, eAssetId::Empty},
		{emptyContainments},
		1,false, {0, 0, 0}, {0, 1,0}, {0, 1} , {0, -1}};
		Helpers::MakeTemplateHolder(info);
	}
	//PLATE
	//*****
	{
		HolderInfo info{ eHolderType::Plate, ModelInfo{eAssetId::Model_Holder_Plate, eAssetId::Texture_Diffuse_Holder_Plate},
		{plateContainments},
		4, false,{0, 0.25f, 0}, {0,1,0}, {0, 1}, {0, -1} };
		Helpers::MakeTemplateHolder(info);
	}
	//BOILER
	//******
	{
		HolderInfo info{ eHolderType::Boiler, ModelInfo{eAssetId::Model_Holder_Boiler, eAssetId::Texture_Diffuse_Holder_Boiler},
		{boilerContainments},
		4,true, {0, 1, 0}, {0, 1, 0}, {0, 1.5f} , {0, -1}};
		Helpers::MakeTemplateHolder(info);
	}
	//PAN
	//***
	{
		HolderInfo info{ eHolderType::Pan, ModelInfo{eAssetId::Model_Holder_Pan, eAssetId::Texture_Diffuse_Holder_Pan},
		{panContainments},
		4, true, {0, 0.25f, 0}, {0, 1, 0}, {0, 1} , {0, -1}};
		Helpers::MakeTemplateHolder(info);
	}
}
void Helpers::MakeTemplateHolder(const HolderInfo& info)
{
	//Make Static Copy of Info
	//************************
	StaticInfo::GetInstance().AddInfo(info);
	const HolderInfo& cinfo = StaticInfo::GetInstance().GetInfo(info.type);

	//Make Template
	//*************
	PrototypeObject* pObject = new PrototypeObject();
	auto* pInterface = new HolderInterfaceComponent(cinfo);
	auto* pHolder = new HolderComponent(cinfo);
	auto* pIcons = new HolderIconsComponent(cinfo, cinfo.maxIndex, [](eFoodType type, eFoodState state)
	{
		return type != eFoodType::_None && state != eFoodState::Base;
	});
	auto* pProgress = new HolderProgressComponent(cinfo);
	auto* pNotifier = new HolderBoxNotifier();

	pObject->AddComponent(pHolder);
	pObject->AddComponent(pIcons);
	pObject->AddComponent(pProgress);
	pObject->AddComponent(pInterface);
	pObject->AddComponent(pNotifier);

	pObject->AddObserver(pHolder);
	pObject->AddObserver(pIcons);
	pObject->AddObserver(pProgress);
	pObject->AddObserver(pInterface);
	pObject->AddObserver(pNotifier);

	Templates::GetInstance().Add(info.type, pObject);
}
void Helpers::MakeTemplateFood(const FoodInfo& info)
{
	const float scale = 1.0f;

	//Make Static Copy of Info
	//************************
	StaticInfo::GetInstance().AddInfo(info);
	const FoodInfo& cinfo = StaticInfo::GetInstance().GetInfo(info.type);

	//Make Template
	//*************
	PrototypeObject* pObject = new PrototypeObject();
	auto* pInterface = new FoodInterfaceComponent(cinfo);
	auto* pFood = new FoodComponent(cinfo, eFoodState::Base);
	pObject->AddComponent(pFood);
	pObject->AddComponent(pInterface);
	pObject->AddObserver(pFood);
	pObject->AddObserver(pInterface);

	pObject->GetTransform()->Scale(scale, scale, scale);
	Templates::GetInstance().Add(info.type, pObject);
}



GameObject* Helpers::MakeBoxFire(const GameContext&, physx::PxMaterial* pMaterial, bool staticRigid)
{
	const float size = 3.0f;

	GameObject* pObject = new GameObject();

	//Box
	//***
	BoxComponentPassiveUpdator* pBoxComp = new BoxComponentPassiveUpdator(eBoxType::Fire, {0,(2.15f)*size, 0}, 
		{ eHolderType::Boiler, eHolderType::Pan }, 
		ModelInfo{ eAssetId::Model_Box_Fire, eAssetId::Texture_Diffuse_Box_Fire }, 
		eUpdate::Fire, 1.0f);
	pObject->AddComponent(pBoxComp);

	//Rigid
	//*****
	RigidBodyComponent* pRigidComponent = new RigidBodyComponent(staticRigid);
	pObject->AddComponent(pRigidComponent);
	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxBoxGeometry(size, size, size));
	pObject->AddComponent(new ColliderComponent(pGeometry, *pMaterial, physx::PxTransform(physx::PxVec3{ 0, size, 0 })));
	if (!staticRigid) pRigidComponent->SetKinematic(true);

	//Fire
	auto* pFire = new FireComponent(L"./Resources/Textures/Particles/Particle_Fire.png", 8.0f, 2.01f * size, 1);
	pObject->AddComponent(pFire);
	pObject->AddObserver(pFire);
	pObject->AddObserver(pBoxComp);

	return pObject;
}
GameObject* Helpers::MakeBoxDelivery(const GameContext&, physx::PxMaterial* pMaterial, RecipeManagerComponent* pRecipeManager, bool staticRigid)
{
	const float size = 3.0f;

	GameObject* pObject = new GameObject();

	//Box
	//***
	BoxComponentDeletor* pBoxComp = new BoxComponentDeletor(eBoxType::Delivery,
		{0, 2*size, 0}, {eHolderType::Plate}, ModelInfo{eAssetId::Model_Box_Delivery, eAssetId::Texture_Diffuse_Box_Delivery},
		{{eHolderType::Plate, BoxComponentDeletor::eDeleteSpecifier::All}}, {0, 2*size, 0.5f*size}, {0, 2*size, -0.5f*size}, 
		[pRecipeManager](const HolderPointer& holder)
	{
		if (holder.IsValid() && holder.Get()->GetHolderComponent()) pRecipeManager->Deliver(holder.Get()->GetHolderComponent()->GetRecipe());
	});
	pObject->AddComponent(pBoxComp);

	//Rigid
	//*****
	RigidBodyComponent* pRigidComponent = new RigidBodyComponent(staticRigid);
	pObject->AddComponent(pRigidComponent);
	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxBoxGeometry(2 * size, size, size));
	pObject->AddComponent(new ColliderComponent(pGeometry, *pMaterial, physx::PxTransform{ physx::PxVec3{0, size, 0} }));
	if (!staticRigid) pRigidComponent->SetKinematic(true);

	//Fire
	auto* pFire = new FireComponent(L"./Resources/Textures/Particles/Particle_Fire.png", 8.0f, 2.01f * size, 1);
	pObject->AddComponent(pFire);
	pObject->AddObserver(pFire);
	pObject->AddObserver(pBoxComp);

	return pObject;
}
GameObject* Helpers::MakeBoxBase(const GameContext&, physx::PxMaterial* pMaterial, bool staticRigid)
{
	const float size = 3.0f;

	GameObject* pObject = new GameObject();

	//Box
	//***
	BoxComponentBase* pBoxComp = new BoxComponentBase(eBoxType::Base, { 0, 2 * size, 0 },
		{ eHolderType::Boiler, eHolderType::Empty, eHolderType::Pan, eHolderType::Plate },
		ModelInfo{ eAssetId::Model_Box_Base, eAssetId::Texture_Diffuse_Box_Base });
	pObject->AddComponent(pBoxComp);

	//Rigid
	//*****
	RigidBodyComponent* pRigidComponent = new RigidBodyComponent(staticRigid);
	pObject->AddComponent(pRigidComponent);
	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxBoxGeometry(size, size, size));
	pObject->AddComponent(new ColliderComponent(pGeometry, *pMaterial, physx::PxTransform{ physx::PxVec3{0, size, 0} }));
	if (!staticRigid) pRigidComponent->SetKinematic(true);

	//Fire
	auto* pFire = new FireComponent(L"./Resources/Textures/Particles/Particle_Fire.png", 8.0f, 2.01f * size, 1);
	pObject->AddComponent(pFire);
	pObject->AddObserver(pFire);
	pObject->AddObserver(pBoxComp);

	return pObject;
}
GameObject* Helpers::MakeBoxChoppingBox(const GameContext&, physx::PxMaterial* pMaterial, bool staticRigid)
{
	const float size = 3.0f;
	const float delay = 0.35f;
	const float speed = 1.0f;

	GameObject* pObject = new GameObject();

	//Box
	//***
	BoxComponentActiveUpdator* pBoxComp = new BoxComponentActiveUpdator(eBoxType::ChoppingBox, { 0, 2.1f*size, 0 },
		{ eHolderType::Empty }, ModelInfo{ eAssetId::Model_Box_ChoppingBox, eAssetId::Texture_Diffuse_Box_ChoppingBox },
		eUpdate::Cut, speed, delay, [](const HolderPointer& holder)
	{
		TransformComponent* pTransform = holder.Get()->GetTransform();
		pTransform->Rotate(0, randF(0, 180), 0, true);
		const DirectX::XMFLOAT3& scale = pTransform->GetScale();
		const float mul = 0.95f;
		pTransform->Scale(scale.x * mul, scale.y * mul, scale.z * mul);
	},
		[](const HolderPointer& holder)
	{
		TransformComponent* pTransform = holder.Get()->GetTransform();
		pTransform->Scale(1, 1, 1);
	});
	pObject->AddComponent(pBoxComp);

	//Rigid
	//*****
	RigidBodyComponent* pRigidComponent = new RigidBodyComponent(staticRigid);
	pObject->AddComponent(pRigidComponent);
	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxBoxGeometry(size, size, size));
	pObject->AddComponent(new ColliderComponent(pGeometry, *pMaterial, physx::PxTransform{ physx::PxVec3{0, size, 0} }));
	if (!staticRigid) pRigidComponent->SetKinematic(true);

	//Fire
	auto* pFire = new FireComponent(L"./Resources/Textures/Particles/Particle_Fire.png", 8.0f, 2.01f * size, 1);
	pObject->AddComponent(pFire);
	pObject->AddObserver(pFire);
	pObject->AddObserver(pBoxComp);

	return pObject;
}
GameObject* Helpers::MakeBoxFoodContainer(const GameContext&, physx::PxMaterial* pMaterial, eFoodType food, bool staticRigid)
{
	const float size = 3.0f;

	GameObject* pObject = new GameObject();

	//Box
	//***
	BoxComponentFoodContainer* pBoxComp = new BoxComponentFoodContainer(eBoxType::FoodContainer, {0, 2*size, 0}, 
		{eHolderType::Boiler, eHolderType::Empty, eHolderType::Pan, eHolderType::Plate},
		ModelInfo{eAssetId::Model_Box_Container, eAssetId::Texture_Diffuse_Box_Container},
		food, [](const HolderPointer& holder)
	{
		return holder.IsEmpty();
	});
	pObject->AddComponent(pBoxComp);

	//Rigid
	//*****
	RigidBodyComponent* pRigidComponent = new RigidBodyComponent(staticRigid);
	pObject->AddComponent(pRigidComponent);
	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxBoxGeometry(size, size, size));
	pObject->AddComponent(new ColliderComponent(pGeometry, *pMaterial, physx::PxTransform{ physx::PxVec3{0, size, 0} }));
	if (!staticRigid) pRigidComponent->SetKinematic(true);

	//Fire
	auto* pFire = new FireComponent(L"./Resources/Textures/Particles/Particle_Fire.png", 8.0f, 2.01f * size, 1);
	pObject->AddComponent(pFire);
	pObject->AddObserver(pFire);
	pObject->AddObserver(pBoxComp);

	return pObject;
}
GameObject* Helpers::MakeBoxHolderContainer(const GameContext&, physx::PxMaterial* pMaterial,
	eHolderType holder, bool staticRigid)
{
	const float size = 3.0f;

	GameObject* pObject = new GameObject();

	//Box
	//***
	BoxComponentHolderContainer* pBoxComp = new BoxComponentHolderContainer(eBoxType::HolderContainer, { 0, 2 * size, 0 },
		{}, ModelInfo{ eAssetId::Model_Box_Container, eAssetId::Texture_Diffuse_Box_Container },
		holder, -1);
	pObject->AddComponent(pBoxComp);

	//Rigid
	//*****
	RigidBodyComponent* pRigidComponent = new RigidBodyComponent(staticRigid);
	pObject->AddComponent(pRigidComponent);
	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxBoxGeometry(size, size, size));
	pObject->AddComponent(new ColliderComponent(pGeometry, *pMaterial, physx::PxTransform{ physx::PxVec3{0, size, 0} }));
	if (!staticRigid) pRigidComponent->SetKinematic(true);

	//Fire
	auto* pFire = new FireComponent(L"./Resources/Textures/Particles/Particle_Fire.png", 8.0f, 2.01f * size, 1);
	pObject->AddComponent(pFire);
	pObject->AddObserver(pFire);
	pObject->AddObserver(pBoxComp);

	return pObject;
}
GameObject* Helpers::MakeBoxBin(const GameContext&, physx::PxMaterial* pMaterial, bool staticRigid)
{
	const float size = 3.0f;

	GameObject* pObject = new GameObject();

	//Box
	//***
	BoxComponentDeletor* pBoxComp = new BoxComponentDeletor(eBoxType::Bin, {}, {}, 
		ModelInfo{ eAssetId::Model_Box_Bin, eAssetId::Texture_Diffuse_Box_Bin },
		{ {eHolderType::Boiler, BoxComponentDeletor::eDeleteSpecifier::Food},
		{eHolderType::Empty, BoxComponentDeletor::eDeleteSpecifier::Food},
		{eHolderType::Pan, BoxComponentDeletor::eDeleteSpecifier::Food},
		{eHolderType::Plate, BoxComponentDeletor::eDeleteSpecifier::Food} },
		{ 0, 2 * size, 0 }, { 0,0,0 }, nullptr);
	pObject->AddComponent(pBoxComp);

	//Rigid
	//*****
	RigidBodyComponent* pRigidComponent = new RigidBodyComponent(staticRigid);
	pObject->AddComponent(pRigidComponent);
	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxBoxGeometry(size, size, size));
	pObject->AddComponent(new ColliderComponent(pGeometry, *pMaterial, physx::PxTransform{ physx::PxVec3{0, size, 0} }));
	if (!staticRigid) pRigidComponent->SetKinematic(true);

	//Fire
	auto* pFire = new FireComponent(L"./Resources/Textures/Particles/Particle_Fire.png", 8.0f, 2.01f * size, 1);
	pObject->AddComponent(pFire);
	pObject->AddObserver(pFire);
	pObject->AddObserver(pBoxComp);

	return pObject;
}



GameObject* Helpers::AddHolder(BoxComponentBase* pBoxComponent, eHolderType holder)
{
	if (pBoxComponent)
	{
		PrototypeObject* pClone = Templates::GetInstance().GetClone(holder);
		HolderInterfaceComponent* pHolderInterface = (pClone) ? pClone->GetDynamicComponent<HolderInterfaceComponent>() : nullptr;
		if (pHolderInterface && pBoxComponent->InitializeHolder(pHolderInterface))
		{
			return pClone;
		}
		if (pClone) Destroyer::GetInstance().Destroy(pClone);
	}
	return nullptr;
}
GameObject* Helpers::AddHolder(HolderPointer& holder, eHolderType type)
{
	auto* pClone = Templates::GetInstance().GetClone(type);
	HolderInterfaceComponent* pHolderInterface = (pClone) ? pClone->GetDynamicComponent<HolderInterfaceComponent>() : nullptr;
	if (pHolderInterface)
	{
		auto* pPrevious = holder.Set(pHolderInterface);
		if (pPrevious) Destroyer::GetInstance().Destroy(pPrevious);
		return pClone;
	}
	if (pClone) Destroyer::GetInstance().Destroy(pClone);
	return nullptr;
}
GameObject* Helpers::AddFood(HolderInterfaceComponent* pHolderInterface, eFoodType food)
{
	if (pHolderInterface)
	{
		PrototypeObject* pClone = Templates::GetInstance().GetClone(food);
		FoodInterfaceComponent* pFoodInterface = (pClone) ? pClone->GetDynamicComponent<FoodInterfaceComponent>() : nullptr;
		if (pFoodInterface && pHolderInterface->AddFood(*pFoodInterface))
		{
			return pClone;
		}
		if (pClone) Destroyer::GetInstance().Destroy(pClone);
	}
	return nullptr;
}
GameObject* Helpers::AddFood(HolderPointer& holder, eFoodType food)
{
	if (holder.IsValid()) return AddFood(holder.Get(), food);
	return nullptr;
}
bool Helpers::AddItem(ItemPointer& itemPtr, GameObject* pItem)
{
	if (itemPtr.IsValid()) return false;
	itemPtr.Set(pItem->GetDynamicComponent<ItemInterfaceComponent>());
	return true;
}
bool Helpers::AddItem(BoxComponentBase* pBoxComponent, GameObject* pItem)
{
	return pBoxComponent->InitializeItem(pItem->GetDynamicComponent<ItemInterfaceComponent>());
}

GameObject* Helpers::MakeItemExtinguisher(const GameContext& gameContext)
{
	GameObject* pObject = new GameObject();

	//Comp
	auto* pItemComp = new ExtinguisherComponent(15, 70, 2.5f);
	pObject->AddComponent(pItemComp);
	pObject->AddObserver(pItemComp);

	//Model
	ModelInfo modelInfo{ eAssetId::Model_Item_Extinguisher, eAssetId::Texture_Diffuse_Item_Extinguisher };
	auto modelPair = modelInfo.MakeDiffuseModel(gameContext);
	if (modelPair.first) pObject->AddComponent(modelPair.first);

	return pObject;
}


GameObject* Helpers::MakeScoreManager(const GameContext&)
{
	GameObject* pObject = new GameObject();
	auto* pScoreManager = new ScoreManagerComponent();
	pObject->AddComponent(pScoreManager);	
	return pObject;
}
GameObject* Helpers::MakeRecipeManager(const GameContext&, const CountData& count, const std::vector<RecipeData>& recipes)
{
	auto* pObject = new GameObject();
	auto* pRecipeManager = new RecipeManagerComponent(count, recipes, { 5000,0,0 });
	pObject->AddComponent(pRecipeManager);
	return pObject;
}
GameObject* Helpers::MakeTimeManager(const GameContext&, float time, const std::wstring& endscene)
{
	auto* pObject = new GameObject();
	auto* pTimeManager = new TimerComponent(time, [endscene]()
	{
		SceneManager::GetInstance()->SetActiveGameScene(endscene);
	});
	pObject->AddComponent(pTimeManager);
	return pObject;
}


GameObject* Helpers::MakeCharacter(const GameContext&, UINT playerID, float radius, float height, float movespeed, float rotatespeed, const DirectX::XMFLOAT4& color)
{
	//********
	GameObject* pObject = new GameObject();
	auto* pCharacterComponent = new CharacterComponent(GamepadIndex(playerID), color,
		radius, height, movespeed, rotatespeed, { 0, 0.8f * height, radius *1.4f });
	pObject->AddComponent(pCharacterComponent);
	return pObject;
}

GameObject* Helpers::MakeCamera(const GameContext&, GameScene* pScene)
{
	//CAMERA
	//******
	GameObject* pFixedCamera = new GameObject();
	pFixedCamera->AddComponent(new TopDownCameraComponent(MinMax<float>{0, 3}, 0.125f, 0.175f));
	CameraComponent* pCamera = new CameraComponent();
	pFixedCamera->AddComponent(pCamera);
	pFixedCamera->AddComponent(new CameraLightComponent({ 0, -1, -100 }, { 0, -5, 0 }));
	pFixedCamera->AddComponent(new CameraEventComponent());

	pScene->AddChild(pFixedCamera);
	pScene->SetActiveCamera(pCamera);
	return pFixedCamera;
}

void Helpers::EnableDebugRendering(PhysxProxy* pProxy, bool enable)
{
	pProxy->EnablePhysxDebugRendering(enable);
}

GameObject* Helpers::MakeGroundPlane(float staticFriction, float dynamicFriction, float restitution)
{
	//GROUND PLANE
	//************
	auto* pPhysx = PhysxManager::GetInstance()->GetPhysics();
	auto* pMaterial = pPhysx->createMaterial(staticFriction, dynamicFriction, restitution);
	auto* pObject = new GameObject();

	pObject->AddComponent(new RigidBodyComponent(true));
	std::shared_ptr<physx::PxGeometry> pGeometry(new physx::PxPlaneGeometry());
	pObject->AddComponent(new ColliderComponent(pGeometry, *pMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));

	return pObject;
}

