#pragma once
#include <SFML/Graphics.hpp>

inline static sf::Vector2f WindowSize = { 800, 800 };

inline float plGetVectorLength(sf::Vector2f _vector)
{
	return sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y));
}

inline sf::Vector2f plNormalize(sf::Vector2f _vector)
{
	if (_vector.length() <= 0.0f)
	{
		return sf::Vector2f(0.0f, 0.0f);
	}
	float Magnitude = plGetVectorLength(_vector);

	return _vector /= Magnitude;
}

inline sf::Vector2f plDivVecByScalar(sf::Vector2f _vector, int _int)
{
	_vector.x /= _int;
	_vector.y /= _int;

	return _vector;
}