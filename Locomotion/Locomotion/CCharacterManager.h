#pragma once
#include <vector>
#include <optional>
#include "CCharacter.h"

struct KeyBinds
{
	bool m_spawn = false;
};

class CCharacterManager
{
public:

	CCharacterManager();
	~CCharacterManager();

	void InputUpdate(const std::optional<sf::Event>& _event, sf::RenderWindow* _window);
	void Update(sf::Vector2f _targetPosition, sf::Vector2f _targetVelocity, float _dt);
	void Draw(sf::RenderWindow* _window);

	void SpawnCharacter(sf::Vector2f _position);
	void HandleBehavior(CCharacter& _character);

private:

	std::vector<CCharacter*> m_characters;

	sf::Vector2f m_targetPosition;
	sf::Vector2f m_targetVelocity;
	float m_dt;

	KeyBinds m_keyBinds;
	void UpdateKeyBinds(const std::optional<sf::Event>& _event);


};

