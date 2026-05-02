#pragma once
#include <SFML/Graphics.hpp>

#include "CCharacter.h"

class CProgramManager
{
public:

	CProgramManager();
	~CProgramManager();

	void Initialize();
	void RunProgram();
	void Draw();

private:

	sf::RenderWindow* m_window;

	CCharacter* m_character;

	sf::Clock m_clock;
};

