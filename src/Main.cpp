#include <SFML/Graphics.hpp>
#include "Misc.hpp"
#include "BezierCurve.hpp"

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
