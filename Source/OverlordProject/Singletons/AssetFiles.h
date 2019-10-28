#pragma once
#include "../Templates/SingletonObject.h"

enum class eAssetId
{
	Empty,

	//Sprites
	//*******
	Sprite_Recipe_Background,
	Sprite_Recipe_FoodBackground,
	Sprite_Recipe_Timer,
	Sprite_Icons_Food,
	Sprite_Icons_Meals,
	Sprite_Icons_States,
	Sprite_Scoring_Coin,
	Sprite_Progress_Background,
	Sprite_Progress_Timer,
	Sprite_Progress_States,
	Sprite_Timer_Hourglass,

	//Fonts
	//*****
	Font_Game_Text,
	Font_Menu_ButtonText,
	Font_Menu_ScoreText,

	//Models
	//******
	Model_Default_Plane,
	Model_Food_Apple,
	Model_Food_Broccoli,
	Model_Food_Mushroom,
	Model_Food_Pepper,
	Model_Food_Potato,
	Model_Food_Liquid_One,
	Model_Food_Cut_One,
	Model_Food_Cut_Two,
	Model_Food_Cut_Three,

	Model_Holder_Boiler,
	Model_Holder_Pan,
	Model_Holder_Plate,

	Model_Box_ChoppingBox,
	Model_Box_Container,
	Model_Box_Fire,
	Model_Box_Bin,
	Model_Box_Delivery,
	Model_Box_Base,

	Model_Item_Extinguisher,

	//Textures
	//********
	Texture_Diffuse_Food_Apple,
	Texture_Diffuse_Food_Broccoli,
	Texture_Diffuse_Food_Mushroom,
	Texture_Diffuse_Food_Pepper,
	Texture_Diffuse_Food_Potato,
	Texture_Diffuse_Food_Liquid_One,

	Texture_Diffuse_Holder_Boiler,
	Texture_Diffuse_Holder_Pan,
	Texture_Diffuse_Holder_Plate,

	Texture_Diffuse_Box_ChoppingBox,
	Texture_Diffuse_Box_Container,
	Texture_Diffuse_Box_Fire,
	Texture_Diffuse_Box_Bin,
	Texture_Diffuse_Box_Delivery,
	Texture_Diffuse_Box_Base,

	Texture_Diffuse_Item_Extinguisher,

	//MENU
	//****
	Texture_Menu_Button,
	Texture_Menu_MainMenu,
	Texture_Menu_PauseMenu,
	Texture_Menu_ScoreMenu,
	Texture_Menu_ControlsMenu,

	//Colors
	//******
	Color_Mushroom_Brown,
	Color_Mushroom_DarkBrown,
	Color_Broccoli_Green,
	Color_Broccoli_DarkGreen,
	Color_Potato_Yellow,
	Color_Potato_DarkYellow,
	Color_Apple_Red,
	Color_Apple_DarkRed,
	Color_Pepper_Red,
	Color_Pepper_DarkRed,



	//Animated
	//********
	Animated_Character,


	//Particles
	//*********
	Particle_Smoke,
	Particle_Dust,

	END
};


class AssetFiles final : public SingletonObject<AssetFiles>
{
public:
	AssetFiles() = default;
	virtual ~AssetFiles() = default;

	bool Add(eAssetId key, const std::wstring& assetFile);
	bool Get(eAssetId key, std::wstring& assetFile) const;

	void SetDefault(const std::wstring& default) { m_Default = default; }
	const std::wstring& GetDefault() const { return m_Default; }
	void SetBasePath(const std::wstring& basepath) { m_BasePath = basepath; }
	const std::wstring& GetBasePath() const { return m_BasePath; }
private:
	std::map<eAssetId, std::wstring> m_AssetFiles;
	std::wstring m_BasePath = L"";
	std::wstring m_Default = L"";
};

