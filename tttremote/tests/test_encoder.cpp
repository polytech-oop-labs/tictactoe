#include "encoder.h"
#include "game_engine.h"
#include <iostream>
#include "basic_fields.h"
#include "events.h"

int testSettings()
{
    GameSettings settings = {
       .field_size = {
           .min = {.x = -2, .y = -2},
           .max = {.x = 2, .y = 2},
       },
       .max_moves = 0,
       .win_length = 4,
       .move_timelimit_ms = 0,
    };
    return 1;
}

int testEvents()
{
    Event e = MoveEvent::make(Point{ 1, 2 }, Mark::Cross);
    return 1;
}

int testfield() {
    Boundary field_size = {
            .min = {.x = 1, .y = 1},
            .max = {.x = 40, .y = 40},
    };
    Point point;
    FixedSizeField field(field_size);
    point = { .x = 10, .y = 10 };
    field.set_value(point, Mark::Cross);
    MessageEncoder encoder;
    size_t size = encoder.determineSize(field);
    if (size != 21) return 1;

    char* buf = new char[size];
    encoder.encodeGameField(buf, size, field);

    MessageDecoder decoder;
    FixedSizeField field2(field_size);
    decoder.decodeField(buf, size, field2);

    if (field2.get_value(point) == Mark::Cross) return 0;
    else return 1;
}

int main() {
    if (testfield()) return 1;
    if (testSettings()) return 2;
	if (testEvents()) return 3;
	return 0;
}