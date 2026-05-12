#include "CCharacterManager.h"

CCharacterManager::CCharacterManager()
{
}

CCharacterManager::~CCharacterManager()
{
	for (auto& iter : m_characters)
	{
		delete iter;
		iter = nullptr;
	}
}

void CCharacterManager::InputUpdate(const std::optional<sf::Event>& _event, sf::RenderWindow* _window)
{
	if (m_keyBinds.m_spawn)
	{
		if (const auto mouseButtonReleased = _event->getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseButtonReleased->button == sf::Mouse::Button::Left)
			{
				SpawnCharacter(sf::Vector2f(sf::Mouse::getPosition(*_window).x, sf::Mouse::getPosition(*_window).y));
			}
		}
	}

	UpdateKeyBinds(_event);

}

void CCharacterManager::Update(sf::Vector2f _targetPosition, sf::Vector2f _targetVelocity, float _dt)
{
	//update characters
	for (auto& iter : m_characters)
	{
		iter->Update();
	}

	m_targetPosition = _targetPosition;
	m_targetVelocity = _targetVelocity;
	m_dt = _dt;

	//update character behavior
	for (auto& iter : m_characters)
	{
		HandleBehavior(*iter);
	}
}

void CCharacterManager::Draw(sf::RenderWindow* _window)
{
	//draw characters
	for (auto& iter : m_characters)
	{
		iter->Draw(_window);
	}
}

void CCharacterManager::SpawnCharacter(sf::Vector2f _position)
{
	CCharacter* character = new CCharacter(_position, Behavior::SEEK);

	m_characters.push_back(character);
}

void CCharacterManager::HandleBehavior(CCharacter& _character)
{
	switch (_character.GetBehavior())
	{
	case Behavior::SEEK:
		_character.Seek(m_targetPosition, m_dt);
		break;
	case Behavior::FLEE:
		_character.Flee(m_targetPosition, m_dt);
		break;
	case Behavior::WANDER:
		_character.Wander(m_dt);
		break;
	case Behavior::PURSUIT:
		_character.Pursuit(m_targetVelocity, m_targetPosition, m_dt);
		break;
	case Behavior::EVADE:
		_character.Evade(m_targetVelocity, m_targetPosition, m_dt);
		break;
	default:
		break;
	}
}

void CCharacterManager::UpdateKeyBinds(const std::optional<sf::Event>& _event)
{
	if (const auto* keyPressed = _event->getIf<sf::Event::KeyReleased>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::Num1)
		{
			m_keyBinds.m_spawn = !m_keyBinds.m_spawn;
		}
	}
}
