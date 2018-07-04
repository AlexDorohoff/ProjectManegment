#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
constexpr unsigned maxNumberOfBalls = 10;

struct Ball
{
    sf::CircleShape shape;
    sf::Vector2f speed;
};

struct BallParameters
{
    float radius;
    sf::Color color;
    sf::Vector2f startPosition;
    sf::Vector2f speed;
};

struct PRNG
{
    std::mt19937 engine;
};

void initGenerator(PRNG &generator)
{
    std::random_device device;
    generator.engine.seed(device());
}

// Генерирует число с плавающей точкой в диапазоне [minValue, maxValue)
float getRandomFloat(PRNG &generator, float minValue, float maxValue)
{
    assert(minValue < maxValue);

    std::uniform_real_distribution<float> distribution(minValue, maxValue);

    return distribution(generator.engine);
}

sf::Vector2f ballSpeed(PRNG &generator)
{
    return sf::Vector2f({getRandomFloat(generator, -250.f, 250.f), getRandomFloat(generator, -250.f, 250.f)});
}

void initBall(Ball (&ball)[maxNumberOfBalls], BallParameters (&parametrs)[maxNumberOfBalls])
{
    PRNG generator;
    initGenerator(generator);

    for (int i = 0; i < maxNumberOfBalls; i++)
    {
        ball[i].shape.setRadius(parametrs[i].radius);
        ball[i].shape.setOrigin(parametrs[i].radius, parametrs[i].radius);
        ball[i].shape.setFillColor(parametrs[i].color);
        ball[i].shape.setPosition(parametrs[i].startPosition);
        ball[i].speed = ballSpeed(generator);
    }
}

void getBallParametrs(BallParameters (&array)[maxNumberOfBalls])
{
    array[0] = {10.f, sf::Color(0x99, 0x33, 0x66), sf::Vector2f{20, 20}};
    array[1] = {70.f, sf::Color(0xFF, 0x99, 0xCC), sf::Vector2f{120, 100}};
    array[2] = {34.f, sf::Color(0xFF, 0x00, 0x00), sf::Vector2f{200, 200}};
    array[3] = {30.f, sf::Color(0x33, 0x33, 0x99), sf::Vector2f{300, 300}};
    array[4] = {30.f, sf::Color(0x00, 0xFF, 0xFF), sf::Vector2f{400, 400}};
    array[5] = {35.f, sf::Color(0xFF, 0xFF, 0x00), sf::Vector2f{500, 500}};
    array[6] = {25.f, sf::Color(0xFF, 0x99, 0x00), sf::Vector2f{600, 600}};
    array[7] = {15.f, sf::Color(0x00, 0x00, 0xFF), sf::Vector2f{700, 700}};
    array[8] = {20.f, sf::Color(0xFF, 0xFF, 0xFF), sf::Vector2f{750, 750}};
    array[9] = {10.f, sf::Color(0x96, 0x96, 0x96), sf::Vector2f{100, 400}};
}

void collisionWall(Ball (&arrayOfBalls)[maxNumberOfBalls], sf::Clock &clock)
{
    const float deltaTime = clock.restart().asSeconds();
    for (int i = 0; i < maxNumberOfBalls; i++)
    {
        sf::Vector2f position = arrayOfBalls[i].shape.getPosition();
        position += arrayOfBalls[i].speed * deltaTime;

        if ((position.x + arrayOfBalls[i].shape.getRadius() >= WINDOW_WIDTH) && (arrayOfBalls[i].speed.x > 0))
        {
            arrayOfBalls[i].speed.x = -arrayOfBalls[i].speed.x;
        }
        if (((position.x - arrayOfBalls[i].shape.getRadius()) < 0) && (arrayOfBalls[i].speed.x < 0))
        {
            arrayOfBalls[i].speed.x = -arrayOfBalls[i].speed.x;
        }
        if ((position.y + arrayOfBalls[i].shape.getRadius() >= WINDOW_HEIGHT) && (arrayOfBalls[i].speed.y > 0))
        {
            arrayOfBalls[i].speed.y = -arrayOfBalls[i].speed.y;
        }
        if (((position.y - arrayOfBalls[i].shape.getRadius()) < 0) && (arrayOfBalls[i].speed.y < 0))
        {
            arrayOfBalls[i].speed.y = -arrayOfBalls[i].speed.y;
        }

        arrayOfBalls[i].shape.setPosition(position);
    }
}

float length(const sf::Vector2f &vector)
{
    return std::hypot(vector.x, vector.y);
}

void collisionBall(Ball (&arrayOfBalls)[maxNumberOfBalls], sf::Clock &clock)
{
    const float deltaTime = clock.restart().asSeconds();
    for (int i = 0; i < maxNumberOfBalls; ++i)
    {
        for (int j = i + 1; j < maxNumberOfBalls; ++j)
        {
            if (length(arrayOfBalls[i].shape.getPosition() - arrayOfBalls[j].shape.getPosition()) <= (arrayOfBalls[i].shape.getRadius() + arrayOfBalls[j].shape.getRadius()))
            {
                sf::Vector2f deltaI = arrayOfBalls[i].shape.getPosition() - arrayOfBalls[j].shape.getPosition();
                sf::Vector2f speedI = arrayOfBalls[i].speed - arrayOfBalls[j].speed;

                sf::Vector2f deltaJ = arrayOfBalls[j].shape.getPosition() - arrayOfBalls[i].shape.getPosition();
                sf::Vector2f speedJ = arrayOfBalls[j].speed - arrayOfBalls[i].speed;

                arrayOfBalls[i].speed = arrayOfBalls[i].speed -
                                        ((speedI.x * deltaI.x + speedI.y * deltaI.y) / (deltaI.x * deltaI.x + deltaI.y * deltaI.y)) * deltaI;
                arrayOfBalls[j].speed = arrayOfBalls[j].speed -
                                        ((speedJ.x * deltaJ.x + speedJ.y * deltaJ.y) / (deltaJ.x * deltaJ.x + deltaJ.y * deltaJ.y)) * deltaJ;
            }
        }
    }
}

void drawBalls(Ball (&arrayOfBalls)[maxNumberOfBalls], sf::RenderWindow &window)
{
    for (int i = 0; i < maxNumberOfBalls; i++)
    {
        window.draw(arrayOfBalls[i].shape);
    }
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
    PRNG generator;
    initGenerator(generator);

    BallParameters arrayOfParams[maxNumberOfBalls];
    getBallParametrs(arrayOfParams);

    Ball arrayOfBalls[maxNumberOfBalls];
    initBall(arrayOfBalls, arrayOfParams);

    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Flying balls");
    sf::Clock clock;

    Ball ball;

    while (window.isOpen())
    {
        sf::Event event;
        collisionWall(arrayOfBalls, clock);
        collisionBall(arrayOfBalls, clock);
        window.clear();
        drawBalls(arrayOfBalls, window);
        window.display();
        pollEvents(window);
    }
}
