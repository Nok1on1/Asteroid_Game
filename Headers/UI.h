#pragma once
#include <SFML/Graphics.hpp>

namespace UITextures {
    inline sf::Texture starsT;
}
class UI {
public:
    static sf::RectangleShape stars;
};
sf::RectangleShape UI::stars;