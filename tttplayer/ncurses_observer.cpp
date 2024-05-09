#include "ncurses_observer.h"

FieldWindow::FieldWindow(WINDOW *window, const Boundary &bounds)
    : _win(window), _current_view(bounds) {}

void FieldWindow::draw_one(const GameView &view, const Point &p, bool move) {
  char c;
  if (view.get_settings().field_size.is_within(p)) {
    switch (view.get_state().field->get_value(p)) {
    case Mark::Zero:
      c = '0';
      break;
    case Mark::Cross:
      c = 'X';
      break;
    default:
      c = '.';
      break;
    }
  } else {
    c = '#';
  }
  mvwprintw(_win, p.y - _current_view.min.y + 1, p.x - _current_view.min.x + 1,
            "%c", c);
  if (!move) {
    wmove(_win, -1, 0);
  }
}

void FieldWindow::draw_field(const GameView &view) {
  for (Point p = _current_view.min; p.y <= _current_view.max.y; ++p.y) {
    for (p.x = _current_view.min.x; p.x <= _current_view.max.x; ++p.x) {
      draw_one(view, p);
    }
  }
}

void FieldWindow::highlight(const GameView &view, const Point &point,
                            bool hightlighted) {
  if (!_current_view.is_within(point) ||
      !view.get_settings().field_size.is_within(point))
    return;
  if (hightlighted) {
    attron(A_STANDOUT);
  }
  draw_one(view, point);
  if (hightlighted) {
    attroff(A_STANDOUT);
  }
}

void FieldWindow::update() { wrefresh(_win); }

NCursesObserver::NCursesObserver(FieldWindow &window)
    : _window(window), _last_highlighted() {}

void NCursesObserver::notify(const GameView &view, const Event &event) {
  _window.draw_field(view);
  if (event.get_type() == MoveEvent::TYPE) {
    auto &data = get_data<MoveEvent>(event);
    _window.draw_one(view, data.point, false);
  }
  _window.update();
}
