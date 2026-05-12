#include "CProgramManager.h"
#include "PhysicsLibrary.h"

CProgramManager::CProgramManager()
{
	m_window = new sf::RenderWindow(sf::VideoMode({ 800, 800 }), "Locomotion!");
}

CProgramManager::~CProgramManager()
{
    delete m_window;
    m_window = nullptr;

    delete m_target;
    m_target = nullptr;

    delete m_characterManager;
    m_characterManager = nullptr;
}

void CProgramManager::Initialize()
{
    //spawn a character
    m_target = new CTarget(sf::Vector2f(WindowSize.x / 2.0f, WindowSize.y / 2.0f));

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

        }

        m_mousePosition = sf::Vector2f(sf::Mouse::getPosition(*m_window).x, sf::Mouse::getPosition(*m_window).y);

        m_characterManager->Update(m_target->GetCurrentPosition(), m_target->GetCurrentVelocity(), dt);

        m_target->Update(dt);
        m_target->Update();

        Draw();
    }
}

void CProgramManager::Draw()
{
    m_window->clear();

    m_target->Draw(m_window);

    m_characterManager->Draw(m_window);

    m_window->display();
}

