#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

class Ball {
public:
	sf::CircleShape shape;
	sf::Vector2f velocity;
	static constexpr float radius = 20.0f;
	static constexpr float gravity = 0.98f;
	bool isStatic;
	static constexpr float maxSpeed = 500.0f;

	Ball(float x, float y, bool staticFlag) : velocity(0.0f, 0.0f), isStatic(staticFlag) {
		shape.setRadius(radius);
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(x, y);
		shape.setOrigin(radius, radius);
	}

	void update(float delta, float groundHeight) {
		if (!isStatic) {
			velocity.y += gravity;
			velocity.y = std::min(velocity.y, maxSpeed);
			shape.move(velocity * delta);

			if (shape.getPosition().y + radius * 2 > groundHeight) {
				shape.setPosition(shape.getPosition().x, groundHeight - radius * 2);
				velocity.y *= -0.9;
			}
		}
	}

	void setPosition(float x, float y) {
		if (!isStatic) {
			shape.setPosition(x, y);
		}
	}

	sf::Vector2f getPosition() const {
		return shape.getPosition();
	}

	void applyForce(const sf::Vector2f& force, float delta) {
		if (!isStatic) {
			velocity += force * delta;
			float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
			if (speed > maxSpeed) {
				velocity *= maxSpeed / speed;
			}
		}
	}


	void setPositionDirectly(sf::Vector2f position) {
		if (!isStatic) {
			shape.setPosition(position);
		}
	}
};

class Spring {
public:
	sf::RectangleShape shape;
	float stiffness;
	float restLength;
	float maxLength;

	Spring(float k, float l, float maxL) : stiffness(k), restLength(l), maxLength(maxL) {
		shape.setFillColor(sf::Color::Blue);
		shape.setSize(sf::Vector2f(l, 5));
		shape.setOrigin(0, 2.5f);
	}

	void update(Ball& ball1, Ball& ball2) {
		sf::Vector2f pos1 = ball1.getPosition();
		sf::Vector2f pos2 = ball2.getPosition();
		sf::Vector2f diff = pos2 - pos1;
		float currentLength = std::sqrt(diff.x * diff.x + diff.y * diff.y);
		float angle = std::atan2(diff.y, diff.x) * 180 / 3.14159;

		if (currentLength > maxLength) {
			sf::Vector2f direction = diff / currentLength;
			if (!ball1.isStatic) {
				ball1.setPositionDirectly(pos2 - direction * maxLength);
			}
			if (!ball2.isStatic) {
				ball2.setPositionDirectly(pos1 + direction * maxLength);
			}
		}

		shape.setPosition(pos1);
		shape.setRotation(angle);
		shape.setSize(sf::Vector2f(currentLength, 5));
	}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Balls and Spring Simulation");
	float groundHeight = 580.f;
	Ball ball1(300.f, 300.f, true);
	Ball ball2(500.f, 300.f, false);
	Spring spring(0.1f, 200.f, 200.f);

	bool draggingBall2 = false;
	sf::Vector2f mousePosition;

	sf::Clock clock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
				if (ball2.shape.getGlobalBounds().contains(mousePosition)) {
					draggingBall2 = true;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				draggingBall2 = false;
			}
			else if (event.type == sf::Event::MouseMoved && draggingBall2) {
				mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
				ball2.setPosition(mousePosition.x, mousePosition.y);
			}
		}

		float delta = clock.restart().asSeconds();
		ball1.update(delta, groundHeight);
		ball2.update(delta, groundHeight);
		spring.update(ball1, ball2);

		window.clear();
		window.draw(spring.shape);
		window.draw(ball1.shape);
		window.draw(ball2.shape);

		// Draw the ground
		sf::RectangleShape ground(sf::Vector2f(800.f, 20.f));
		ground.setPosition(0, groundHeight);
		ground.setFillColor(sf::Color::Green);
		window.draw(ground);

		window.display();
	}

	return 0;
}
