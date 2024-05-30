#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

const float SCALE = 30.f;
const float DEGTORAD = 0.0174532925199432957f;

b2Body* createBox(b2World& world, float x, float y, float width, float height, bool dynamic, float density, float friction) {
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
	bodyDef.type = dynamic ? b2_dynamicBody : b2_staticBody;
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox((width / 2) / SCALE, (height / 2) / SCALE);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* createCircle(b2World& world, float x, float y, float radius, bool dynamic, float density, float friction) {
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(x / SCALE, y / SCALE);
	bodyDef.type = dynamic ? b2_dynamicBody : b2_staticBody;
	b2Body* body = world.CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius / SCALE;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	body->CreateFixture(&fixtureDef);

	return body;
}

b2RevoluteJoint* connectBodies(b2World& world, b2Body* bodyA, b2Body* bodyB, const b2Vec2& anchor, float lowerAngle, float upperAngle) {
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(bodyA, bodyB, bodyA->GetWorldPoint(anchor));
	jointDef.lowerAngle = lowerAngle * DEGTORAD;
	jointDef.upperAngle = upperAngle * DEGTORAD;
	jointDef.enableLimit = true;

	return static_cast<b2RevoluteJoint*>(world.CreateJoint(&jointDef));
}

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Ragdoll Simulation");
	window.setFramerateLimit(60);

	b2Vec2 gravity(0.f, 9.8f);
	b2World world(gravity);

	// Create ground
	b2Body* ground = createBox(world, 400, 590, 800, 40, false, 0.f, 0.3f);

	// Create ragdoll parts
	b2Body* head = createCircle(world, 400, 50, 15, true, 1.f, 0.3f);
	b2Body* torso = createBox(world, 400, 100, 15, 50, true, 1.f, 0.3f);
	b2Body* leftArm = createBox(world, 350, 100, 40, 10, true, 1.f, 0.3f);
	b2Body* rightArm = createBox(world, 450, 100, 40, 10, true, 1.f, 0.3f);
	b2Body* leftLeg = createBox(world, 390, 200, 10, 50, true, 1.f, 0.3f);
	b2Body* rightLeg = createBox(world, 410, 200, 10, 50, true, 1.f, 0.3f);

	connectBodies(world, torso, head, b2Vec2(0.f, -1.5f), -30.f, 30.f);
	connectBodies(world, torso, leftArm, b2Vec2(-0.75f, -1.f), -90.f, 90.f);
	connectBodies(world, torso, rightArm, b2Vec2(0.75f, -1.f), -90.f, 90.f);
	connectBodies(world, torso, leftLeg, b2Vec2(-0.4f, 2.f), -45.f, 45.f);
	connectBodies(world, torso, rightLeg, b2Vec2(0.4f, 2.f), -45.f, 45.f);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		world.Step(1 / 60.f, 8, 3);

		window.clear(sf::Color::White);

		// Draw ground
		sf::RectangleShape groundShape(sf::Vector2f(800.f, 40.f));
		groundShape.setFillColor(sf::Color::Green);
		groundShape.setOrigin(400.f, 20.f);
		groundShape.setPosition(400.f, 590.f);
		window.draw(groundShape);

		// Draw ragdoll parts
		auto drawBody = [&](b2Body* body, float width, float height) {
			sf::RectangleShape shape(sf::Vector2f(width, height));
			shape.setOrigin(width / 2, height / 2);
			shape.setPosition(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
			shape.setRotation(body->GetAngle() * 180 / b2_pi);
			shape.setFillColor(sf::Color::Red);
			window.draw(shape);
			};

		drawBody(torso, 30.f, 100.f);
		drawBody(leftArm, 80.f, 20.f);
		drawBody(rightArm, 80.f, 20.f);
		drawBody(leftLeg, 20.f, 100.f);
		drawBody(rightLeg, 20.f, 100.f);

		sf::CircleShape headShape(30.f);
		headShape.setOrigin(15.f, 15.f);
		headShape.setPosition(head->GetPosition().x * SCALE, head->GetPosition().y * SCALE);
		headShape.setRotation(head->GetAngle() * 180 / b2_pi);
		headShape.setFillColor(sf::Color::Blue);
		window.draw(headShape);

		window.display();
	}

	return 0;
}
