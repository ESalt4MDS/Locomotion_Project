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
		iter->Update(_dt);
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
	CCharacter* character = new CCharacter(_position, m_keyBinds.m_currentBehavior);

	m_characters.push_back(character);

	UpdateCharacters();
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
	case Behavior::FLOCKING:
		_character.Seek(m_targetPosition, m_dt);
		_character.Separate(m_characters, m_dt);
		_character.Cohesion(m_characters, m_dt);
		_character.Alignment(m_characters, m_dt);
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

		if (keyPressed->scancode == sf::Keyboard::Scancode::Num2)
		{
			m_keyBinds.m_debug = !m_keyBinds.m_debug;
			UpdateCharacters();
		}

		if (keyPressed->scancode == sf::Keyboard::Scancode::Num3)
		{
			//change character behavior to seek
			m_keyBinds.m_currentBehavior = Behavior::SEEK;
			UpdateCharacters();
		}

		if (keyPressed->scancode == sf::Keyboard::Scancode::Num4)
		{
			//change character behavior to flee
			m_keyBinds.m_currentBehavior = Behavior::FLEE;
			UpdateCharacters();
		}

		if (keyPressed->scancode == sf::Keyboard::Scancode::Num5)
		{
			//change character behavior to wander
			m_keyBinds.m_currentBehavior = Behavior::WANDER;
			UpdateCharacters();
		}

		if (keyPressed->scancode == sf::Keyboard::Scancode::Num6)
		{
			//change character behavior to pursuit
			m_keyBinds.m_currentBehavior = Behavior::PURSUIT;
			UpdateCharacters();
		}

		if (keyPressed->scancode == sf::Keyboard::Scancode::Num7)
		{
			//change character behavior to evade
			m_keyBinds.m_currentBehavior = Behavior::EVADE;
			UpdateCharacters();
		}

		if (keyPressed->scancode == sf::Keyboard::Scancode::Num8)
		{
			//change character behavior to evade
			m_keyBinds.m_currentBehavior = Behavior::FLOCKING;
			UpdateCharacters();
		}
	}
}
void CCharacterManager::UpdateCharacters()
{
	for (auto& iter : m_characters)
	{
		iter->SetBehavior(m_keyBinds.m_currentBehavior);
		iter->SetDebug(m_keyBinds.m_debug);
	}
}
