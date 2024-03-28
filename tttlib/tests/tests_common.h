#pragma once
#include <string>


template<typename T>
int __run_test__(const std::string& test_name) {
    T test;
    if (test.name == test_name) return test();
    return 1;
}

template<typename T1, typename T2, typename... Tests>
int __run_test__(const std::string& test_name) {
    T1 test;
    if (test.name == test_name) return test();
    return __run_test__<T2, Tests...>(test_name);
}

#define TESTS(...) int main(int argc, char *argv[]) \
    { return __run_test__<__VA_ARGS__>(std::string(argv[1])); }

#define ASSERT(x) if (!(x)) return 1;
