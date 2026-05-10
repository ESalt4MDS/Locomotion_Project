#pragma once
#include <SFML/Graphics.hpp>

class CTarget
{
public:
	CTarget(sf::Vector2f _position);
	~CTarget();

	void Update();
	void Update(float _dt);
	void Draw(sf::RenderWindow* _window);

	sf::Vector2f GetCurrentPosition();

private:
	sf::RectangleShape* m_shape;
	sf::Vector2f m_currentPosition;
	sf::Vector2f m_currentVelocity;
	sf::Vector2f m_steeringForce;

	float m_moveSpeed = 128.0f;
	float m_friction = 1.0f;
	float m_maxVelocity = 512.0f;
	float m_maxSteering = 64.0f;

};

