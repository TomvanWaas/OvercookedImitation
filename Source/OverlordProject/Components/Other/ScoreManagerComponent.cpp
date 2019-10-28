#include "stdafx.h"
#include "ScoreManagerComponent.h"
#include "SpriteComponent.h"
#include "../../Singletons/AssetFiles.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ScoreTextComponent.h"
#include "TransformComponent.h"
#include "TextureData.h"
#include "../../Structs/Recipe.h"
#include "../../Helpers/Timer.h"

void ScoreManagerComponent::FailRecipe()
{
	//Loose 10 score when recipe fails (red)
	int diff = -10;
	AddScore(diff);
	++m_NrFailedRecipes;
}

void ScoreManagerComponent::SucceedRecipe(const Timer& t, const Recipe& r)
{
	//Just a function to calculate the score depending on the timer & recipe
	
	int score = 5 + int(t.GetTime() * 3.334f);
	score *= r.Size();

	//Add
	AddScore(score);
	++m_NrFinishedRecipes;
}

void ScoreManagerComponent::Update(const GameContext&)
{
}
void ScoreManagerComponent::Draw(const GameContext&)
{
}
void ScoreManagerComponent::Initialize(const GameContext&)
{
	//Add Text Comp && Sprite Comp
	auto* pObject = GetGameObject();
	std::wstring asset{};

	//Coin Sprite
	SpriteComponent* pCoinSprite = nullptr;
	if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Scoring_Coin, asset))
	{
		pCoinSprite = new SpriteComponent(asset);
		pObject->AddComponent(pCoinSprite);	
	}
	//Text
	if (AssetFiles::GetInstance().Get(eAssetId::Font_Game_Text, asset))
	{
		GameObject* pTextObject = new GameObject();
		if (m_Score < 0)
		{
			m_pScoreText = new TextComponent(asset, std::to_wstring(m_Score));
		}
		else
		{
			m_pScoreText = new TextComponent(asset, L" " + std::to_wstring(m_Score));
		}
		pTextObject->AddComponent(m_pScoreText);
		pObject->AddChild(pTextObject);
		if (pCoinSprite)
		{
			const auto& dim = pCoinSprite->GetTexture()->GetDimension();
			pTextObject->GetTransform()->Translate(dim.x * 0.5f, dim.y * 0.25f, 0);
		}
	}
}

void ScoreManagerComponent::AddScore(int difference)
{
	//Update Score Text
	m_Score += difference;
	if (m_pScoreText)
	{
		if (m_Score < 0)
		{
			m_pScoreText->SetText(std::to_wstring(m_Score));
		}
		else
		{
			m_pScoreText->SetText(L" " + std::to_wstring(m_Score));
		}
	}

	//Spawn Score Text
	std::wstring asset{};
	if (AssetFiles::GetInstance().Get(eAssetId::Font_Game_Text, asset))
	{
		auto* pObject = new GameObject();
		ScoreTextComponent* pScore = nullptr;
		if (difference > 0)
		{
			pScore = new ScoreTextComponent(difference, asset, { 0,1,0,1 }, { 0,-15,0 }, { 0, -2.5f, 0 }, -0.45f);
		}
		else
		{
			pScore = new ScoreTextComponent(difference, asset, { 1,0,0,1 }, { 0,-15,0 }, { 0, -2.5f, 0 }, -0.45f);
		}
		pObject->AddComponent(pScore);
		if (m_pScoreText)
		{
			const auto& p = m_pScoreText->GetTransform()->GetPosition();
			pObject->GetTransform()->Translate(p.x, p.y, p.z);
		}
		GetGameObject()->AddChild(pObject);
	}
}
