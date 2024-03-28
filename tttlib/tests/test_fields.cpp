#include "tests_common.h"
#include "basic_fields.h"

#include <unordered_set>


struct TestFixedSizeField {
    std::string name = "fixed_size_field";

    bool is_empty(const FixedSizeField& field) {
        Boundary b = field.get_current_boundary();
        for (int x = b.min.x; x <= b.max.x; ++x) {
            for (int y = b.min.y; y <= b.max.y; ++y) {
                Point p = {.x = x, .y = y};
                if (field.get_value(p) != Mark::None) {
                    return false;
                }
            }
        }
        return true;
    }

    int operator()() {
        Boundary b = {
            .min = {.x = 0, .y = 0},
            .max = {.x = 5, .y = 5},
        };
        FixedSizeField field(b);
        ASSERT(field.get_current_boundary() == b);
        ASSERT(is_empty(field));
        Point p = {.x = 1, .y =1};
        field.set_value(p, Mark::Zero); 
        ASSERT(field.get_value(p) == Mark::Zero);
        ASSERT(!is_empty(field));
        p = {.x = 2, .y = 3};
        field.set_value(p, Mark::Cross); 
        ASSERT(field.get_value(p) == Mark::Cross);
        field.set_value(p, Mark::None);
        ASSERT(field.get_value(p) == Mark::None);
        ASSERT(!is_empty(field));
        field.clear();
        ASSERT(is_empty(field));
        return 0;
    }
};

struct TestFixedFieldIterator {
    std::string name = "fixed_size_iterator";
    int operator()() {
        Boundary b = {
            .min = {.x = 0, .y = 0},
            .max = {.x = 5, .y = 5},
        };
        FixedSizeField field(b);
        {
            auto it = field.get_iterator();
            ASSERT(!it->has_value());
        }
        field.set_value({.x = 1, .y = 2}, Mark::Zero);
        {
            auto it = field.get_iterator();
            ASSERT(it->has_value());
            ASSERT(it->get_point() == Point({.x = 1, .y = 2}));
            ASSERT(it->get_value() == Mark::Zero);
            it->step();
            ASSERT(!it->has_value());
        }
        field.set_value({.x = 2, .y = 3}, Mark::Cross);
        field.set_value({.x = 5, .y = 1}, Mark::Cross);
        size_t count = 0;
        for (auto it = field.get_iterator(); it->has_value(); it->step()) {
            Point p = it->get_point();
            ASSERT(field.get_value(p) == it->get_value());
            ++count;
        }
        ASSERT(count == 3);
        return 0;
    }
};

struct TestInfiniteField {
    std::string name = "infinite_field";
    int operator()() {
        InfiniteField field;
        Boundary b = {.min = {.x = 0, .y = 0}, .max = {.x = 0, .y = 0}};
        ASSERT(field.get_current_boundary() == b);
        Point p = {.x = 1, .y = 2};
        ASSERT(field.get_value(p) == Mark::None);
        field.set_value(p, Mark::Cross);
        ASSERT(field.get_current_boundary() == Boundary({.min = p, .max = p}));
        ASSERT(field.get_value(p) == Mark::Cross);
        field.set_value(p, Mark::None);
        ASSERT(field.get_value(p) == Mark::None);
        ASSERT(field.get_current_boundary() == b);
        field.set_value(p, Mark::Zero);
        ASSERT(field.get_value(p) == Mark::Zero);
        field.clear();
        ASSERT(field.get_current_boundary() == b);
        ASSERT(field.get_value(p) == Mark::None);
        return 0;
    }
};

struct TestInfiniteFieldIterator {
    std::string name = "infinite_field_iterator";
    
    bool iterator_is_empty(const InfiniteField& field) {
        return !field.get_iterator()->has_value();
    }

    int operator()() {
        InfiniteField field;
        ASSERT(iterator_is_empty(field));
        Point points[10];
        for (int i = 0; i < sizeof(points) / sizeof(points[0]); ++i) {
            points[i] = {.x = i, .y = 100 - i};
            field.set_value(points[i], Mark::Cross);
        }
        std::unordered_set<Point> result_set;
        for (auto it = field.get_iterator(); it->has_value(); it->step()) {
            result_set.insert(it->get_point());
            ASSERT(it->get_value() == Mark::Cross);
        }
        ASSERT(result_set.size() == 10);
        ASSERT(result_set == std::unordered_set<Point>(
                    std::begin(points), std::end(points)));
        field.clear();
        ASSERT(iterator_is_empty(field));
        return 0;
    }
};

TESTS(TestFixedSizeField, TestFixedFieldIterator, TestInfiniteField, TestInfiniteFieldIterator)
