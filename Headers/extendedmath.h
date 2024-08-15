#pragma once
#include <cmath>
#include <bits/random.h>

const float PI_F = 3.14159265358979f;

struct linearEq {
    double K;
    double B;
    float x;
    float y;
};

static sf::Vector2f normalV2f(sf::Vector2f vector) {
    const auto mag = static_cast<float>(sqrt(pow(vector.x, 2) + pow(vector.y, 2)));
    return {vector.x / mag, vector.y / mag};
}

static sf::Vector2f tangV2f(sf::Vector2f vector) {
    return {-vector.y, vector.x};
}

static sf::Vector2f multV2f(float k, sf::Vector2f vector) {
    return {vector.x*k,vector.y*k};
}

static float dotProduct(sf::Vector2f vector1, sf::Vector2f vector2) {
    return vector1.x * vector2.x + vector1.y * vector2.y;
};
static sf::Vector2f addV2f(sf::Vector2f vector1, sf::Vector2f vector2) {
    return {vector1.x + vector2.x, vector1.y + vector2.y};
}
static sf::Vector2f subV2f(sf::Vector2f vector1, sf::Vector2f vector2) {
    return {vector1.x - vector2.x, vector1.y - vector2.y};
}

static double rangify(double x) {
    while (x > 360) {
        x -= 360;
    }
    while (x < 0) {
        x += 360;
    }
    return x;
}

static double rangifylb(double x) {
    while (x > 180) {
        x -= 180;
    }
    while (x < 0) {
        x += 180;
    }
    return x;
}

static double cot(double angle) {
    return 1.0 / std::tan(angle);
}

static double DtoR(const double degrees) {
    return rangify(degrees) * (PI_F / 180);
}

static double acot(double x) {
    return std::atan(DtoR(x)) * (180.0 / M_PI);
}

static double cotToTan(const double degree) {
    double z = atan(cot(DtoR(degree))) * (180 / PI_F);
    return rangify(z);
}
