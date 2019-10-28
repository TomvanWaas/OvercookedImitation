#include "stdafx.h"
#include "HolderIconsComponent.h"
#include "Observed.h"
#include "../../Helpers/Events.h"
#include "../../Structs/Recipe.h"
#include "HolderComponent.h"
#include "../../Components/Visuals/ParticleComponent.h"
#include "GameObject.h"
#include "../../Singletons/AssetFiles.h"
#include "TransformComponent.h"



HolderIconsComponent::HolderIconsComponent(const HolderInfo& info, int size,
	const std::function<bool(eFoodType, eFoodState)>& showRequirement)
	: m_HolderInfo(info)
	, m_pParticles()
	, m_pHolderComponent(nullptr)
	, m_ShowRequirement(showRequirement)
{
	m_pParticles.resize(size);
}

PrototypeComponent* HolderIconsComponent::Clone() const
{
	return new HolderIconsComponent(m_HolderInfo, int(m_pParticles.size()), m_ShowRequirement);
}

void HolderIconsComponent::Initialize(const GameContext&)
{
	const float scale = 0.015f;

	auto* pObject = GetGameObject();

	//Get Attached HolderComponent
	//****************************
	m_pHolderComponent = pObject->GetComponent<HolderComponent>();

	//Create Particle Components
	//**************************
	std::wstring texture{};
	if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Icons_Food, texture))
	{
		GameObject* pChild = new GameObject();
		for (int i = 0, j = m_pParticles.size(); i < j; ++i)
		{
			//Create
			//******
			ParticleComponent* pParticle = new ParticleComponent(texture);
			pParticle->DrawOnScreen();
			pParticle->SetSize({ 0,0 });
			pParticle->SetPivot({ 0,0 });


			m_pParticles[i] = pParticle;
			pChild->AddComponent(pParticle);
		}
		pChild->GetTransform()->Translate(m_HolderInfo.iconOffsetWorld);
		pChild->GetTransform()->Scale(scale, scale, scale);
		pObject->AddChild(pChild);
	}
}
void HolderIconsComponent::Update(const GameContext&)
{
}
void HolderIconsComponent::Draw(const GameContext&)
{
}


void HolderIconsComponent::OnNotify(ObservedID observedID)
{
	if (m_pHolderComponent)
	{
		switch (observedID.id)
		{
		//Events
		//******
		case Events::EVENT_HOLDER_FOOD_ADDED:
		case Events::EVENT_HOLDER_FOOD_REMOVED:
		case Events::EVENT_FOOD_STATE_CHANGED:
		{
			//Info
			//****
			const float offset = 1.0f / (int(eFoodType::_END));

			Recipe recipe = m_pHolderComponent->GetRecipe();
			int size = recipe.Size();
			int count = 0;

			for (int i = 0, j = m_pParticles.size(); i < j && i < Recipe::SIZE; ++i)
			{
				ParticleComponent* pParticle = m_pParticles[i];
				if (!pParticle) continue;
				eFoodState state = recipe.foodStates[i];
				eFoodType type = recipe.foodTypes[i];

				//Show
				//****
				if (state == eFoodState::Burned && i == 0)
				{
					pParticle->SetSize({ offset, 1 });
					pParticle->SetPivot({ int(eFoodType::_Fire) * offset, 0 });
					pParticle->SetOffset(m_HolderInfo.iconOffsetScreen);
				}
				else if (!m_ShowRequirement || m_ShowRequirement(type, state) && state != eFoodState::Burned)
				{
					pParticle->SetSize({ offset, 1 });
					pParticle->SetPivot({ int(type) * offset, 0 });
					pParticle->SetOffset(IconOffset(count, size, offset));
					++count;
				}
				//Don't Show
				//**********
				else
				{
					pParticle->SetSize({ 0,0 });
				}
			}
		}
		break;
		}
	}
}

DirectX::XMFLOAT2 HolderIconsComponent::IconOffset(int iconIndex, int maxIndex, float offset) const
{
	int s = int(std::sqrt(maxIndex) + 0.5f);
	int c = iconIndex / s;
	int r = iconIndex % s;

	int maxc = (maxIndex-1) / s;

	DirectX::XMFLOAT2 result{ (c - maxc * 0.5f) + m_HolderInfo.iconOffsetScreen.x, (r) + m_HolderInfo.iconOffsetScreen.y };
	result.x *= offset;
	return result;
}
