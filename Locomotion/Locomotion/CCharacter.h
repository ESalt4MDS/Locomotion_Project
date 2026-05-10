#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class CCharacter
{
public:
	CCharacter(sf::Vector2f _position);
	~CCharacter();

	//void Update(float _dt, sf::Vector2f _targetPosition);
	void Update();
	void Draw(sf::RenderWindow* _window);

	void Seek(sf::Vector2f _targetPosition, float _dt);
	void Flee(sf::Vector2f _targetPosition, float _dt);
	void Wander(float _dt);

private:
	sf::RectangleShape* m_shape;
	sf::Vector2f m_currentPosition;
	sf::Vector2f m_currentVelocity;
	sf::Vector2f m_steeringForce;

	//seek & flee
	float m_maxDesiredVelocity = 128.0f;
	float m_maxDesiredSteering = 64.0f;
	float m_speed = 0.0f;
	float m_maxSpeed = 20.0f;

	//wander
	float m_wanderForwardDistance = 1.0f;
	float m_wanderAngle = 0.22f;
	float m_wanderRadius = 100.0f;
	float m_wanderRandSpin = 0.05f;

	//arrive
	float m_slowingRadius = 200.0f;


	//seek & flee debug lines
	sf::VertexArray m_currentVelLine;
	sf::VertexArray m_steeringLine;
	sf::VertexArray m_desiredVelLine;

	//wander debug lines
	sf::CircleShape m_wanderCircle;
	sf::VertexArray m_wanderLine;

	//arrive debug
	sf::CircleShape m_arrivalCircle;

};

