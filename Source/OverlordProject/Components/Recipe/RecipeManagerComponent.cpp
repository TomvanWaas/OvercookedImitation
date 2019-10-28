#include "stdafx.h"
#include "RecipeManagerComponent.h"
#include "RecipeComponent.h"
#include "GameObject.h"
#include "GameScene.h"
#include "../Other/ScoreManagerComponent.h"
#include "TransformComponent.h"
#include "../../Helpers/Events.h"
#include "Observed.h"
#include "../Other/LerpComponent.h"
#include "Destroyer.h"

RecipeManagerComponent::RecipeManagerComponent(CountData recipeCount, const std::vector<RecipeData>& recipeTemplateData, const DirectX::XMFLOAT3& spawnPos)
	: m_RecipeCountData(recipeCount)
	, m_RecipeTemplateData(recipeTemplateData)
	, m_SpawnedRecipes()
	, m_Accu(0.0f)
	, m_pScoreObject(nullptr)
	, m_SpawnPosition(spawnPos)
{
}

void RecipeManagerComponent::Deliver(const Recipe& recipe)
{
	//Points etc & Remove from spawned if

	//Find in Spawned Recipes
	//***********************
	auto it = std::find_if(m_SpawnedRecipes.begin(), m_SpawnedRecipes.end(), [&recipe](RecipeComponent* pRecipe)
	{
		return (pRecipe && pRecipe->GetRecipe() == recipe);
	});

	//Not Found
	//*****
	if (it == m_SpawnedRecipes.end())
	{
		Logger::LogInfo(L"RecipeManagerComponent::Deliver >> Delivered Recipe not in list");
		return;
	}
	Logger::LogInfo(L"RecipeManagerComponent::Deliver >> Delivered Recipe in list");

	//Found
	//*********

	//Reward
	if (m_pScoreObject)
	{
		const auto& timer = (*it)->GetTimer();
		m_pScoreObject->SucceedRecipe(timer, recipe);
	}

	//Remove
	auto* pRecipe = (*it);
	(*it)->Remove([this, pRecipe]()
	{
		auto iterator = std::find(m_SpawnedRecipes.begin(), m_SpawnedRecipes.end(), pRecipe);
		if (iterator != m_SpawnedRecipes.end())
		{
			Logger::LogInfo(L"RecipeManagerComponent::Deliver >> Removed Recipe");
			m_SpawnedRecipes.erase(iterator);
		}
		this->UpdatePositions();
	});

}

void RecipeManagerComponent::Draw(const GameContext&)
{
}
void RecipeManagerComponent::Initialize(const GameContext&)
{
	//Init ScoreObject
	//****************
	auto* pScene = GetGameObject()->GetScene();
	if (pScene)
	{
		m_pScoreObject = pScene->GetObjectWith<ScoreManagerComponent>();
	}
}
void RecipeManagerComponent::Update(const GameContext& gameContext)
{
	//Spawn New Recipes
	//*****************
	UINT count = m_SpawnedRecipes.size();

	//Return if too many
	if (count >= m_RecipeCountData.maximum) return;

	//Spawn if too little
	if (count < m_RecipeCountData.minimum)
	{
		for (count; count < m_RecipeCountData.minimum; ++count)
		{
			SpawnRecipe();
		}
		m_Accu = 0;
		return;
	}

	//Spawn Delayed around Average
	m_Accu += gameContext.pGameTime->GetElapsed();
	if (m_Accu >= TargetTime())
	{
		SpawnRecipe();
		m_Accu = 0;
	}
}

void RecipeManagerComponent::OnNotify(ObservedID observedID)
{
	switch(observedID.id)
	{
	case Events::EVENT_RECIPE_TIMEOUT:
	{
		if (m_pScoreObject) m_pScoreObject->FailRecipe();
	}
	break;

	//case Events::EVENT_RECIPE_TIMELATE:
	//{
	//	//...
	//}
	//break;
	}
}

GameObject* RecipeManagerComponent::SpawnRecipe()
{
	Logger::LogInfo(L"RecipeManagerComponent::SpawnRecipe >> Spawning New Recipe");

	//Create
	//******
	GameObject* pObject = new GameObject();
	int recipeID = rand() % m_RecipeTemplateData.size();
	const RecipeData& data = m_RecipeTemplateData[recipeID];
	RecipeComponent* pRecipeComponent = new RecipeComponent(data.recipe, data.time, data.meal);
	pObject->AddComponent(pRecipeComponent);
	pObject->AddObserver(this);
	auto* pLerp = new LerpComponent(GetTransform()->GetPosition(), 3.5f, 1);
	pLerp->SetOnDestination([pRecipeComponent, pLerp]()
	{
		pRecipeComponent->Activate();
		pLerp->SetOnDestination(nullptr);
	});
	pObject->AddComponent(pLerp);


	//Add
	//***
	m_SpawnedRecipes.push_back(pRecipeComponent);
	pObject->GetTransform()->Translate(m_SpawnPosition);
	GetGameObject()->AddChild(pObject);

	//Transform
	//*********
	UpdatePositions();

	return pObject;
}

float RecipeManagerComponent::TargetTime() const
{
	return m_RecipeCountData.delay;
	//TODO::...
}







void RecipeManagerComponent::UpdatePositions()
{
	float x = 0;
	float offset = 10.0f;
	const auto& pos = GetTransform()->GetPosition();
	for (UINT i = 0, size = m_SpawnedRecipes.size(); i < size; ++i)
	{
		auto* pRecipe = m_SpawnedRecipes[i];
		if (pRecipe)
		{
			auto* pLerpComponent = pRecipe->GetGameObject()->GetComponent<LerpComponent>();
			if (pLerpComponent)
			{
				pLerpComponent->SetTarget({ x, pos.y, pos.z });
				x += pRecipe->GetWidth() + offset;
			}	
		}
	}
}
