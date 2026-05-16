#include "CCharacter.h"
#include "PhysicsLibrary.h";
#include "random"

CCharacter::CCharacter(sf::Vector2f _position, Behavior _behavior)
{
	m_behavior = _behavior;

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

	m_arrivalCircle.setFillColor(sf::Color::Transparent);
	m_arrivalCircle.setOutlineThickness(1.0f);
	m_arrivalCircle.setOutlineColor(sf::Color::Yellow);
	m_arrivalCircle.setRadius(m_slowingRadius);
	m_arrivalCircle.setOrigin({ m_slowingRadius , m_slowingRadius });

	m_pursuitLine.append(sf::Vertex());
	m_pursuitLine.append(sf::Vertex());
	m_pursuitLine.setPrimitiveType(sf::PrimitiveType::Lines);
}

CCharacter::~CCharacter()
{
	delete m_shape;
	m_shape = nullptr;
}

void CCharacter::Update(float _dt)
{

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

	if (m_behavior == Behavior::FLOCKING)
	{
		//summing steering forces together to apply all force affects
		sf::Vector2f finalSteeringForce = sf::Vector2f(0.0f, 0.0f);
		finalSteeringForce += m_steeringForce;
		finalSteeringForce += m_separateSteeringForce;
		finalSteeringForce += m_cohesionSteeringForce;
		finalSteeringForce += m_alignmentSteeringForce;

		//update velocity and position
		m_currentVelocity += finalSteeringForce * m_steeringStrength * _dt;

		if (m_currentVelocity.length() > m_maxDesiredVelocity)
		{
			m_currentVelocity = plNormalize(m_currentVelocity) * m_maxDesiredVelocity;
		}
	}
	else
	{
		m_currentVelocity += m_steeringForce * _dt;

		if (m_currentVelocity.length() > m_maxDesiredVelocity)
		{
			m_currentVelocity = plNormalize(m_currentVelocity) * m_maxDesiredVelocity;
		}
	}

	m_currentPosition += m_currentVelocity * _dt;
	m_shape->setPosition(m_currentPosition);

}

void CCharacter::Draw(sf::RenderWindow* _window)
{

	_window->draw(*m_shape);
	
	if (m_drawDebug)
	{
		_window->draw(m_currentVelLine);
		_window->draw(m_steeringLine);

		if (m_behavior == Behavior::WANDER)
		{
			_window->draw(m_wanderCircle);
			_window->draw(m_wanderLine);
		}
		else
		{
			_window->draw(m_desiredVelLine);
		}

		if (m_behavior == Behavior::SEEK || m_behavior == Behavior::PURSUIT)
		{
			_window->draw(m_arrivalCircle);
		}

		if (m_behavior == Behavior::PURSUIT)
		{
			_window->draw(m_pursuitLine);
		}
	}

}

Behavior CCharacter::GetBehavior()
{
	return m_behavior;
}

void CCharacter::SetBehavior(Behavior _behavior)
{
	m_behavior = _behavior;
}

void CCharacter::SetDebug(bool _debug)
{
	m_drawDebug = _debug;
}

sf::Vector2f CCharacter::GetCurrentPosition()
{
	return m_currentPosition;
}

sf::Vector2f CCharacter::GetCurrentVelocity()
{
	return m_currentVelocity;
}

