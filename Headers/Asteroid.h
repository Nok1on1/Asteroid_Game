#pragma once
#include "extendedmath.h"
#include <cmath>
#include <future>
#include <iostream>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>

namespace asteroidTextures {
    static inline sf::Texture lutidze;
}


class Asteroid {
public:
    sf::ConvexShape shape;
    sf::Vector2f dir;
    float radius;
    static inline std::vector<Asteroid> asteroids;

    static void newAsteroid(const sf::Vector2f center, const int points, const float radius, const bool add) {
        Asteroid x;
        x.shape.setPointCount(points);
        //range of placement between points
        double dDistance = 360.0 / static_cast<double>(x.shape.getPointCount());
        //center
        linearEq centerpoint{0.01, 0, 0, 0};
        centerpoint.B = centerpoint.y - tan(DtoR(centerpoint.K)) * centerpoint.x;
        //generate all points
        for (int i = 0; i < x.shape.getPointCount(); i++) {
            //generate point
            sf::Vector2f z = generatepoint(centerpoint, dDistance, radius);
            x.shape.setPoint(i, z);

            // std::cout << i + 1 << " x: " << z.x << " y: " << z.y << std::endl;
            //degree for next border
            centerpoint.K = rangify(centerpoint.K + dDistance);
            //correct displacement
            centerpoint.B = centerpoint.y - tan(DtoR(centerpoint.K)) * centerpoint.x;
        }
        // uTextures::lutidze.loadFromFile("/home/noklon/Prog-projects/CLionProjects/Asteroids/Headers/lutidze.png");
        // x.shape.setTexture(&uTextures::lutidze);
        x.shape.setOutlineThickness(4);
        x.shape.setOutlineColor(sf::Color(52, 63, 62));
        x.shape.setFillColor(sf::Color(80, 90, 91));
        x.shape.setScale(1, 1);
        x.radius = radius;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(-200, 200);
        //generate y
        std::random_device rd1;
        std::mt19937 gen1(rd1());
        std::uniform_int_distribution<> distrib1(-200, 200);
        x.dir = {static_cast<float>(distrib(gen)), static_cast<float>(distrib1(gen1))};

        x.shape.setPosition(center);
        if (add) asteroids.emplace_back(x);
    }


    static void generateAsteroids(const sf::RenderWindow &window, const int count, float radius) {
        //generate x
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(radius, window.getSize().x - radius);
        //generate y
        std::random_device rd1;
        std::mt19937 gen1(rd1());
        std::uniform_int_distribution<> distrib1(radius, window.getSize().x - radius);
        for (int i = 0; i < count; i++) {
            newAsteroid(sf::Vector2f{static_cast<float>(distrib(gen)), static_cast<float>(distrib1(gen1))},
                        12, radius, true);
        }
    }

    static void updateCordinates(sf::RenderWindow &window, sf::Clock &clock) {
        for (auto &asteroid: asteroids) {
            screenBorders(window, asteroid);
            sf::Vector2f V1 = asteroid.shape.getPosition();
            asteroid.shape.setPosition(asteroid.dir.x * clock.getElapsedTime().asSeconds() + V1.x,
                                       asteroid.dir.y * clock.getElapsedTime().asSeconds() + V1.y);
            asteroid.shape.rotate(0.001);
        }
        clock.restart();
    }

    static void collisions() {
        for (int i = 0; i < asteroids.size(); i++) {
            for (int j = i + 1; j < asteroids.size(); j++) {
                Asteroid &asteroid1 = asteroids.at(i);
                Asteroid &asteroid2 = asteroids.at(j);
                if (pow(asteroid1.shape.getPosition().x - asteroid2.shape.getPosition().x, 2)
                    + pow(asteroid1.shape.getPosition().y - asteroid2.shape.getPosition().y, 2)
                    <= pow(asteroid1.radius + asteroid2.radius, 2)) {
                    sf::Vector2f vec = subV2f(asteroid1.shape.getPosition(), asteroid2.shape.getPosition());
                    sf::Vector2f unitNormal = normalV2f(vec);
                    sf::Vector2f unitTangent = normalV2f(tangV2f(vec));
                    float v1n = dotProduct(unitNormal, asteroid1.dir);
                    float v1t = dotProduct(unitTangent, asteroid1.dir);
                    float v2n = dotProduct(unitNormal, asteroid2.dir);
                    float v2t = dotProduct(unitTangent, asteroid2.dir);
                    float newV1n = (99 / 100) * (v1n * (asteroid1.radius - asteroid2.radius) + 2 * asteroid2.radius *
                                                 v2n)
                                   / (
                                       asteroid1.radius + asteroid2.radius);
                    float newV2n = (v2n * (asteroid2.radius - asteroid1.radius) + 2 * asteroid1.radius * v1n)
                                   / (
                                       asteroid1.radius + asteroid2.radius);
                    asteroid1.dir = addV2f(multV2f(newV1n, unitNormal), multV2f(v1t, unitTangent));
                    asteroid2.dir = addV2f(multV2f(newV2n, unitNormal), multV2f(v2t, unitTangent));
                }
            }
        }
    }

private:
    static void screenBorders(sf::RenderWindow &window, Asteroid &asteroid) {
        if (asteroid.shape.getPosition().x < asteroid.radius) {
            asteroid.dir.x = -asteroid.dir.x;
            asteroid.shape.setPosition(asteroid.radius, asteroid.shape.getPosition().y);

            //   std::cout << "x0" << std::endl;
        } else if (asteroid.shape.getPosition().x > window.getSize().x - asteroid.radius) {
            asteroid.dir.x = -asteroid.dir.x;
            asteroid.shape.setPosition(window.getSize().x - asteroid.radius, asteroid.shape.getPosition().y);
        }

        if (asteroid.shape.getPosition().y < asteroid.radius) {
            asteroid.dir.y = -asteroid.dir.y;
            asteroid.shape.setPosition(asteroid.shape.getPosition().x, asteroid.radius);
        } else if (asteroid.shape.getPosition().y > window.getSize().y - asteroid.radius) {
            asteroid.dir.y = -asteroid.dir.y;
            asteroid.shape.setPosition(asteroid.shape.getPosition().x, window.getSize().y - asteroid.radius);
        }
    }


