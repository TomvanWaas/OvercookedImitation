#include "stdafx.h"
#include "RecipeComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "../../Singletons/AssetFiles.h"
#include "TransformComponent.h"
#include "TextureData.h"
#include "../../Structs/Recipe.h"
#include "../../Helpers/Enums.h"
#include "../Other/ColorFlickerComponent.h"
#include "../../Helpers/Events.h"
#include "../Other/ShakeComponent.h"
#include "../Other/LerpComponent.h"
#include "Destroyer.h"

RecipeComponent::RecipeComponent(const Recipe& recipe, float time, eMealType meal)
	: m_Recipe(recipe)
	, m_Speed(1/time)
	, m_Accu()
	, m_Meal(meal)
{
}


float RecipeComponent::GetRemainingTime() const
{
	return m_Accu.GetTime() / m_Speed;
}

const Timer& RecipeComponent::GetTimer() const
{
	return m_Accu;
}

float RecipeComponent::GetWidth() const
{
	if (m_pTimerSprite)
	{
		float width = m_pTimerSprite->GetTexture()->GetDimension().x;
		width *= m_pTimerSprite->GetTransform()->GetScale().x;
		auto* pBackgr = m_pTimerSprite->GetGameObject()->GetParent();
		if (pBackgr)
		{
			width *= pBackgr->GetTransform()->GetScale().x;
		}
		return width;
	}
	return 0;
}



void RecipeComponent::Remove(const std::function<void()>& onDestroy)
{
	if (m_pFlickerComponent)
	{
		m_pFlickerComponent->SetColorA({ 0,1,0,1 });
		m_pFlickerComponent->SetColorB({ 1,1,1,1 });
		m_pFlickerComponent->SetOnEnd([this, onDestroy]()
		{
			Destroyer::GetInstance().Destroy(this->GetGameObject());
			if (onDestroy) onDestroy();
		});
		m_pFlickerComponent->Enable(true, 0.5f);
	}
	m_Activated = false;
}

void RecipeComponent::Activate()
{
	m_Activated = true;
	//Show Recipe
	if (m_pRecipeLerpComponent)
	{
		const auto& p = m_pRecipeLerpComponent->GetTransform()->GetPosition();
		m_pRecipeLerpComponent->SetTarget({ p.x, p.y + 256, p.z }); //256 pixels as background is that high
	}
}

void RecipeComponent::Update(const GameContext& gameContext)
{	
	if (!m_Activated) return;

	//Timer & Color
	m_Accu.SetTime(m_Accu.GetTime() - gameContext.pGameTime->GetElapsed() * m_Speed);
	if (m_pTimerSprite)
	{
		//Size
		m_pTimerSprite->SetSize({ m_Accu.GetTime(), 1 }, true);

		//Color
		DirectX::XMFLOAT4 color = (m_Accu.GetTime() > 0.5f) ?
			(Lerp({ 0,1,0,1 }, { 1,1,0,1 }, (m_Accu.GetTime() - 1) * -2)) :
			(Lerp({ 1,1,0,1 }, { 1,0,0,1 }, (m_Accu.GetTime() - 0.5f) * -2));

		m_pTimerSprite->SetColor(color);
	}

	//Late => Shake
	//*************
	if (m_pShakeComponent && !m_pShakeComponent->IsEnabled() && m_Accu.IsLate())
	{
		GetGameObject()->Notify(Events::EVENT_RECIPE_TIMELATE);
		if (m_pShakeComponent) m_pShakeComponent->Enable(true, m_Accu.GetTime() / m_Speed + gameContext.pGameTime->GetElapsed());
	}

	//Out of Time?
	//************
	if (m_pFlickerComponent && m_Accu.IsEmpty() && !m_pFlickerComponent->IsEnabled())
	{
		GetGameObject()->Notify(Events::EVENT_RECIPE_TIMEOUT);
		m_Accu.SetTime(1.0f);
		m_pFlickerComponent->Enable(true, 1);
	}
}

