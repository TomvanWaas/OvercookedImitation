#include "stdafx.h"
#include "LoadgameScene.h"
#include "SceneManager.h"
#include "../Helpers/Helpers.h"
#include "../Structs/ModelInfo.h"
#include "../Components/Food/FoodInfo.h"
#include "../Singletons/AssetFiles.h"
#include "SoundManager.h"
#include "../Helpers/SoundID.h"

LoadgameScene::LoadgameScene()
	: GameScene(L"StartupScene")
{
}

GameScene* LoadgameScene::Clone() const
{
	return new LoadgameScene();
}

void LoadgameScene::Initialize()
{
	//Only allowed Once!
	static bool isInitialized = false;
	if (!isInitialized)
	{	isInitialized = true;

		//ASSETFILES
		//**********
		Helpers::AddAssetFiles();

		//SOUNDFILES
		//**********
		SoundManager* pSound = SoundManager::GetInstance();
		pSound->LoadSound({
			{int(eSoundID::Main01Looped), SoundData{"./Resources/Sound/Song01_Loop.wav", FMOD_LOOP_NORMAL}},
			{int(eSoundID::Main02Looped), SoundData{"./Resources/Sound/Song02_Loop.wav", FMOD_LOOP_NORMAL}},
		
			{int(eSoundID::FireLooped), SoundData{"./Resources/Sound/EffectFire_Loop.wav", FMOD_LOOP_NORMAL}},
			{int(eSoundID::DropSingle), SoundData{"./Resources/Sound/EffectDrop_Single.wav", FMOD_DEFAULT}},
			{int(eSoundID::AlarmSingle), SoundData{"./Resources/Sound/EffectAlarm_Single.wav", FMOD_DEFAULT}},
			{int(eSoundID::ChopSingle), SoundData{"./Resources/Sound/EffectChop_Single.wav", FMOD_DEFAULT}},
			{int(eSoundID::TakeSingle), SoundData{"./Resources/Sound/EffectTake_Single.wav", FMOD_DEFAULT}}

		});

		//TEMPLATES & DATA
		//****************
		MakeTemplatesFood();
		MakeTemplatesHolder();
	}
}
void LoadgameScene::Update()
{
	SceneManager::GetInstance()->SetActiveGameScene(L"MenuScene");
}
void LoadgameScene::Draw()
{
}

