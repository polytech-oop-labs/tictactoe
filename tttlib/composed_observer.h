#pragma once
#include <list>
#include "game_state.h"

/**
 * Simple observer that broadcasts every event to all added observers.
 */
class ComposedObserver: public Observer {
    std::list<Observer*> _observers;
public:
    void notify(const GameView& game, const Event& event) override;
    void add_observer(Observer& observer);
    void remove_observer(Observer& observer);
};
