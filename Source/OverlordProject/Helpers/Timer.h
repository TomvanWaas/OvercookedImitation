#pragma once

class Timer final
{
public:
	explicit Timer() : m_Time(1.0f){}
	~Timer() = default;

	float GetTime() const { return m_Time; }
	void SetTime(float t) { m_Time = t; }

	bool IsGreen() const
	{
		return m_Time > 0.5f;
	}
	bool IsOrange() const
	{
		return (!IsGreen() && !IsRed());
	}
	bool IsRed() const
	{
		return m_Time < 0.25f;
	}
	bool IsLate() const
	{
		return m_Time < 0.10f;
	}
	bool IsEmpty() const
	{
		return m_Time <= 0.0f;
	}
private:
	float m_Time;
};