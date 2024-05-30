#include <SFML/Graphics.hpp>
#include <cmath>

class Ball {
public:
	sf::CircleShape shape;
	sf::Vector2f velocity;
	static constexpr float radius = 20.0f;
	static constexpr float gravity = 0.98f;

	Ball(float x, float y) : velocity(0.0f, 0.0f) {
		shape.setRadius(radius);
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(x, y);
		shape.setOrigin(radius, radius);
	}

	void update(float delta, float groundHeight) {
		velocity.y += gravity;
		shape.move(velocity * delta);

		// Check collision with the ground
		if (shape.getPosition().y + radius * 2 > groundHeight) {
			shape.setPosition(shape.getPosition().x, groundHeight - radius * 2);
			velocity.y *= -0.9;
		}
	}

	void setPosition(float x, float y) {
		shape.setPosition(x, y);
	}

	sf::Vector2f getPosition() const {
		return shape.getPosition();
	}

	void applyForce(const sf::Vector2f& force, float delta) {
		velocity += force * delta;
	}

	void setPositionDirectly(sf::Vector2f position) {
		shape.setPosition(position);
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
			ball1.setPositionDirectly(pos2 - direction * maxLength);
		}

		shape.setPosition(pos1);
		shape.setRotation(angle);
		shape.setSize(sf::Vector2f(currentLength, 5));
	}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Balls and Spring Simulation");
	float groundHeight = 580.f;
	Ball ball1(300.f, 300.f);
	Ball ball2(500.f, 300.f);
	Spring spring(0.1f, 200.f, 300.f);

	bool draggingBall1 = false, draggingBall2 = false;
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
				if (ball1.shape.getGlobalBounds().contains(mousePosition)) {
					draggingBall1 = true;
				}
				if (ball2.shape.getGlobalBounds().contains(mousePosition)) {
					draggingBall2 = true;
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				draggingBall1 = false;
				draggingBall2 = false;
			}
			else if (event.type == sf::Event::MouseMoved && (draggingBall1 || draggingBall2)) {
				mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
				if (draggingBall1) {
					ball1.setPosition(mousePosition.x, mousePosition.y);
				}
				if (draggingBall2) {
					ball2.setPosition(mousePosition.x, mousePosition.y);
				}
			}
		}

		float delta = clock.restart().asSeconds();
		if (!draggingBall1) ball1.update(delta, groundHeight);
		if (!draggingBall2) ball2.update(delta, groundHeight);
		spring.update(ball1, ball2);

		window.clear();
		window.draw(spring.shape);
		window.draw(ball1.shape);
		window.draw(ball2.shape);

		sf::RectangleShape ground(sf::Vector2f(800.f, 20.f));
		ground.setPosition(0, groundHeight);
		ground.setFillColor(sf::Color::Green);
		window.draw(ground);

		window.display();
	}

	return 0;
}
