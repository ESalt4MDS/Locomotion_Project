#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class CCharacter
{
public:
	CCharacter(sf::Vector2f _position);
	~CCharacter();

	void Update(float _dt, sf::Vector2f _targetPosition);
	void Draw(sf::RenderWindow* _window);

	void Seek(sf::Vector2f _targetPosition);

private:
	sf::RectangleShape* m_shape;
	sf::Vector2f m_currentPosition;
	sf::Vector2f m_currentVelocity;
	sf::Vector2f m_steeringForce;

	float m_maxDesiredVelocity = 128.0f;
	float m_maxDesiredSteering = 64.0f;

	float m_speed = 0.0f;
	float m_maxSpeed = 20.0f;

	sf::VertexArray m_line;
};

