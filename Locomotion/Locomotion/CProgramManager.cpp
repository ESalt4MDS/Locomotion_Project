#include "CProgramManager.h"

CProgramManager::CProgramManager()
{
	m_window = new sf::RenderWindow(sf::VideoMode({ 800, 800 }), "Locomotion!");
}

CProgramManager::~CProgramManager()
{
    delete m_window;
    m_window = nullptr;

    delete m_character;
    m_character = nullptr;
}

void CProgramManager::Initialize()
{
    //spawn a character
    m_character = new CCharacter(sf::Vector2f(100.0f, 100.0f));


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
        }

        sf::Vector2f mousePosition;
        mousePosition = sf::Vector2f(sf::Mouse::getPosition(*m_window).x, sf::Mouse::getPosition(*m_window).y);

        //update character behaviour
        m_character->Seek(mousePosition, dt);
        //m_character->Flee(mousePosition, dt);
        //m_character->Wander(dt);

        m_character->Update();

        Draw();
    }
}

void CProgramManager::Draw()
{
    m_window->clear();

    //draw character
    m_character->Draw(m_window);

    m_window->display();
}

