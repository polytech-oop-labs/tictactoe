/**
 * This file defines some field implementations.
 */
#pragma once
#include <vector>
#include <unordered_map>

#include "field.h"

/**
 * Field implementation for field of fixed size with preallocated array.
 *
 * It takes ~ 2 bits to store single cell.
 */
class FixedSizeField : public Field {
    class _Iterator : public FieldIterator {
        size_t _i;
        const FixedSizeField *_field;

        size_t _get_max_i() const;
        void _find_next();
    public:
        _Iterator(const FixedSizeField *field, size_t i = 0);
        Point get_point() const override;
        Mark get_value() const override;
        bool has_value() const override;

        void step() override;
        std::unique_ptr<FieldIterator> copy() override;
    };
    
    Boundary _size;
    std::vector<bool> _filled;
    std::vector<bool> _crosses;

    size_t _to_index(const Point& p) const;
    Point _to_point(size_t i) const;
    Mark _get_value(size_t i) const;
public:
    FixedSizeField(const Boundary& size);

    Mark get_value(const Point& p) const override;
    Boundary get_current_boundary() const override;
    std::unique_ptr<FieldIterator> get_iterator() const override;

    void set_value(const Point& p, Mark value) override;
    void clear() override;
};


/**
 * Hash function definition for `Point`. 
 *
 * This is needed for using `Point` as a key in hash map.
 */
template<>
struct std::hash<Point>
{
    std::size_t operator()(const Point& point) const {
        return point.x ^ (point.y << 1);
    }
};


/**
 * Hash map implementation of field. Stores only non-empty cells.
 */
class InfiniteField : public Field {
    using PointMap = std::unordered_map<Point, Mark>;
    const Boundary EMPTY_BOUNDARY = {.min = {.x = 0, .y = 0}, .max = {.x = 0, .y = 0}};

    class _Iterator : public FieldIterator {
        PointMap::const_iterator _it;
        PointMap::const_iterator _end;
    public:
        _Iterator(PointMap::const_iterator it, PointMap::const_iterator end);
        Point get_point() const override;
        Mark get_value() const override;
        bool has_value() const override;

        void step() override;
        std::unique_ptr<FieldIterator> copy() override;
    };

    PointMap _points;
    Boundary _current_boundary = EMPTY_BOUNDARY;
public:
    Mark get_value(const Point& p) const override;
    Boundary get_current_boundary() const override;
    std::unique_ptr<FieldIterator> get_iterator() const override;

    void set_value(const Point& p, Mark value) override;
    void clear() override;
};
