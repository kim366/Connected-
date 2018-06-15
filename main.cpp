#include <SFML/Graphics.hpp>
#include <cassert>
#include <cmath>
#include <array>
#include <algorithm>
// #define CATCH_CONFIG_MAIN
#include "catch.hpp"

void draw_lines(const std::vector<sf::Vector2f>& vector_, sf::RenderTarget& target_, sf::Color color_ = sf::Color::White)
{
    for (auto it{vector_.cbegin()}; it != vector_.cend();)
    {
        auto current_it{it};
        if (++it != vector_.cend())
        {
            sf::Vertex line[2]
            {
                {*current_it, color_},
                {*it, color_}
            };

            target_.draw(line, 2, sf::Lines);
        }
    }
}

float dot(sf::Vector2f first_, sf::Vector2f second_)
{
    return first_.x * second_.x + first_.y * second_.y;
}

float mag(sf::Vector2f vec_)
{
    return std::hypot(vec_.x, vec_.y);
}

sf::Vector2f unitV(sf::Vector2f vec_)
{
    return vec_ / mag(vec_);
}

namespace sf
{
    std::ostream& operator<<(std::ostream& os_, const sf::Vector2f& vec_)
    {
        os_ << vec_.x << ", " << vec_.y << '\n';
        return os_;
    }
}

sf::Vector2f put_point_on_line(sf::Vector2f point, sf::Vector2f line_point0, sf::Vector2f line_point1)
{
    const sf::Vector2f line_seg{line_point1 - line_point0};
    return line_point1 + unitV(line_seg) * dot(unitV(line_seg), point - line_point1);
}

float interpolate(float t, float a, float b)
{
    return (b - a) * t + a;
}

sf::Vector2f vinterpolate(float t, sf::Vector2f a, sf::Vector2f b)
{
    return {interpolate(t, a.x, b.x), interpolate(t, a.y, b.y)};
}

class BezierCurve : public sf::Drawable
{
public:
    BezierCurve()
    {
        std::fill(_points.begin(), _points.end(), sf::Vector2f{-1, -1});
    }

    sf::Vector2f getA()
    {
        return _points[2] + (_points[2] - _points[1]);
    }

    void setA(sf::Vector2f a_)
    {
        _points[2] = (a_ + _points[1]) / 2.f;
    }

    sf::Vector2f& operator[](size_t i_)
    {
        return _points[i_];
    }

    void makeCurve()
    {
        // all points filled -> draw bezier curve
        if (fullyInitialized())
        {
            for (float n{50}, t{0}; t <= 1.01; t += 1 / n)
            {
                std::vector<sf::Vector2f> remaining_points;
                remaining_points.resize(4);
                std::copy(_points.begin(), _points.end(), remaining_points.begin());

                while (remaining_points.size() > 1)
                {
                    std::vector<sf::Vector2f> current_remaining_points;
                    for (auto it{remaining_points.cbegin()}; it != remaining_points.cend();)
                    {
                        auto current_it{it};
                        if (++it != remaining_points.cend())
                            current_remaining_points.push_back(vinterpolate(t, *current_it, *it));
                    }
                    remaining_points = current_remaining_points;
                }

                if (remaining_points.size() == 1)
                    _curve.push_back(remaining_points[0]);
            }
        }
    }

    void setNextPoint(sf::Vector2f point_)
    {
        auto found{std::find(_points.begin(), _points.end(), sf::Vector2f{-1, -1})};
        assert(found != _points.end());
        *found = point_;

        if (fullyInitialized())
            makeCurve();
    }

    bool fullyInitialized() const
    {
        std::cout << "initialized: " << (std::find(_points.begin(), _points.end(), sf::Vector2f{-1, -1}) == _points.end()) << '\n';
        return std::find(_points.begin(), _points.end(), sf::Vector2f{-1, -1}) == _points.end();
    }

private:
    void draw(sf::RenderTarget& target_, sf::RenderStates states_) const override
    {
        for (auto& point : {_points[0], _points[3]})
        {
            sf::CircleShape circle{2};
            circle.setOrigin(2, 2);
            circle.setPosition(point);
            circle.setFillColor({100, 100, 100});
            target_.draw(circle);
        }

        if (fullyInitialized())
            draw_lines(_curve, target_, sf::Color::Red);
    }

private:
    std::array<sf::Vector2f, 4> _points;
    std::vector<sf::Vector2f> _curve;
};

int main()
{
    const sf::ContextSettings aa_8x{0, 0, 8};
    sf::RenderWindow window({350, 350}, "SFML Bezier", sf::Style::Default, aa_8x);

    std::vector<BezierCurve> curves;

    while (true)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                return 0;

            if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mpos{static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)};

                if (curves.empty())
                {
                    curves.emplace_back();
                    curves.back()[0] = mpos;
                }
                else
                {
                    
                    if (curves.back()[1] == sf::Vector2f{-1, -1} && curves.size() > 1)
                    {
                        auto prev_curve{curves[curves.size() - 2]};
                        curves.back().setNextPoint(put_point_on_line(mpos, prev_curve[2], prev_curve[3]));
                    }
                    else
                        curves.back().setNextPoint(mpos);

                    if (curves.back().fullyInitialized())
                    {
                        auto prev_curve{curves.back()};
                        curves.emplace_back();
                        curves.back()[0] = prev_curve[3];
                    }
                }
            }
        }

        window.clear();
        for (const auto& curve : curves)
            window.draw(curve);
        window.display();
    }   
}

// TEST_CASE("Point is put on line") {
//     CHECK(put_point_on_line({-1, -3}, {2, 1}, {0, 0}).x == sf::Vector2f{-2, -1}.x);
//     CHECK(put_point_on_line({-1, -3}, {2, 1}, {0, 0}).y == sf::Vector2f{-2, -1}.y);

//     CHECK(put_point_on_line({3, -4}, {2, 4}, {0, 0}).x == sf::Vector2f{-1, -2}.x);
//     CHECK(put_point_on_line({3, -4}, {2, 4}, {0, 0}).y == sf::Vector2f{-1, -2}.y);

//     CHECK(put_point_on_line({9, 4}, {2, 3}, {4, 1}).x == sf::Vector2f{5, 0}.x);
//     CHECK(put_point_on_line({9, 4}, {2, 3}, {4, 1}).y == sf::Vector2f{5, 0}.y);
// }
