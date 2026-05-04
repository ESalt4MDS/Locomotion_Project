#pragma once
#include <SFML/Graphics.hpp>

inline static sf::Vector2f WindowSize = { 800, 800 };

inline float plGetVectorLength(sf::Vector2f _vector)
{
	return sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y));
}

inline sf::Vector2f plNormalize(sf::Vector2f _vector)
{
	float Magnitude = plGetVectorLength(_vector);

	return _vector /= Magnitude;
}