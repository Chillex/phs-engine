#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "FPSCounter.h"

int main()
{
	sf::VideoMode vm(1280, 720);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(vm, "Collision Detector", sf::Style::Default, settings);

	sf::View gameView(sf::FloatRect(0, 0, 1920, 1080));
	sf::View hudView(sf::FloatRect(0, 0, 1920, 1080));

	FPSCounter fpsCounter("Assets/Font/digital_counter_7.ttf");
	sf::Color clearColor(0, 0, 0);

	sf::Clock deltaClock;
	sf::Time dt;
	while (window.isOpen())
	{
		dt = deltaClock.restart();
		window.setView(gameView);

		sf::Vector2i mousePosPixel = sf::Mouse::getPosition(window);
		sf::Vector2f mousePosWorld = window.mapPixelToCoords(mousePosPixel);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
				window.close();
		}

		/* UPDATES */
		fpsCounter.Update(dt);

		/* DRAW CALLS */
		window.clear(clearColor);

		window.setView(hudView);
		fpsCounter.Draw(window);

		window.display();
	}

	return 0;
}