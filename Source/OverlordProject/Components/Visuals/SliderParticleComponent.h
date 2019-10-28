#pragma once

#include "../../Components/Visuals/ParticleComponent.h"
class SliderParticleComponent final : public ParticleComponent
{
public:
	explicit SliderParticleComponent(const std::wstring& assetFile, const DirectX::XMFLOAT2& direction = {1, 0});
	virtual ~SliderParticleComponent() = default;
	SliderParticleComponent(const SliderParticleComponent& other) = delete;
	SliderParticleComponent(SliderParticleComponent&& other) noexcept = delete;
	SliderParticleComponent& operator=(const SliderParticleComponent& other) = delete;
	SliderParticleComponent& operator=(SliderParticleComponent&& other) noexcept = delete;

	void SetSliderDirection(const DirectX::XMFLOAT2& direction);
	const DirectX::XMFLOAT2& GetSliderDirection() const;
	void SetSliderLength(float length);
	float GetSliderLength() const;
private:
	float m_Length;
	DirectX::XMFLOAT2 m_Direction;

};

