#include "CCharacter.h"

CCharacter::CCharacter(sf::Vector2f _position)
{
	m_shape = new sf::RectangleShape({ 20.0f, 20.0f });
	m_shape->setPosition(_position);

	m_currentPosition = _position;
	m_currentVelocity = sf::Vector2f(0.0f, 0.0f);

}

CCharacter::~CCharacter()
{
	delete m_shape;
	m_shape = nullptr;
}

void CCharacter::Update()
{
}

void CCharacter::Draw(sf::RenderWindow* _window)
{
	_window->draw(*m_shape);
}

