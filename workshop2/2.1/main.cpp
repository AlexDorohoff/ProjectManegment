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
    // Создаём псевдо-устройство для получения случайного зерна.
    std::random_device device;
    // Получаем случайное зерно последовательности
    generator.engine.seed(device());
}

// Генерирует целое число в диапазоне [minValue, maxValue)
unsigned random(PRNG &generator, unsigned minValue, unsigned maxValue)
{
    // Проверяем корректность аргументов
    assert(minValue < maxValue);

    // Создаём распределение
    std::uniform_int_distribution<unsigned> distribution(minValue, maxValue);

    // Вычисляем псевдослучайное число: вызовем распределение как функцию,
    //  передав генератор произвольных целых чисел как аргумент.
    return distribution(generator.engine);
}

// Генерирует число с плавающей точкой в диапазоне [minValue, maxValue)
float getRandomFloat(PRNG &generator, float minValue, float maxValue)
{
    // Проверяем корректность аргументов
    assert(minValue < maxValue);

    // Создаём распределение
    std::uniform_real_distribution<float> distribution(minValue, maxValue);

    // Вычисляем псевдослучайное число: вызовем распределение как функцию,
    //  передав генератор произвольных целых чисел как аргумент.
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
        ball[i].shape.setFillColor(parametrs[i].color);
        ball[i].shape.setPosition(parametrs[i].startPosition);
        ball[i].speed = ballSpeed(generator);
    }
}

void getBallParametrs(BallParameters (&array)[maxNumberOfBalls])
{
    array[0] = {10.f, sf::Color(0x99, 0x33, 0x66), sf::Vector2f{320, 320}};
    array[1] = {20.f, sf::Color(0xFF, 0x99, 0xCC), sf::Vector2f{460, 100}};
    array[2] = {25.f, sf::Color(0x00, 0x00, 0xFF), sf::Vector2f{200, 100}};
    array[3] = {30.f, sf::Color(0x33, 0x33, 0x99), sf::Vector2f{400, 300}};
    array[4] = {40.f, sf::Color(0x00, 0xFF, 0xFF), sf::Vector2f{500, 500}};
    array[5] = {35.f, sf::Color(0xFF, 0xFF, 0x00), sf::Vector2f{600, 500}};
    array[6] = {25.f, sf::Color(0xFF, 0x99, 0x00), sf::Vector2f{100, 400}};
    array[7] = {15.f, sf::Color(0xFF, 0x00, 0x00), sf::Vector2f{250, 400}};
    array[8] = {20.f, sf::Color(0xFF, 0xFF, 0xFF), sf::Vector2f{250, 380}};
    array[9] = {10.f, sf::Color(0x96, 0x96, 0x96), sf::Vector2f{100, 200}};
}

void setBallNewPosition(Ball (&arrayOfBalls)[maxNumberOfBalls], sf::Clock &clock)
{
    const float deltaTime = clock.restart().asSeconds();

    for (int i = 0; i < maxNumberOfBalls; i++)
    {
        sf::Vector2f position = arrayOfBalls[i].shape.getPosition();
        position += arrayOfBalls[i].speed * deltaTime;

        if ((position.x + 2 * arrayOfBalls[i].shape.getRadius() >= WINDOW_WIDTH) && (arrayOfBalls[i].speed.x > 0))
        {
            arrayOfBalls[i].speed.x = -arrayOfBalls[i].speed.x;
            std::cout << "blonk!/n";
        }
        if ((position.x < 0) && (arrayOfBalls[i].speed.x < 0))
        {
            arrayOfBalls[i].speed.x = -arrayOfBalls[i].speed.x;
            std::cout << "blonk!/n";
        }
        if ((position.y + 2 * arrayOfBalls[i].shape.getRadius() >= WINDOW_HEIGHT) && (arrayOfBalls[i].speed.y > 0))
        {
            arrayOfBalls[i].speed.y = -arrayOfBalls[i].speed.y;
            std::cout << "blonk!/n";
        }
        if ((position.y < 0) && (arrayOfBalls[i].speed.y < 0))
        {
            arrayOfBalls[i].speed.y = -arrayOfBalls[i].speed.y;
            std::cout << "blonk!/n";
        }

        arrayOfBalls[i].shape.setPosition(position);
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

        setBallNewPosition(arrayOfBalls, clock);
        window.clear();
        drawBalls(arrayOfBalls, window);
        window.display();
        pollEvents(window);
    }
}