void RecipeComponent::Initialize(const GameContext&)
{
	//Tree Structure
	//**************
	//- this GameObject
	//	- Base Background
	//		- Timer
	//		- Meal
	//	- Recipe
	//		- FoodBack(s)
	//		- FoodType(s)
	//		- FoodState(s)

	//Data
	//****
	auto* pMasterObject = GetGameObject();
	std::wstring assetBuffer{};
	int recipeSize = m_Recipe.Size();
	float foodTextureOffset = 1.0f / (int(eFoodType::_END)); 
	std::vector<SpriteComponent*> pFlickerSprites{};


	
	
	//-Recipe
	//*******
	GameObject* pRecipeObject = new GameObject();
	//FoodBacks
	if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Recipe_FoodBackground, assetBuffer))
	{
		float offset = 0.0f;
		for (int i = 0; i < Recipe::SIZE; ++i)
		{
			if (m_Recipe.ValidState(i))
			{
				GameObject* pFoodBackObject = new GameObject();
				SpriteComponent* pFoodBackSprite = new SpriteComponent(assetBuffer);
				pFoodBackSprite->SetSize({ 0.5f, 1 }, true);
				pFoodBackSprite->SetPivot({ 0,0 }, true);

				//Set Values
				pFlickerSprites.push_back(pFoodBackSprite);
				switch(m_Recipe.foodStates[i])
				{
				case eFoodState::Cut: {
						//Small
					pFoodBackSprite->SetPivot({ 0,0 }, true);
					}
					break;
				case eFoodState::Cooked:
				case eFoodState::Baked: {
						//Big
					pFoodBackSprite->SetPivot({ 0.5f,0 }, true);
					}
					break;
				}

				pFoodBackObject->GetTransform()->Translate(offset, 0, 0);
				offset += pFoodBackSprite->GetSize().x;
				pFoodBackObject->AddComponent(pFoodBackSprite);
				pRecipeObject->AddChild(pFoodBackObject);
			}
		}
	}
	//FoodTypes
	if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Icons_Food, assetBuffer))
	{
		float offset = 0.0f;
		for (int i = 0; i < Recipe::SIZE; ++i)
		{
			if (m_Recipe.ValidType(i))
			{
				GameObject* pFoodTypeObject = new GameObject();
				SpriteComponent* pFoodTypeSprite = new SpriteComponent(assetBuffer);

				//Set Values
				pFlickerSprites.push_back(pFoodTypeSprite);
				pFoodTypeSprite->SetSize({ foodTextureOffset, 1 }, true);
				pFoodTypeSprite->SetPivot({ (int(m_Recipe.foodTypes[i]))*foodTextureOffset, 0 }, true);

				pFoodTypeObject->GetTransform()->Translate(offset, 0, 0);
				offset += pFoodTypeSprite->GetSize().x;
				pFoodTypeObject->AddComponent(pFoodTypeSprite);
				pRecipeObject->AddChild(pFoodTypeObject);
			}
		}
	}
	//FoodStates
	if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Icons_States, assetBuffer))
	{
		int nstates = 2;

		std::vector<std::pair<eFoodState, int>> counter{};
		for (int i = 0; i < Recipe::SIZE; ++i)
		{
			if (m_Recipe.ValidState(i))
			{
				auto state = m_Recipe.foodStates[i];
				if (counter.size() == 0)
				{
					counter.push_back({ state, 1 });
				}
				else if (counter[counter.size()-1].first == state)
				{
					++counter[counter.size() - 1].second;
				}
				else
				{
					counter.push_back({ state, 1 });
				}
			}
		}
		
		int offsetCount = 0;
		for (const auto& pair : counter)
		{
			if (int(pair.first) - 1 < nstates)
			{
				GameObject* pStateObject = new GameObject();
				SpriteComponent* pStateSprite = new SpriteComponent(assetBuffer);
				pFlickerSprites.push_back(pStateSprite);
				pStateSprite->SetSize({ 1.0f / nstates, 1 }, true);
				pStateSprite->SetPivot({ (float(pair.first)) / nstates, 0 }, true);
				const auto& s = pStateSprite->GetSize();
				float x = s.x * (offsetCount + pair.second*0.5f) - 0.5f*s.x;
				pStateObject->GetTransform()->Translate(x, s.y, 0);
				pStateObject->AddComponent(pStateSprite);
				pRecipeObject->AddChild(pStateObject);
			}
			offsetCount += pair.second;
		}
	}

	pRecipeObject->GetTransform()->Translate(0, 0, 0);
	pMasterObject->AddChild(pRecipeObject);

	//-Base Background
	//****************
	if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Recipe_Background, assetBuffer))
	{
		GameObject* pBaseBackgroundObject = new GameObject();
		SpriteComponent* pBaseBackgroundSprite = new SpriteComponent(assetBuffer);

		//Set Values
		pFlickerSprites.push_back(pBaseBackgroundSprite);
		pBaseBackgroundObject->GetTransform()->Scale(float(recipeSize), 1, 1); //Texture is made for 1 => Mul recipe size;
		pBaseBackgroundObject->AddComponent(pBaseBackgroundSprite);

		//-Timer
		//*****
		if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Recipe_Timer, assetBuffer))
		{
			GameObject* pTimerObject = new GameObject();
			m_pTimerSprite = new SpriteComponent(assetBuffer);

			//Set Values
			pTimerObject->AddComponent(m_pTimerSprite);
			pBaseBackgroundObject->AddChild(pTimerObject);
		}

		//-Meal
		//****
		if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Icons_Meals, assetBuffer))
		{
			GameObject* pMealObject = new GameObject();
			SpriteComponent* pMealSprite = new SpriteComponent(assetBuffer);

			//Set Values
			pFlickerSprites.push_back(pMealSprite);
			const float mealTextureOffset = 1.0f / (int(eMealType::END));
			pMealSprite->SetSize({ mealTextureOffset, 1 }, true);
			pMealSprite->SetPivot({ mealTextureOffset * (int(m_Meal)), 0 }, true);
			pMealObject->GetTransform()->Scale(1 / float(recipeSize), 1, 1);
			pMealObject->AddComponent(pMealSprite);
			pBaseBackgroundObject->AddChild(pMealObject);

			const auto& msize = pMealSprite->GetSize();
			const auto& bsize = pBaseBackgroundSprite->GetSize();
			pMealObject->GetTransform()->Translate(bsize.x*0.5f - msize.x*0.25f, bsize.y*0.5f - msize.y*0.5f, 0);
		}

		pMasterObject->AddChild(pBaseBackgroundObject);
	}


	//Effects
	//*******
	m_pFlickerComponent = new ColorFlickerComponent({ 1,0,0,1 }, { 1,1,1,1 }, 7.5f, pFlickerSprites);
	pMasterObject->AddComponent(m_pFlickerComponent);
	m_pShakeComponent = new ShakeComponent({ 10,0,0 }, 50);
	pMasterObject->AddComponent(m_pShakeComponent);
	m_pRecipeLerpComponent = new LerpComponent({}, 5);
	pRecipeObject->AddComponent(m_pRecipeLerpComponent);
}

void RecipeComponent::Draw(const GameContext&)
{
}

DirectX::XMFLOAT4 RecipeComponent::Lerp(DirectX::XMFLOAT4 a, DirectX::XMFLOAT4 b, float t) const
{
	return { a.x * (1 - t) + b.x*t,
			a.y * (1 - t) + b.y*t,
			a.z * (1 - t) + b.z*t,
			a.w * (1 - t) + b.w*t };
}
