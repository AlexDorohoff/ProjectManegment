#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <iostream>

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

struct Ball
{
    sf::CircleShape shape;
    sf::Vector2f speed;
};

void initBall(Ball &ball)
{
    ball.shape.setRadius(30.f);
    ball.shape.setFillColor(sf::Color(0xFF, 0xFF, 0xFF));
    ball.shape.setPosition(sf::Vector2f{400, 300});
    ball.speed = sf::Vector2f{100.f, 50.f};
}

void drawBall(sf::RenderWindow &window, Ball &ball)
{
    window.draw(ball.shape);
}

void setBallNewPosition(Ball &ball, sf::Clock &clock)
{
    const float deltaTime = clock.restart().asSeconds();

    sf::Vector2f position = ball.shape.getPosition();
    std::cout << position.x << std::endl;
    position += ball.speed * deltaTime;

    if ((position.x + 2 * ball.shape.getRadius() >= WINDOW_WIDTH) && (ball.speed.x > 0))
    {
        ball.speed.x = -ball.speed.x;
    }
    if ((position.x < 0) && (ball.speed.x < 0))
    {
        ball.speed.x = -ball.speed.x;
    }
    if ((position.y + 2 * ball.shape.getRadius() >= WINDOW_HEIGHT) && (ball.speed.y > 0))
    {
        ball.speed.y = -ball.speed.y;
    }
    if ((position.y < 0) && (ball.speed.y < 0))
    {
        ball.speed.y = -ball.speed.y;
    }

    ball.shape.setPosition(position);
}

void pollEvents(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;
        }
    }
}

int main()
{

    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Moving ballZ");
    sf::Clock clock;

    Ball ball;
    initBall(ball);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            window.clear();
            setBallNewPosition(ball, clock);
            drawBall(window, ball);
            pollEvents(window);
            window.display();
        }
    }
}
