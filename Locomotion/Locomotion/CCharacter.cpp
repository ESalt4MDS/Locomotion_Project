#include "CCharacter.h"
#include "PhysicsLibrary.h";

CCharacter::CCharacter(sf::Vector2f _position)
{
	m_shape = new sf::RectangleShape({ 20.0f, 20.0f });
	m_shape->setPosition(_position);

	m_currentPosition = _position;
	m_currentVelocity = sf::Vector2f(0.0f, 0.0f);

	m_currentVelLine.append(sf::Vertex());
	m_currentVelLine.append(sf::Vertex());
	m_currentVelLine.setPrimitiveType(sf::PrimitiveType::Lines);

	m_desiredVelLine.append(sf::Vertex());
	m_desiredVelLine.append(sf::Vertex());
	m_desiredVelLine.setPrimitiveType(sf::PrimitiveType::Lines);

	m_steeringLine.append(sf::Vertex());
	m_steeringLine.append(sf::Vertex());
	m_steeringLine.setPrimitiveType(sf::PrimitiveType::Lines);
}

CCharacter::~CCharacter()
{
	delete m_shape;
	m_shape = nullptr;
}

void CCharacter::Update(float _dt, sf::Vector2f _targetPosition)
{
	//Seek(_targetPosition, float _dt);

	//update velocity and posiition
	m_currentVelocity += m_steeringForce * _dt;

	m_currentPosition += m_currentVelocity * _dt;
	m_shape->setPosition(m_currentPosition);


	//debug lines
	m_currentVelLine[0].position = m_shape->getPosition();
	m_currentVelLine[0].color = sf::Color::Green;

	m_currentVelLine[1].position = (m_shape->getPosition() + m_currentVelocity) * 1.0f;
	m_currentVelLine[1].color = sf::Color::Green;

	//printf("current position: %f, %f\n", m_shape->getPosition().x, m_shape->getPosition().y);

}

void CCharacter::Draw(sf::RenderWindow* _window)
{

	_window->draw(*m_shape);
	_window->draw(m_currentVelLine);
	_window->draw(m_desiredVelLine);
	_window->draw(m_steeringLine);

}

void CCharacter::Seek(sf::Vector2f _targetPosition, float _dt)
{
	//calculate direction to move in
	sf::Vector2f desiredDirection;
	desiredDirection = _targetPosition - m_shape->getPosition();
	sf::Vector2f desiredVelocity;
	desiredVelocity = desiredDirection;

	//truncate desired velocity
	if (desiredVelocity.length() > m_maxDesiredVelocity)
	{
		desiredVelocity = plNormalize(desiredVelocity) * m_maxDesiredVelocity;
	}

	m_steeringForce = desiredVelocity - m_currentVelocity;

	//truncate steering force
	if (m_steeringForce.length() > m_maxDesiredSteering)
	{
		m_steeringForce = plNormalize(m_steeringForce) * m_maxDesiredSteering;
	}

	//update velocity and posiition
	m_currentVelocity += m_steeringForce * _dt;

	m_currentPosition += m_currentVelocity * _dt;
	m_shape->setPosition(m_currentPosition);


	//debug lines
	//GREEN: Current Velocity
	//CYAN: Desired Velocity
	//BLUE: Steering Force
	m_currentVelLine[0].position = m_shape->getPosition();
	m_currentVelLine[0].color = sf::Color::Green;

	m_currentVelLine[1].position = (m_shape->getPosition() + m_currentVelocity);
	m_currentVelLine[1].color = sf::Color::Green;

	m_desiredVelLine[0].position = m_shape->getPosition();
	m_desiredVelLine[0].color = sf::Color::Cyan;

	m_desiredVelLine[1].position = (m_shape->getPosition() + desiredVelocity);
	m_desiredVelLine[1].color = sf::Color::Cyan;

	m_steeringLine[0].position = (m_shape->getPosition() + m_currentVelocity);
	m_steeringLine[0].color = sf::Color::Blue;

	m_steeringLine[1].position = (m_shape->getPosition() + m_currentVelocity) + m_steeringForce;
	m_steeringLine[1].color = sf::Color::Blue;

	//printf("current position: %f, %f\n", m_shape->getPosition().x, m_shape->getPosition().y);


}

