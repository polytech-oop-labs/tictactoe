#pragma once
#include "player.h"
#include <ncurses.h>
#include <functional>

class FieldWindow {
  WINDOW *_win;
  Boundary _current_view;


public:
  FieldWindow(WINDOW *window, const Boundary& bounds);
  void draw_field(const GameView& view);
  void draw_one(const GameView& view, const Point& p, bool move = true);
  void highlight(const GameView& view, const Point& point, bool hightlighted);
  void update();
};

//  

class NCursesObserver : public Observer {
    FieldWindow &_window;
    Point _last_highlighted;
public:
  NCursesObserver(FieldWindow& window);
  void notify(const GameView &view, const Event &) override;
};
