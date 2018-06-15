#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Misc.hpp"

TEST_CASE("Point is put on line") {
    CHECK(put_point_on_line({-1, -3}, {2, 1}, {0, 0}).x == sf::Vector2f{-2, -1}.x);
    CHECK(put_point_on_line({-1, -3}, {2, 1}, {0, 0}).y == sf::Vector2f{-2, -1}.y);

    CHECK(put_point_on_line({3, -4}, {2, 4}, {0, 0}).x == sf::Vector2f{-1, -2}.x);
    CHECK(put_point_on_line({3, -4}, {2, 4}, {0, 0}).y == sf::Vector2f{-1, -2}.y);

    CHECK(put_point_on_line({9, 4}, {2, 3}, {4, 1}).x == sf::Vector2f{5, 0}.x);
    CHECK(put_point_on_line({9, 4}, {2, 3}, {4, 1}).y == sf::Vector2f{5, 0}.y);
}
