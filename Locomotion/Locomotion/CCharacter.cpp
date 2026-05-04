#include "CCharacter.h"
#include "PhysicsLibrary.h";
#include "random"

CCharacter::CCharacter(sf::Vector2f _position)
{
	m_shape = new sf::RectangleShape({ 20.0f, 20.0f });
	m_shape->setPosition(_position);
	m_shape->setOrigin({ 10.0f,10.0f });

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

	m_wanderLine.append(sf::Vertex());
	m_wanderLine.append(sf::Vertex());
	m_wanderLine.setPrimitiveType(sf::PrimitiveType::Lines);

	m_wanderCircle.setFillColor(sf::Color::Transparent);
	m_wanderCircle.setOutlineThickness(1.0f);
	m_wanderCircle.setOutlineColor(sf::Color::Yellow);
	m_wanderCircle.setRadius(m_wanderRadius);
	m_wanderCircle.setOrigin({ m_wanderRadius , m_wanderRadius });
}

CCharacter::~CCharacter()
{
	delete m_shape;
	m_shape = nullptr;
}

void CCharacter::Update()
{
	//Seek(_targetPosition, float _dt);

	//update velocity and posiition
	//m_currentVelocity += m_steeringForce * _dt;

	//m_currentPosition += m_currentVelocity * _dt;
	//m_shape->setPosition(m_currentPosition);


	////debug lines
	//m_currentVelLine[0].position = m_shape->getPosition();
	//m_currentVelLine[0].color = sf::Color::Green;

	//m_currentVelLine[1].position = (m_shape->getPosition() + m_currentVelocity) * 1.0f;
	//m_currentVelLine[1].color = sf::Color::Green;

	//printf("current position: %f, %f\n", m_shape->getPosition().x, m_shape->getPosition().y);

	if (m_currentPosition.x > WindowSize.x)
	{
		m_currentPosition.x = 0.0f;
	}

	if (m_currentPosition.y > WindowSize.y)
	{
		m_currentPosition.y = 0.0f;
	}

	if (m_currentPosition.x < 0.0f)
	{
		m_currentPosition.x = WindowSize.x;
	}

	if (m_currentPosition.y < 0.0f)
	{
		m_currentPosition.y = WindowSize.y;
	}

	m_shape->setPosition(m_currentPosition);
}

void CCharacter::Draw(sf::RenderWindow* _window)
{

	_window->draw(*m_shape);

	_window->draw(m_currentVelLine);
	//_window->draw(m_desiredVelLine);
	_window->draw(m_steeringLine);

	_window->draw(m_wanderCircle);
	_window->draw(m_wanderLine);

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

void CCharacter::Flee(sf::Vector2f _targetPosition, float _dt)
{
	//calculate direction to move in
	sf::Vector2f desiredDirection;
	desiredDirection = m_shape->getPosition() - _targetPosition;
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

void CCharacter::Wander(float _dt)
{
	if (m_currentVelocity.length() < 0.01)
	{
		m_currentVelocity.x = 1.0f;
	}

	float randnum = ((float)rand()) / RAND_MAX * m_wanderRandSpin - (m_wanderRandSpin / 2.0f);
	m_wanderAngle += randnum;
	//printf("%f\n", randnum);


	sf::Vector2f wanderOrigin = m_shape->getPosition() + plNormalize(m_currentVelocity) * m_wanderForwardDistance;
	sf::Vector2f wanderVector = sf::Vector2f(cos(m_wanderAngle), sin(m_wanderAngle));
	sf::Vector2f wanderTarget = wanderOrigin + plNormalize(wanderVector) * m_wanderRadius;

	m_steeringForce = wanderTarget - m_shape->getPosition();

	if (m_steeringForce.length() > m_maxDesiredSteering)
	{
		m_steeringForce = plNormalize(m_steeringForce) * m_maxDesiredSteering;
	}

	m_currentVelocity += m_steeringForce * _dt;

	if (m_currentVelocity.length() > m_maxDesiredVelocity)
	{
		m_currentVelocity = plNormalize(m_currentVelocity) * m_maxDesiredVelocity;
	}

	m_currentPosition += m_currentVelocity * _dt;
	m_shape->setPosition(m_currentPosition);

	//debug
	m_wanderCircle.setPosition(wanderOrigin);

	m_wanderLine[0].position = wanderOrigin;
	m_wanderLine[0].color = sf::Color::Cyan;

	m_wanderLine[1].position = wanderTarget;
	m_wanderLine[1].color = sf::Color::Cyan;

	//debug lines
	//GREEN: Current Velocity
	//CYAN: Desired Velocity
	//BLUE: Steering Force
	m_currentVelLine[0].position = m_shape->getPosition();
	m_currentVelLine[0].color = sf::Color::Green;

	m_currentVelLine[1].position = (m_shape->getPosition() + m_currentVelocity);
	m_currentVelLine[1].color = sf::Color::Green;

	m_steeringLine[0].position = (m_shape->getPosition() + m_currentVelocity);
	m_steeringLine[0].color = sf::Color::Blue;

	m_steeringLine[1].position = (m_shape->getPosition() + m_currentVelocity) + m_steeringForce;
	m_steeringLine[1].color = sf::Color::Blue;
	
}

