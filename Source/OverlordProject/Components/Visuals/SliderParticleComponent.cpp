#include "stdafx.h"
#include "SliderParticleComponent.h"
#include "GameObject.h"
#include "../../Components/Visuals/ParticleComponent.h"

SliderParticleComponent::SliderParticleComponent(const std::wstring& assetFile, const DirectX::XMFLOAT2& direction)
	: ParticleComponent(assetFile)
	, m_Length(1.0f)
	, m_Direction(direction)
{
	//Normalize Direction
	float magn = sqrt(m_Direction.x * m_Direction.x + m_Direction.y * m_Direction.y);
	if (magn != 0.0f)
	{
		m_Direction.x /= magn;
		m_Direction.y /= magn;
	}
}

void SliderParticleComponent::SetSliderDirection(const DirectX::XMFLOAT2& direction)
{
	m_Direction = direction;
	SetSliderLength(m_Length);
}
const DirectX::XMFLOAT2& SliderParticleComponent::GetSliderDirection() const
{
	return m_Direction;
}
void SliderParticleComponent::SetSliderLength(float length)
{
	//Calculate Original Offset
	DirectX::XMFLOAT2 extraOffset{};
	const DirectX::XMFLOAT2& currentOffset = GetOffset();
	float offset = -(1 - m_Length) * 0.5f;
	extraOffset.x = currentOffset.x - (offset * m_Direction.x + (1 - m_Direction.x));
	extraOffset.y = currentOffset.y - (offset * m_Direction.y + (1 - m_Direction.y));

	//Set Length
	m_Length = length;
	Clamp(m_Length, 1.0f, 0.0f);
	SetSize({ m_Length * m_Direction.x + (1 - m_Direction.x), m_Length * m_Direction.y + (1 - m_Direction.y) });

	//Calculate New Offset
	offset = -(1 - m_Length)*0.5f;
	SetOffset({ offset * m_Direction.x + (1 - m_Direction.x) + extraOffset.x, offset * m_Direction.y + (1 - m_Direction.y) + extraOffset.y});
}
float SliderParticleComponent::GetSliderLength() const
{
	return m_Length;
}

