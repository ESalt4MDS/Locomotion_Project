#pragma once
#include <SFML/Graphics.hpp>

class CCharacter
{
public:
	CCharacter(sf::Vector2f _position);
	~CCharacter();

	void Update();
	void Draw(sf::RenderWindow* _window);

private:
	sf::RectangleShape* m_shape;
	sf::Vector2f m_currentPosition;
	sf::Vector2f m_currentVelocity;

	float m_speed = 0.0f;
	float m_maxSpeed = 10.0f;
};

