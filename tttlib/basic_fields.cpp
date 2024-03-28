#include "basic_fields.h"
#include "types.h"
#include <memory>

FixedSizeField::FixedSizeField(const Boundary& size):
    _size(size), _filled(), _crosses() 
{
    size_t n_cells = size.get_width() * size.get_height();
    _filled.resize(n_cells, false);
    _crosses.resize(n_cells, false);
}

size_t FixedSizeField::_to_index(const Point& p) const {
    const size_t i =  p.y - _size.min.y, j = p.x - _size.min.x;
    return i * _size.get_width() + j;
}

Point FixedSizeField::_to_point(size_t i) const {
    const size_t width = _size.get_width();
    return {
        .x = static_cast<field_index_t>(_size.min.x + i % width),
        .y = static_cast<field_index_t>(_size.min.y + i / width),
    };
}

Mark FixedSizeField::_get_value(size_t i) const {
    return _filled.at(i) 
       ? ( _crosses.at(i) ? Mark::Cross : Mark::Zero)
       : Mark::None;
}

Mark FixedSizeField::get_value(const Point& p) const {
    return _get_value(_to_index(p));
}

Boundary FixedSizeField::get_current_boundary() const {
    return _size;
}

void FixedSizeField::clear() {
    _filled.assign(_filled.size(), false);
}

void FixedSizeField::set_value(const Point& p, Mark value) {
    const size_t i = _to_index(p);
    switch (value) {
        case Mark::Cross:
            _filled.at(i) = true;
            _crosses.at(i) = true;
            break;
        case Mark::Zero:
            _filled.at(i) = true;
            _crosses.at(i) = false;
            break;
        case Mark::None:
            _filled.at(i) = false;
            break;
    }
}

FixedSizeField::_Iterator::_Iterator(const FixedSizeField *field, size_t i):
    _field(field), _i(i)
{
    if (!_field->_filled.at(_i)) {
        _find_next();
    }
}

size_t FixedSizeField::_Iterator::_get_max_i() const {
    return _field->_filled.size();
}

void FixedSizeField::_Iterator::_find_next() {
    const size_t max_i = _get_max_i();
    ++_i;
    while (_i != max_i && !_field->_filled.at(_i)) ++_i;
}

bool FixedSizeField::_Iterator::has_value() const {
    return _i < _get_max_i();
}

Point FixedSizeField::_Iterator::get_point() const {
    return _field->_to_point(_i);
}

Mark FixedSizeField::_Iterator::get_value() const {
    return _field->_get_value(_i);
}

void FixedSizeField::_Iterator::step() {
    _find_next();
}

std::unique_ptr<FieldIterator> FixedSizeField::_Iterator::copy() {
    return std::make_unique<_Iterator>(_field, _i);
}

std::unique_ptr<FieldIterator> FixedSizeField::get_iterator() const {
    return std::make_unique<FixedSizeField::_Iterator>(this);
}

Mark InfiniteField::get_value(const Point& p) const {
    auto it = _points.find(p);
    if (it == _points.end()) return Mark::None;
    return it->second;
}

Boundary InfiniteField::get_current_boundary() const {
    return _current_boundary;
}

void InfiniteField::clear() {
    _current_boundary = EMPTY_BOUNDARY;
    _points.clear();
}

void InfiniteField::set_value(const Point& p, Mark value) {
    auto it = _points.find(p);
    if (value == Mark::None) {
        if (it != _points.end()) {
            _points.erase(it);
            if (_points.empty()) {
                _current_boundary = EMPTY_BOUNDARY;
            }
        }
        return;
    }
    if (_points.empty()) {
        _current_boundary = {
            .min = {.x = p.x, .y = p.y}, .max = {.x = p.x, .y = p.y}
        };
    }
    else if (it == _points.end()) {
        _current_boundary = {
            .min = {
                .x = std::min(p.x, _current_boundary.min.x),
                .y = std::min(p.y, _current_boundary.min.y),
            },
            .max = {
                .x = std::max(p.x, _current_boundary.max.x),
                .y = std::max(p.y, _current_boundary.max.y),
            },
        };
    }
    _points[p] = value;
}

std::unique_ptr<FieldIterator> InfiniteField::get_iterator() const {
    return std::make_unique<InfiniteField::_Iterator>(_points.begin(), _points.end());
}

InfiniteField::_Iterator::_Iterator(PointMap::const_iterator it, PointMap::const_iterator end):
    _it(it), _end(end) {}

Point InfiniteField::_Iterator::get_point() const {
    return _it->first;   
}

Mark InfiniteField::_Iterator::get_value() const {
    return _it->second;
}

bool InfiniteField::_Iterator::has_value() const {
    return _it != _end;
}

void InfiniteField::_Iterator::step() {
    ++_it;
}

std::unique_ptr<FieldIterator> InfiniteField::_Iterator::copy() {
    return std::make_unique<InfiniteField::_Iterator>(_it, _end);
}
