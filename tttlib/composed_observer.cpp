#include "composed_observer.h"

void ComposedObserver::notify(const GameView& game, const Event& event) {
    for (auto& obs: _observers) {
        obs->notify(game, event);
    }
}

void ComposedObserver::add_observer(Observer& observer) {
    _observers.push_back(&observer);
}

void ComposedObserver::remove_observer(Observer& observer) {
    _observers.remove(&observer);
}
