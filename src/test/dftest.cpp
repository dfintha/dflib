#include "dftest.hpp"
#include <iomanip>
#include <iostream>

static constexpr const int max_name_length = 64;
static constexpr const int separator_length = 10;
static constexpr const char passed[] = "Passed";
static constexpr const char failed[] = "Failed";
static constexpr const char nl = '\n';

static void print_separator_line() {
    std::cout << std::string(max_name_length + separator_length, '-') << nl;
}

static void print_test_result(const std::string& name, const bool result) {
    std::cout << std::setfill(' ') << name;
    std::cout << "    " << (result ? passed : failed) << nl;
}

static void print_suite_result(const dftest::statistics& stats) {
    std::cout << "Passed: " << stats.passed << nl;
    std::cout << "Failed: " << stats.failed << nl;
    std::cout << " Total: " << stats.total << nl;
}

namespace dftest {
    statistics::statistics() :
        total(0),
        passed(0),
        failed(0),
        success(false)
    { }

    void suite::add(std::string name, const std::function<bool()>& function) {
        name.resize(max_name_length, ' ');
        tests.push_back(test{std::move(name), function});
        ++stats.total;
    }

    bool suite::run() {
        stats.passed = 0;
        stats.failed = 0;
        print_separator_line();
        for (auto& t : tests) {
            const bool result = t.function();
            (result) ? (++stats.passed) : (++stats.failed);
            print_test_result(t.name, result);
        }
        stats.success = (stats.failed == 0 && stats.passed == stats.total);
        print_separator_line();
        print_suite_result(stats);
        return stats.success;
    }
}
