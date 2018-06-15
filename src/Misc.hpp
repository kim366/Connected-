#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <ostream>
#include <vector>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sf
{
    std::ostream& operator<<(std::ostream& os_, const sf::Vector2f& vec_);
}

void draw_lines(const std::vector<sf::Vector2f>& points_, sf::RenderTarget& target_, sf::Color color_ = sf::Color::White);
float dot(sf::Vector2f first_, sf::Vector2f second_);
float mag(sf::Vector2f vec_);
sf::Vector2f unitV(sf::Vector2f vec_);
sf::Vector2f put_point_on_line(sf::Vector2f point, sf::Vector2f line_point0, sf::Vector2f line_point1);
float interpolate(float t, float a, float b);
sf::Vector2f vinterpolate(float t, sf::Vector2f a, sf::Vector2f b);
