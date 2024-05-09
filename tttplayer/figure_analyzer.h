#pragma once
#include <vector>

#include "player.h"

class FigureAnalyzer {
public:
    virtual ~FigureAnalyzer() {}
    virtual void start_game(const GameView& view, Mark our );
    virtual std::vector<Point> choose() = 0;
    virtual void set_point(const Point& p, bool is_ours) = 0;
};
