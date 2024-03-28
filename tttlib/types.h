#pragma once
#include <cstddef>

/** Type used for indexing in game field */
using field_index_t = int;

/** Enum, representing possible contents of field cell */
enum class Mark {
    Zero,   /// cell contains `O`
    Cross,  /// cell contains `X`
    None,   /// cell does not contain any mark
};


struct Point {
    field_index_t x;
    field_index_t y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

/**
 * Helper struct to be used for representing of boundries of field.
 *
 * Note that it does not make any checks, that coordinates in `min` are actually
 * less that coordinates in `max`.
 */
struct Boundary {
    Point min; /// Point with min `x` and min `y` coordinates
    Point max; /// Point with max `x` and max `y` coordinates

    bool is_within(const Point& p) const {
        return min.x <= p.x && p.x <= max.x 
            && min.y <= p.y && p.y <= max.y;
    }
    
    size_t get_width() const { return max.x - min.x + 1; }
    size_t get_height() const { return max.y - min.y + 1; }

    bool operator==(const Boundary& other) const {
        return min == other.min && max == other.max;
    }
};
