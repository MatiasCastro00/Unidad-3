#include <SFML/Graphics.hpp>
#include <iostream>

class Square {
public:
	sf::RectangleShape shape;
	sf::Vector2f velocity;
	float acceleration = 600.0f;

	Square(float size, float x, float y) {
		shape.setSize(sf::Vector2f(size, size));
		shape.setFillColor(sf::Color::Blue);
		shape.setPosition(x, y);
	}

	void applyForce(const sf::Vector2f& force, float deltaTime) {
		sf::Vector2f acceleration = force;
		velocity += acceleration * deltaTime;
		shape.move(velocity * deltaTime);
	}

	void update(float deltaTime) {
		float friction = 0.95;
		velocity *= friction;
	}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Move Square with Forces");
	window.setFramerateLimit(60);

	Square square(50.0f, 375.0f, 275.0f);

	sf::Clock clock;
	while (window.isOpen()) {
		sf::Event event;
		sf::Vector2f force(0.0f, 0.0f);
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		float deltaTime = clock.restart().asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			force.x -= square.acceleration;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			force.x += square.acceleration;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			force.y -= square.acceleration;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			force.y += square.acceleration;
		}

		square.applyForce(force, deltaTime);
		square.update(deltaTime);

		window.clear();
		window.draw(square.shape);
		window.display();
	}

	return 0;
}
