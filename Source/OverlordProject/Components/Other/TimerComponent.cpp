#include "stdafx.h"
#include "TimerComponent.h"
#include "TextComponent.h"
#include "SpriteComponent.h"
#include "../../Singletons/AssetFiles.h"
#include "TextureData.h"
#include "TransformComponent.h"
#include "GameObject.h"

TimerComponent::TimerComponent(float time, const std::function<void()>& onDeplete)
	: m_pTextComponent(nullptr)
	, m_Time(time)
	, m_OnDeplete(onDeplete)
{
}

void TimerComponent::Initialize(const GameContext&)
{
	auto* pObject = GetGameObject();
	std::wstring asset{};

	//Background Texture
	SpriteComponent* pBackground = nullptr;
	if (AssetFiles::GetInstance().Get(eAssetId::Sprite_Timer_Hourglass, asset))
	{
		pBackground = new SpriteComponent(asset);
		pObject->AddComponent(pBackground);
	}
	//Text
	if (AssetFiles::GetInstance().Get(eAssetId::Font_Game_Text, asset))
	{
		GameObject* pTextObject = new GameObject();
		m_pTextComponent = new TextComponent(asset, TimeString());
		pTextObject->AddComponent(m_pTextComponent);
		pObject->AddChild(pTextObject);

		if (pBackground)
		{
			const auto& dim = pBackground->GetTexture()->GetDimension();
			pTextObject->GetTransform()->Translate(dim.x * 0.25f, dim.y * 0.25f, 0);
		}
	}
}

void TimerComponent::Update(const GameContext& gameContext)
{
	const float low = 10.0f;
	bool finished = m_Time <= 0.0f;
	bool below = m_Time <= low;
	m_Time -= gameContext.pGameTime->GetElapsed();
	if (m_pTextComponent)
	{
		m_pTextComponent->SetText(TimeString());
		if (!below && m_Time <= low) {
			m_pTextComponent->SetColor({ 1,0,0,1 });
		}
	}
	if (!finished && m_Time <= 0.0f && m_OnDeplete)
	{
		m_OnDeplete();
	}
}

std::wstring TimerComponent::TimeString() const
{
	return std::to_wstring(int(m_Time + 0.99f));
}
