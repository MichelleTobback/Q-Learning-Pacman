#pragma once
#include <functional>

class Controller
{
public:
	Controller() = default;
	~Controller() = default;

	void Update();

	std::function<void()> m_MoveLeft;
	std::function<void()> m_MoveRight;
	std::function<void()> m_MoveUp;
	std::function<void()> m_MoveDown;
};

