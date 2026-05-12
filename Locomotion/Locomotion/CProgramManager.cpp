#include "CProgramManager.h"

CProgramManager::CProgramManager()
{
	m_window = new sf::RenderWindow(sf::VideoMode({ 800, 800 }), "Locomotion!");
}

CProgramManager::~CProgramManager()
{
    delete m_window;
    m_window = nullptr;

    /*delete m_character;
    m_character = nullptr;*/

    delete m_target;
    m_target = nullptr;

    delete m_characterManager;
    m_characterManager = nullptr;
}

void CProgramManager::Initialize()
{
    //spawn a character
    //m_character = new CCharacter(sf::Vector2f(500.0f, 500.0f), Behavior::SEEK);

    m_target = new CTarget(sf::Vector2f(500.0f, 500.0f));

    m_characterManager = new CCharacterManager();

}

void CProgramManager::RunProgram()
{
    while (m_window->isOpen())
    {
        sf::Time deltaTime = m_clock.restart();
        float dt = deltaTime.asSeconds();

        while (const std::optional event = m_window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                m_window->close();

            m_characterManager->InputUpdate(event, m_window);

            /*if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left)
                {
                    m_characterManager->SpawnCharacter(m_mousePosition);
                }
            }*/
        }

        m_mousePosition = sf::Vector2f(sf::Mouse::getPosition(*m_window).x, sf::Mouse::getPosition(*m_window).y);

        //update character behaviour
        //m_character->Seek(mousePosition, dt);
        //m_character->Flee(mousePosition, dt);
        //m_character->Wander(dt);

        //m_character->Seek(m_target->GetCurrentPosition(), dt);
        //m_character->Pursuit(m_target->GetCurrentVelocity(), m_target->GetCurrentPosition(), dt);
        //m_character->Evade(m_target->GetCurrentVelocity(), m_target->GetCurrentPosition(), dt);

        //printf("%f, %f\n", m_target->GetCurrentPosition().x, m_target->GetCurrentPosition().y);

        //m_character->Update();

        m_characterManager->Update(m_target->GetCurrentPosition(), m_target->GetCurrentVelocity(), dt);

        m_target->Update(dt);
        m_target->Update();

        Draw();
    }
}

void CProgramManager::Draw()
{
    m_window->clear();

    //draw character
    //m_character->Draw(m_window);

    m_target->Draw(m_window);

    m_characterManager->Draw(m_window);

    m_window->display();
}