void CCharacter::Seek(sf::Vector2f _targetPosition, float _dt)
{
	//calculate direction to move in
	sf::Vector2f desiredDirection;
	desiredDirection = _targetPosition - m_shape->getPosition();
	sf::Vector2f desiredVelocity;
	desiredVelocity = desiredDirection;

	//Arrive
	float distance = desiredVelocity.length();
	//printf("%f\n", (distance / m_slowingRadius));

	if (distance < m_slowingRadius)
	{
		desiredVelocity = plNormalize(desiredVelocity) * m_maxDesiredVelocity * (distance / m_slowingRadius) * 0.5f;
	}
	else
	{
		//truncate desired velocity
		if (desiredVelocity.length() > m_maxDesiredVelocity)
		{
			desiredVelocity = plNormalize(desiredVelocity) * m_maxDesiredVelocity;
		}
	}
	
	m_steeringForce = desiredVelocity - m_currentVelocity;

	//truncate steering force
	if (m_steeringForce.length() > m_maxDesiredSteering)
	{
		m_steeringForce = plNormalize(m_steeringForce) * m_maxDesiredSteering;
	}

	//how strong influential the seek force is on the final steering force
	m_steeringForce *= m_seekStrength;

	//update velocity and position
	//m_currentVelocity += m_steeringForce * _dt;

	/*m_currentPosition += m_currentVelocity * _dt;
	m_shape->setPosition(m_currentPosition);*/


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

	m_arrivalCircle.setPosition(m_currentPosition);
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
	/*m_currentVelocity += m_steeringForce * _dt;

	m_currentPosition += m_currentVelocity * _dt;
	m_shape->setPosition(m_currentPosition);*/


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

	/*m_currentVelocity += m_steeringForce * _dt;

	if (m_currentVelocity.length() > m_maxDesiredVelocity)
	{
		m_currentVelocity = plNormalize(m_currentVelocity) * m_maxDesiredVelocity;
	}*/

	/*m_currentPosition += m_currentVelocity * _dt;
	m_shape->setPosition(m_currentPosition);*/

	//debug
	m_wanderCircle.setPosition(wanderOrigin);

	m_wanderLine[0].position = wanderOrigin;
	m_wanderLine[0].color = sf::Color::Magenta;

	m_wanderLine[1].position = wanderTarget;
	m_wanderLine[1].color = sf::Color::Magenta;

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

void CCharacter::Pursuit(sf::Vector2f _targetVelocity, sf::Vector2f _targetPosition, float _dt)
{
	//if X position is close in line with the targets X position seek the targets current position
	if (m_currentPosition.x < _targetPosition.x + m_seekRange && m_currentPosition.x > _targetPosition.x - m_seekRange)
	{
		Seek(_targetPosition, _dt);
		//printf("seek X\n");
		/*printf("Current Pos: %f, %f\n", m_currentPosition.x, m_currentPosition.y);
		printf("target Pos: %f, %f\n", _targetPosition.x, _targetPosition.y);*/

		m_pursuitLine[0].position = m_shape->getPosition();
		m_pursuitLine[0].color = sf::Color::Red;

		m_pursuitLine[1].position = _targetPosition;
		m_pursuitLine[1].color = sf::Color::Red;
	}
	//if Y position is close in line with the targets Y position seek the targets current position
	else if (m_currentPosition.y < _targetPosition.y + m_seekRange && m_currentPosition.y > _targetPosition.y - m_seekRange)
	{
		Seek(_targetPosition, _dt);
		//printf("seek Y\n");

		m_pursuitLine[0].position = m_shape->getPosition();
		m_pursuitLine[0].color = sf::Color::Red;

		m_pursuitLine[1].position = _targetPosition;
		m_pursuitLine[1].color = sf::Color::Red;
	}
	//else seek the target future positon
	else
	{
		sf::Vector2f newTargetPosition = _targetPosition + (plNormalize(_targetVelocity) * 40.0f);
		Seek(newTargetPosition, _dt);
		//printf("Pursue\n");
		/*printf("Current Pos: %f, %f\n", _targetPosition.x, _targetPosition.y);
		printf("New Pos: %f, %f\n", newTargetPosition.x, newTargetPosition.y);*/

		m_pursuitLine[0].position = m_shape->getPosition();
		m_pursuitLine[0].color = sf::Color::Red;

		m_pursuitLine[1].position = newTargetPosition;
		m_pursuitLine[1].color = sf::Color::Red;
	}

}

void CCharacter::Evade(sf::Vector2f _targetVelocity, sf::Vector2f _targetPosition, float _dt)
{
	//if X position is close in line with the targets X position flee the targets current position
	if (m_currentPosition.x < _targetPosition.x + m_seekRange && m_currentPosition.x > _targetPosition.x - m_seekRange)
	{
		Flee(_targetPosition, _dt);
		//printf("flee X\n");
		/*printf("Current Pos: %f, %f\n", m_currentPosition.x, m_currentPosition.y);
		printf("target Pos: %f, %f\n", _targetPosition.x, _targetPosition.y);*/

		m_pursuitLine[0].position = m_shape->getPosition();
		m_pursuitLine[0].color = sf::Color::Red;

		m_pursuitLine[1].position = _targetPosition;
		m_pursuitLine[1].color = sf::Color::Red;
	}
	//if Y position is close in line with the targets Y position flee the targets current position
	else if (m_currentPosition.y < _targetPosition.y + m_seekRange && m_currentPosition.y > _targetPosition.y - m_seekRange)
	{
		Flee(_targetPosition, _dt);
		//printf("flee Y\n");

		m_pursuitLine[0].position = m_shape->getPosition();
		m_pursuitLine[0].color = sf::Color::Red;

		m_pursuitLine[1].position = _targetPosition;
		m_pursuitLine[1].color = sf::Color::Red;
	}
	//else flee the target future positon
	else
	{
		sf::Vector2f newTargetPosition = _targetPosition + (plNormalize(_targetVelocity) * 40.0f);
		Flee(newTargetPosition, _dt);
		//printf("evade\n");
		/*printf("Current Pos: %f, %f\n", _targetPosition.x, _targetPosition.y);
		printf("New Pos: %f, %f\n", newTargetPosition.x, newTargetPosition.y);*/

		m_pursuitLine[0].position = m_shape->getPosition();
		m_pursuitLine[0].color = sf::Color::Red;

		m_pursuitLine[1].position = newTargetPosition;
		m_pursuitLine[1].color = sf::Color::Red;
	}
}

void CCharacter::Separate(std::vector<CCharacter*>& _characters, float _dt)
{
	//setup sum variables
	sf::Vector2f sumSeparate = sf::Vector2f(0.0f, 0.0f);
	int separateCount = 0;

	//go through all potential neighbouring characters with in the neighbourhood radius
	for (int i = 0; i < _characters.size(); i++)
	{
		sf::Vector2f move = m_currentPosition - _characters[i]->GetCurrentPosition();
		if (move.length() < m_neighbourhoodRadius)
		{
			move = plNormalize(move);
			sumSeparate += move;
			separateCount += 1;
		}
	}

	if (separateCount == 0)
	{
		m_separateSteeringForce = sf::Vector2f(0.0f, 0.0f);
		return;
	}

	//calculate desired velocity from the sum
	sumSeparate = plDivVecByScalar(sumSeparate, separateCount);
	sumSeparate = plNormalize(sumSeparate) * m_maxSpeed;

	//calculate steering force
	m_separateSteeringForce = sumSeparate - m_currentVelocity;

	//truncate and scale
	if (m_separateSteeringForce.length() > m_maxDesiredSteering)
	{
		m_separateSteeringForce = plNormalize(m_separateSteeringForce) * m_maxDesiredSteering;
	}
	
	m_separateSteeringForce *= m_separateStrength;



}

void CCharacter::Cohesion(std::vector<CCharacter*>& _characters, float _dt)
{
	//the centre of mass the characters are drawn too and a count for num of neighbours
	sf::Vector2f sumCohesion = sf::Vector2f(0.0f, 0.0f);
	int cohesionCount = 0;

	//go through all potential neighbouring characters with in the neighbourhood radius
	for (int i = 0; i < _characters.size(); i++)
	{
		if (_characters[i] == this)
		{
			continue;
		}

		sf::Vector2f position = m_currentPosition - _characters[i]->GetCurrentPosition();
		if (position.length() < m_neighbourhoodRadius)
		{
			//move = plNormalize(move);
			sumCohesion += _characters[i]->GetCurrentPosition();
			cohesionCount++;
		}
	}

	//average the sum of neighbour positions
	sumCohesion = plDivVecByScalar(sumCohesion, cohesionCount);

	sumCohesion = plNormalize(sumCohesion) * m_maxSpeed;

	m_cohesionSteeringForce = sumCohesion - m_currentVelocity;

	//truncate and scale
	if (m_cohesionSteeringForce.length() > m_maxDesiredSteering)
	{
		m_cohesionSteeringForce = plNormalize(m_cohesionSteeringForce) * m_maxDesiredSteering;
	}

	m_cohesionSteeringForce *= m_cohesionStrength;
}

void CCharacter::Alignment(std::vector<CCharacter*>& _characters, float _dt)
{
	//the centre of mass the characters are drawn too and a count for num of neighbours
	sf::Vector2f sumAlignment = sf::Vector2f(0.0f, 0.0f);
	int alignmentCount = 0;

	//go through all potential neighbouring characters with in the neighbourhood radius
	for (int i = 0; i < _characters.size(); i++)
	{
		if (_characters[i] == this)
		{
			continue;
		}

		sf::Vector2f position = m_currentPosition - _characters[i]->GetCurrentPosition();
		if (position.length() < m_neighbourhoodRadius)
		{
			//move = plNormalize(move);
			sumAlignment += _characters[i]->GetCurrentVelocity();
			alignmentCount++;
		}
	}

	//average the sum of neighbour positions
	sumAlignment = plDivVecByScalar(sumAlignment, alignmentCount);

	sumAlignment = plNormalize(sumAlignment) * m_maxSpeed;

	m_alignmentSteeringForce = sumAlignment - m_currentVelocity;

	//truncate and scale
	if (m_alignmentSteeringForce.length() > m_maxDesiredSteering)
	{
		m_alignmentSteeringForce = plNormalize(m_alignmentSteeringForce) * m_maxDesiredSteering;
	}

	m_alignmentSteeringForce *= m_alignmentStrength;
}

