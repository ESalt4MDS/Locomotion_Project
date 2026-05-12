#pragma once
#include <SFML/Graphics.hpp>

#include "CCharacterManager.h"
#include "CTarget.h"

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

	CTarget* m_target;

	CCharacterManager* m_characterManager;

	sf::Vector2f m_mousePosition;
};

