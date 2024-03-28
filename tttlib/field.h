/** This file defines interfaces for different field implementations. */

#pragma once
#include <memory>

#include "types.h"

/**
 * Java-style readonly forward iterator for iteration through X's and O's on the
 * field.
 *
 * Example usage:
 *
 * ```c++
 * void draw(const Field& field) {
 *  char field_chars[10][10];
 *
 *  // fill `field_chars` with spaces
 *
 *  for (auto it = field.get_iterator(); it->has_value(); it->step()) {
 *    Point p = it->get_point();
 *    field_chars[p.y][p.x] = it->get_value() == Mark::Cross ? 'X' : 'O';
 *  }
 *
 *  // print `field_chars`
 *
 * }
 * ```
 */
class FieldIterator {
public:
    virtual ~FieldIterator() {}
    /**
     * Returns whether iterator has any value or it has reached end of all
     * available non-empty cells
     */
    virtual bool has_value() const = 0;
    /**
     * Returns coordinates of current cell.
     *
     * If called when `has_value() == false`, behaviour is undefined.
     */
    virtual Point get_point() const = 0;
    /**
     * Returns the contents of current cell.
     *
     * If called when `has_value() == false`, behaviour is undefined.
     */
    virtual Mark get_value() const = 0;

    /**
     * Get to the next value.
     *
     * If called when `has_value() == false`, behaviour is undefined.
     */
    virtual void step() = 0;

    /** Makes a dynamic copy of `this`. */
    virtual std::unique_ptr<FieldIterator> copy() = 0;
};


/** Interface for different field implementations. */
class Field {
public:
    virtual ~Field() {}
    /**
     * Returns `Boundary` object, that is guaranteed to contain all the
     * non-empty cells
     */
    virtual Boundary get_current_boundary() const = 0;
    virtual Mark get_value(const Point& p) const = 0;
    virtual std::unique_ptr<FieldIterator> get_iterator() const = 0;

    virtual void set_value(const Point& p, Mark value) = 0;
    /** Sets all cells to `Mark::None` */
    virtual void clear() = 0;
};
