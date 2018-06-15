#pragma once

#include <vector>
#include <array>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>

class BezierCurve : public sf::Drawable
{
public:
    BezierCurve();

    sf::Vector2f& operator[](size_t i_) { return _points[i_]; }
    const sf::Vector2f& operator[](size_t i_) const { return _points[i_]; }

    void makeCurve();
    void setNextPoint(sf::Vector2f point_);

    bool fullyInitialized() const;

private:
    void draw(sf::RenderTarget& target_, sf::RenderStates states_) const override;

private:
    std::array<sf::Vector2f, 4> _points;
    std::vector<sf::Vector2f> _curve;
};
