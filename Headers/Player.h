#pragma once
#include <iostream>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include "extendedmath.h"

class Player {
public:
    static inline sf::Clock velocityClock;
    sf::ConvexShape shape{4};
    sf::Vector2f dir{0, -1};
    sf::Vector2f velocity{0, 0};
    float acc{0};

    void initPlayer(const sf::RenderWindow &window) {
        shape.setPoint(0, sf::Vector2f(0, -60));
        shape.setPoint(1, sf::Vector2f(30, 30));
        shape.setPoint(2, sf::Vector2f(0, 0));
        shape.setPoint(3, sf::Vector2f(-30, 30));
        shape.setFillColor(sf::Color(45, 70, 84));
        shape.setOutlineThickness(4);
        shape.setOutlineColor(sf::Color(36, 59, 74));
        shape.setPosition(window.getSize().x / 2, window.getSize().y * 9 / 10);
    }

    void updateCordinates(sf::RenderWindow &window) {
        screenBorders(window);
        shape.setPosition(shape.getPosition().x + velocity.x * velocityClock.getElapsedTime().asSeconds(),
                          shape.getPosition().y + velocity.y * velocityClock.getElapsedTime().asSeconds());
        if (velocity != sf::Vector2f{0, 0})
            acc -= acc / 5;
        velocity -= multV2f(9 / 10, velocity);
        velocityClock.restart();
    }

    void moveplayer(sf::Keyboard::Key move, float delta) {
        switch (move) {
            case sf::Keyboard::Right: {
                const double angle = (270 * delta) * M_PI / 180.0;
                dir.x = dir.x * cos(angle) - dir.y * sin(angle);
                dir.y = dir.x * sin(angle) + dir.y * cos(angle);
                shape.rotate(angle * 180 / M_PI);
                break;
            }
            case sf::Keyboard::Left: {
                const double angle = (270 * delta) * M_PI / 180.0;
                dir.x = dir.x * cos(angle) + dir.y * sin(angle);
                dir.y = -dir.x * sin(angle) + dir.y * cos(angle);
                shape.rotate(-angle * 180 / M_PI);
                break;
            }
            case sf::Keyboard::Up:
                if (acc < 0.5)
                    acc += 0.1;
                velocity = addV2f(velocity, multV2f(acc, dir));
                break;
            case sf::Keyboard::Down:
                if (acc < 0.5)
                    acc += 0.1;
            velocity = addV2f(velocity, multV2f(-acc, dir));
            break;
        }
    }

    void screenBorders(sf::RenderWindow &window) {
        if (shape.getPosition().x < 0) {
            velocity.x = -velocity.x/2;
            shape.setPosition(0, shape.getPosition().y);

            //   std::cout << "x0" << std::endl;
        } else if ( shape.getPosition().x > window.getSize().x -   0) {
             velocity.x = - velocity.x/2;
             shape.setPosition(window.getSize().x -   0,  shape.getPosition().y);
        }

        if ( shape.getPosition().y <   0) {
             velocity.y = - velocity.y/2;
             shape.setPosition( shape.getPosition().x,   0);
        } else if ( shape.getPosition().y > window.getSize().y -   0) {
             velocity.y = - velocity.y/2;
             shape.setPosition( shape.getPosition().x, window.getSize().y -   0);
        }
    }
};
