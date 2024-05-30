#include <SFML/Graphics.hpp>

class Ball {
public:
	sf::CircleShape shape;
	sf::Vector2f velocity;

	Ball(float radius, float x, float y) : velocity(sf::Vector2f(100.0f, 100.0f)) {
		shape.setRadius(radius);
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(x, y);
	}

	void update(float delta, sf::Vector2u windowSize) {
		shape.move(velocity * delta);

		if (shape.getPosition().x < 0 || shape.getPosition().x + shape.getRadius() * 2 > windowSize.x) {
			velocity.x *= -1;
		}
		if (shape.getPosition().y < 0 || shape.getPosition().y + shape.getRadius() * 2 > windowSize.y) {
			velocity.y *= -1;
		}
	}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Bouncing Ball Simulation");
	Ball ball(20.f, 50.f, 50.f);
	sf::Clock clock;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		sf::Time elapsed = clock.restart();
		float delta = elapsed.asSeconds();

		ball.update(delta, window.getSize());

		window.clear();
		window.draw(ball.shape);
		window.display();
	}

	return 0;
}
