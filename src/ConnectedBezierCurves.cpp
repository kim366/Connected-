#include "ConnectedBezierCurves.hpp"
#include "Misc.hpp"

void ConnectedBezierCurves::addPoint(sf::Vector2f mpos)
{
	if (_curves.empty())
	{
	    _curves.emplace_back();
	    _curves.back()[0] = mpos;
	}
	else
	{
	    
	    if (_curves.back()[1] == sf::Vector2f{-1, -1} && _curves.size() > 1)
	    {
	        auto prev_curve{_curves[_curves.size() - 2]};
	        _curves.back().setNextPoint(put_point_on_line(mpos, prev_curve[2], prev_curve[3]));
	    }
	    else
	        _curves.back().setNextPoint(mpos);

	    if (_curves.back().fullyInitialized())
	    {
	        auto prev_curve{_curves.back()};
	        _curves.emplace_back();
	        _curves.back()[0] = prev_curve[3];
	    }
	}
}
