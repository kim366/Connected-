#pragma once

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "BezierCurve.hpp"

class ConnectedBezierCurves
{
public:
	void addPoint(sf::Vector2f mpos);

	auto begin() const { return _curves.cbegin(); }
	auto end() const { return _curves.cend(); }

private: 
    std::vector<BezierCurve> _curves;
};