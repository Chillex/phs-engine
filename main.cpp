#include <SFML/Graphics.hpp>

#include "FPSCounter.h"
#include "Simulation.h"

int main()
{
	// seed rand
	srand(time(nullptr));

	sf::VideoMode vm(1280, 720);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(vm, "Collision Detector", sf::Style::Default, settings);

	sf::View gameView(sf::FloatRect(0, 0, 1920, 1080));
	sf::View hudView(sf::FloatRect(0, 0, 1920, 1080));

	FPSCounter fpsCounter("Assets/Font/digital_counter_7.ttf");
	sf::Color clearColor(0, 0, 0);

	Simulation simulation;

	float dt = 1.0f / 60.0f;
	sf::Clock deltaClock;
	sf::Time frameTimer;
	while (window.isOpen())
	{
		frameTimer = deltaClock.restart();
		float frameTime = frameTimer.asSeconds();
		window.setView(gameView);

		sf::Vector2i mousePosPixel = sf::Mouse::getPosition(window);
		sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosPixel);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::F)
				{
					simulation.ToggleFan();
				}

				if (event.key.code == sf::Keyboard::D)
				{
					simulation.ToggleDebugDraw();
				}

				if (event.key.code == sf::Keyboard::B)
				{
					simulation.ToggleBlizzards();
				}

				if (event.key.code == sf::Keyboard::R)
				{
					simulation.ResetBalls();
				}

				if (event.key.code == sf::Keyboard::Space)
				{
					simulation.SpawnBalls(1);
				}
			}
		}

		/* UPDATES */
		fpsCounter.Update(frameTimer);

		// semi-fixed timestep
		while(frameTime > 0.0f)
		{
			float deltaTime = std::min(frameTime, dt);
			simulation.Update(deltaTime);
			frameTime -= deltaTime;
		}

		/* DRAW CALLS */
		window.clear(clearColor);
		simulation.Render(window);

		window.setView(hudView);
		fpsCounter.Draw(window);

		window.display();
	}

	return 0;
}
