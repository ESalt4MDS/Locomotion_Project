#include "CTarget.h"
#include "PhysicsLibrary.h"

CTarget::CTarget(sf::Vector2f _position)
{
	m_shape = new sf::RectangleShape({ 20.0f, 20.0f });
	m_shape->setPosition(_position);
	m_shape->setFillColor(sf::Color::Red);
	m_shape->setOrigin({ 10.0f, 10.0f });
}

CTarget::~CTarget()
{
}

void CTarget::Update(float _dt)
{
	sf::Vector2f MoveVec;

	//user input for player movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		MoveVec.y += -m_moveSpeed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		MoveVec.y += m_moveSpeed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		MoveVec.x += -m_moveSpeed;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		MoveVec.x += m_moveSpeed;
	}

	sf::Vector2f targetPosition;
	targetPosition = m_currentPosition + MoveVec;

	//m_currentPosition = plNormalize(m_currentVelocity) * m_moveSpeed * _dt;
	m_shape->setPosition(m_currentPosition);

	//calculate direction to move in
	sf::Vector2f desiredDirection;
	desiredDirection = targetPosition - m_shape->getPosition();
	sf::Vector2f desiredVelocity;
	desiredVelocity = desiredDirection;

	
	//truncate desired velocity
	if (desiredVelocity.length() > m_maxVelocity)
	{
		desiredVelocity = plNormalize(desiredVelocity) * m_maxVelocity;
	}
	
	m_steeringForce = desiredVelocity - m_currentVelocity;

	//truncate steering force
	if (m_steeringForce.length() > m_maxSteering)
	{
		m_steeringForce = plNormalize(m_steeringForce) * m_maxSteering;
	}

	//update velocity and position
	m_currentVelocity += m_steeringForce * _dt;

	m_currentPosition += m_currentVelocity * _dt;
	m_shape->setPosition(m_currentPosition);

	//printf("%f, %f\n", m_currentVelocity.x, m_currentVelocity.y);
}

void CTarget::Draw(sf::RenderWindow* _window)
{
	_window->draw(*m_shape);
}