void LoadgameScene::MakeTemplatesFood()
{
	//MUSHROOM :: Base, Cut, Liquid, Cooked
	//********
	{
		FoodInfo info{ eFoodType::Mushroom,
		{
			{eFoodState::Base, {FoodConversion{1.5f, eUpdate::Cut, eFoodState::Cut}}},
			{eFoodState::Liquid, {FoodConversion{3.5f, eUpdate::Fire, eFoodState::Cooked}}}
		},
		{
			{eFoodState::Base, ModelInfo{eAssetId::Model_Food_Mushroom, eAssetId::Texture_Diffuse_Food_Mushroom}},
			{eFoodState::Cut, ModelInfo{eAssetId::Model_Food_Cut_One, eAssetId::Color_Mushroom_Brown}},
			{eFoodState::Liquid, ModelInfo{eAssetId::Model_Food_Liquid_One, eAssetId::Color_Mushroom_Brown}},
			{eFoodState::Cooked, ModelInfo{eAssetId::Model_Food_Liquid_One, eAssetId::Color_Mushroom_DarkBrown}},
			{eFoodState::Burned, ModelInfo{eAssetId::Empty, eAssetId::Color_Mushroom_DarkBrown}}
		} };
		Helpers::MakeTemplateFood(info);
	}

	//PEPPER :: Base, Cut, Liquid, Cooked
	//******
	{
		FoodInfo info{ eFoodType::Pepper,
		{
			{eFoodState::Base, {FoodConversion{1.5f, eUpdate::Cut, eFoodState::Cut}}},
			{eFoodState::Liquid, {FoodConversion{3.5f, eUpdate::Fire, eFoodState::Cooked}}}
		},
		{
			{eFoodState::Base, ModelInfo{eAssetId::Model_Food_Pepper, eAssetId::Texture_Diffuse_Food_Pepper}},
			{eFoodState::Cut, ModelInfo{eAssetId::Model_Food_Cut_Two, eAssetId::Color_Pepper_Red}},
			{eFoodState::Liquid, ModelInfo{eAssetId::Model_Food_Liquid_One, eAssetId::Color_Pepper_Red}},
			{eFoodState::Cooked, ModelInfo{eAssetId::Model_Food_Liquid_One, eAssetId::Color_Pepper_DarkRed}},
			{eFoodState::Burned, ModelInfo{eAssetId::Empty, eAssetId::Color_Pepper_DarkRed}}
		} };
		Helpers::MakeTemplateFood(info);
	}

	//POTATO :: Base, Cut, Baked
	//******
	{
		FoodInfo info{ eFoodType::Potato,
		{
			{eFoodState::Base, {FoodConversion{1.5f, eUpdate::Cut, eFoodState::Cut}}},
			{eFoodState::Cut, {FoodConversion{3.5f, eUpdate::Fire, eFoodState::Baked}}}
		},
		{
			{eFoodState::Base, ModelInfo{eAssetId::Model_Food_Potato, eAssetId::Texture_Diffuse_Food_Potato}},
			{eFoodState::Cut, ModelInfo{eAssetId::Model_Food_Cut_Three, eAssetId::Color_Potato_Yellow}},
			{eFoodState::Baked, ModelInfo{eAssetId::Model_Food_Cut_Three, eAssetId::Color_Potato_DarkYellow}},
			{eFoodState::Burned, ModelInfo{eAssetId::Empty, eAssetId::Color_Potato_DarkYellow}}
		} };
		Helpers::MakeTemplateFood(info);
	}

	//BROCCOLI :: Base, Cut
	//********
	{
		FoodInfo info{ eFoodType::Broccoli,
		{
			{eFoodState::Base, {FoodConversion{1.5f, eUpdate::Cut, eFoodState::Cut}}}
		},
		{
			{eFoodState::Base, ModelInfo{eAssetId::Model_Food_Broccoli, eAssetId::Texture_Diffuse_Food_Broccoli}},
			{eFoodState::Cut, ModelInfo{eAssetId::Model_Food_Cut_Two, eAssetId::Color_Broccoli_Green}},
			{eFoodState::Burned, ModelInfo{eAssetId::Empty, eAssetId::Color_Broccoli_DarkGreen}}
		} };
		Helpers::MakeTemplateFood(info);
	}

	//APPLE :: Base, Cut, Liquid, Cooked
	//*****
	{
		FoodInfo info{ eFoodType::Apple,
		{
			{eFoodState::Base, {FoodConversion{1.5f, eUpdate::Cut, eFoodState::Cut}}},
			{eFoodState::Liquid, {FoodConversion{3.5f, eUpdate::Fire, eFoodState::Cooked}}}
		},
		{
			{eFoodState::Base, ModelInfo{eAssetId::Model_Food_Apple, eAssetId::Texture_Diffuse_Food_Apple}},
			{eFoodState::Cut, ModelInfo{eAssetId::Model_Food_Cut_One, eAssetId::Color_Apple_Red}},
			{eFoodState::Liquid, ModelInfo{eAssetId::Model_Food_Liquid_One, eAssetId::Color_Apple_Red}},
			{eFoodState::Cooked, ModelInfo{eAssetId::Model_Food_Liquid_One, eAssetId::Color_Apple_DarkRed}},
			{eFoodState::Burned, ModelInfo{eAssetId::Empty, eAssetId::Color_Apple_DarkRed}}
		} };
		Helpers::MakeTemplateFood(info);
	}

}
void LoadgameScene::MakeTemplatesHolder()
{
	Helpers::MakeTemplateHolders(
		{	//EMPTY
			//*****
			{eFoodType::Mushroom, {{eFoodState::Base, eFoodState::Base}, {eFoodState::Cut, eFoodState::Cut}}},
			{eFoodType::Pepper, {{eFoodState::Base, eFoodState::Base}, {eFoodState::Cut, eFoodState::Cut}}},
			{eFoodType::Potato, {{eFoodState::Base, eFoodState::Base}, {eFoodState::Cut, eFoodState::Cut}}},
			{eFoodType::Broccoli, {{eFoodState::Base, eFoodState::Base}, {eFoodState::Cut, eFoodState::Cut}}},
			{eFoodType::Apple, {{eFoodState::Base, eFoodState::Base}, {eFoodState::Cut, eFoodState::Cut}}}

		},
		{	//PLATE
			//*****
			{eFoodType::Mushroom, {{eFoodState::Cooked, eFoodState::Cooked}}},
			{eFoodType::Potato, {{eFoodState::Baked, eFoodState::Baked}}},
			{eFoodType::Pepper, {{eFoodState::Cut, eFoodState::Cut}, {eFoodState::Cooked, eFoodState::Cooked}}},
			{eFoodType::Apple, {{eFoodState::Cut, eFoodState::Cut}, {eFoodState::Cooked, eFoodState::Cooked}}},
			{eFoodType::Broccoli, {{eFoodState::Cut, eFoodState::Cut}}}

		},
		{	//BOILER
			//******
			{eFoodType::Mushroom, {{eFoodState::Cut, eFoodState::Liquid}}},
			{eFoodType::Pepper, {{eFoodState::Cut, eFoodState::Liquid}}},
			{eFoodType::Apple, {{eFoodState::Cut, eFoodState::Liquid}}}
		},
		{	//PAN
			//***
			{eFoodType::Potato, {{eFoodState::Cut, eFoodState::Cut}}}
		}
		);
}


