#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <vector>
#include "extendedmath.h"

class Bullet {
public:
    sf::CircleShape shape{2};
    static inline sf::Clock clock;
    static inline sf::Clock shoot;
    sf::Vector2f dir;
    static inline std::vector<Bullet> bullets;

    static void initBullet(Player player) {
        if (shoot.getElapsedTime().asSeconds() > 0.5) {
            Bullet bullet;
            bullet.shape.setOrigin(2, 2);
            bullet.dir = player.dir;
            bullet.shape.setPosition(player.shape.getPosition());
            bullets.emplace_back(bullet);
            shoot.restart();
        }
    }

protected:
    bool checkborders(const sf::RenderWindow &window) const {
        return shape.getPosition().x > 0
               && shape.getPosition().x < window.getSize().x
               && shape.getPosition().y > 0
               && shape.getPosition().y < window.getSize().y;
    }

public:
    static void updateBullets(const sf::RenderWindow &window) {
        for (auto &bullet: bullets) {
            if (bullet.checkborders(window))
                bullet.shape.setPosition(
                    bullet.shape.getPosition().x + 3000 * bullet.dir.x * clock.getElapsedTime().asSeconds(),
                    bullet.shape.getPosition().y + 3000 * bullet.dir.y * clock.getElapsedTime().asSeconds());
            if (!bullets.back().checkborders(window)) bullets.pop_back();
        }
        clock.restart();
    }

    static void collisions(std::vector<Asteroid> &asteroids) {
        for (int i = 0; i < bullets.size(); i++)
            for (int j = 0; j < asteroids.size(); j++) {
                if (pow(asteroids[j].shape.getPosition().x - bullets[i].shape.getPosition().x, 2)
                    + pow(asteroids[j].shape.getPosition().y - bullets[i].shape.getPosition().y, 2)
                    <= pow(asteroids[j].radius, 2)) {
                    asteroids.erase(asteroids.begin()+j);
                    bullets.erase(bullets.begin() + i);
                }
            }
    }
};