    static sf::Vector2f generatepoint(linearEq Point, const double dDistance, int radius) {
        //left and right border
        std::pair borders = generateBorders(Point, dDistance);
        //initialize finalpoint
        sf::Vector2f generatedPoint;
        //border to choose cordinates for generatedpoint
        linearEq lowerBorder{};
        //normilize degree to make it perpendicular to centralpoint
        lowerBorder.K = rangifylb(atan(-1 / tan(DtoR(Point.K))) * (180 / PI_F));
        //find displacement
        if (Point.K > 0 && Point.K < 90) {
            double y_change = sin(DtoR(Point.K)) * radius;
            double x_change = cos(DtoR(Point.K)) * radius;
            lowerBorder.B = -tan(DtoR(lowerBorder.K)) * (Point.x - x_change) + (Point.y - y_change);
        } else if (Point.K > 90 && Point.K < 180) {
            double y_change = sin(DtoR(180 - Point.K)) * radius;
            double x_change = cos(DtoR(180 - Point.K)) * radius;
            lowerBorder.B = -tan(DtoR(lowerBorder.K)) * (Point.x + x_change) + (Point.y - y_change);
        } else if (Point.K > 180 && Point.K < 270) {
            double y_change = sin(DtoR(Point.K - 180)) * radius;
            double x_change = cos(DtoR(Point.K - 180)) * radius;
            lowerBorder.B = -tan(DtoR(lowerBorder.K)) * (Point.x + x_change) + (Point.y + y_change);
        } else if (Point.K > 270 && Point.K < 360) {
            double y_change = sin(DtoR(360 - Point.K)) * radius;
            double x_change = cos(DtoR(Point.K)) * radius;
            lowerBorder.B = -tan(DtoR(lowerBorder.K)) * (Point.x - x_change) + (Point.y + y_change);
        }

        lowerBorder.x = Point.x;
        lowerBorder.y = Point.y;

        double x_min = (lowerBorder.B - borders.first.B) / (tan(DtoR(borders.first.K)) - tan(DtoR(lowerBorder.K)));
        double x_max = (lowerBorder.B - borders.second.B) / (tan(DtoR(borders.second.K)) - tan(DtoR(lowerBorder.K)));
        if (x_min > x_max) std::swap(x_min, x_max);
        if (x_max - x_min > 2) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distrib(x_min, x_max);
            generatedPoint.x = distrib(gen);
        } else generatedPoint.x = x_max;
        generatedPoint.y = tan(DtoR(lowerBorder.K)) * generatedPoint.x + lowerBorder.B;
        // std::cout << "|Borders|" << " K: " << lowerBorder.K << " B: " << lowerBorder.B << std::endl;
        // std::cout << "|Borders|" << " K: " << borders.first.K << " B: " << borders.first.B << std::endl;
        // std::cout << "|Borders|" << " K: " << borders.second.K << " B: " << borders.second.B << std::endl;
        return generatedPoint;
    }

    static std::pair<linearEq, linearEq> generateBorders(linearEq point, double dDistance) {
        linearEq first;
        linearEq second;
        first.K = rangify(point.K - dDistance / 2);
        second.K = rangify(point.K + dDistance / 2);
        first.x = second.x = point.x;
        first.y = second.y = point.y;
        first.B = -tan(DtoR(first.K)) * point.x + point.y;
        second.B = -tan(DtoR(second.K)) * point.x + point.y;
        return std::pair{first, second};
    }

    void static printlinearEq(linearEq x) {
        std::cout << " x: " << x.x << "| y: " << x.y << "| K: " << x.K << "| B: " << x.B << std::endl;
    }
};
