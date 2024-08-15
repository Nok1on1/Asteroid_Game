#include <iostream>
#include <SFML/Graphics.hpp>
#include "Headers/Asteroid.h"
#include <cstdlib>
#include "Headers/bullet.h"
#include "Headers/Player.h"
#include "Headers/UI.h"

Player player;

void clearConsole() {
    system("clear");
}

class Game {
    static inline sf::Clock asteroidClock;

public:
    sf::Clock clock;
    float deltaTime;

public:
    sf::RenderWindow _window{sf::VideoMode(1920, 1200, 1), "Program", sf::Style::Fullscreen};

private:
    sf::Event ev;

public:
    void Gameinit() {
        Asteroid::generateAsteroids(_window, 10, 50);
        player.initPlayer(_window);
        UI::stars.setSize({1620 * 1.5, 760 * 1.5});
        UITextures::starsT.loadFromFile("/home/noklon/Prog-projects/CLionProjects/Asteroids/Textures/stars.png");
        UI::stars.setTexture(&UITextures::starsT);
    }

    void Gameloop() {
        while (_window.isOpen()) {
            deltaTime = clock.restart().asSeconds();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                player.moveplayer(sf::Keyboard::Up, deltaTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                player.moveplayer(sf::Keyboard::Down, deltaTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                player.moveplayer(sf::Keyboard::Left, deltaTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                player.moveplayer(sf::Keyboard::Right, deltaTime);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                Bullet::initBullet(player);
            }
            _window.clear();
            _window.draw(UI::stars);

            Asteroid::updateCordinates(_window, asteroidClock);
            player.updateCordinates(_window);
            Bullet::updateBullets(_window);
            Asteroid::collisions();
            Bullet::collisions(Asteroid::asteroids);

            for (const auto &bullet: Bullet::bullets) {
                _window.draw(bullet.shape);
            }
            for (const auto &asteroid: Asteroid::asteroids) {
                _window.draw(asteroid.shape);
                // std::cout << asteroid.shape.getPosition().x << " "<<asteroid.shape.getPosition().y << std::endl;
            }
            _window.draw(player.shape);


            _window.display();
        }
    }

    void GameEvents() {
        while (_window.pollEvent(ev)) {
            switch (ev.type) {
                case sf::Event::Closed:
                    _window.close();
                    break;
                case sf::Event::KeyPressed:

                    break;

                default:
                    break;
            }
        }
    }
};

int main() {
    Game game;
    Asteroid asteroid;
    game.Gameinit();
    game.Gameloop();
}
