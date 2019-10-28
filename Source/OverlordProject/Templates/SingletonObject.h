#pragma once

template <class T>
class SingletonObject abstract
{
public:
	static T& GetInstance()
	{
		static T instance{};
		return instance;
	}
	virtual ~SingletonObject() = default;

	SingletonObject(const SingletonObject& other) = delete;
	SingletonObject(SingletonObject&& other) noexcept = delete;
	SingletonObject& operator=(const SingletonObject& other) = delete;
	SingletonObject& operator=(SingletonObject&& other) noexcept = delete;
protected:
	SingletonObject() = default;
};