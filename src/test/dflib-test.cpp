#include "test/dftest.hpp"

#include "lib/approx.hpp"
#include "lib/circulator.hpp"
#include "lib/comparable.hpp"
#include "lib/console.hpp"
#include "lib/delegate.hpp"
#include "lib/dependant.hpp"
#include "lib/documentation.hpp"
#include "lib/flagset.hpp"
#include "lib/metalist.hpp"
#include "lib/number.hpp"
#include "lib/resource.hpp"
#include "lib/series.hpp"
#include "lib/strings.hpp"
#include "lib/types.hpp"
#include "lib/utilities.hpp"

#include <cstdlib>
#include <ctime>

static bool test_series_series() {
    const size_t fi_rand = std::rand() % 80 + 1;
    const size_t fi_base = df::fibonacci(fi_rand);
    const size_t fi_add1 = df::fibonacci(fi_rand + 1);
    const size_t fi_add2 = df::fibonacci(fi_rand + 2);
    dftest_assert_equals(fi_base + fi_add1, fi_add2);
    const size_t fa_rand = std::rand() % 20 + 1;
    const size_t fa_base = df::factorial(fa_rand);
    const size_t fa_add1 = df::factorial(fa_rand + 1);
    dftest_assert_equals(fa_add1, (fa_rand + 1) * fa_base);
    return true;
}

static bool test_utility_minmax() {
    std::vector<int> tester;
    for (int i = 0; i < 20; ++i)
        tester.push_back(std::rand() % 100);
    const int max1 = *df::max(tester.begin(), tester.end());
    const int min1 = *df::min(tester.begin(), tester.end());
    std::sort(tester.begin(), tester.end());
    const int max2 = *df::max(tester.begin(), tester.end());
    const int min2 = *df::min(tester.begin(), tester.end());
    dftest_assert_equals(max1, max2);
    dftest_assert_equals(min1, min2);
    dftest_assert_equals(max1, tester[tester.size() - 1]);
    dftest_assert_equals(min1, tester[0]);
    return true;
}

static bool test_utility_bytes() {
    unsigned char c = std::rand() % 256;
    const auto bytes = df::bytes_of(c);
    dftest_assert_equals((unsigned char)(bytes[0]), c);
    df::clear(c);
    dftest_assert_equals(c, 0);
    return true;
}

static bool test_flagset_flagset() {
    df::flagset16 flags (df::full_mask<16>());
    dftest_assert_true(flags.test<2>());
    dftest_assert_true(flags.test<6>());
    dftest_assert_true(flags.test<12>());
    dftest_assert_true(flags.test<16>());

    flags.clear<12>();
    dftest_assert_false(flags.test<12>());

    flags.set<12>();
    dftest_assert_true(flags.test<12>());

    flags.reset();
    flags.set<11>();
    dftest_assert_true(flags.test<11>());
    dftest_assert_false(flags.test<12>());

    static constexpr const unsigned bits_per_byte = 8;
    dftest_assert_equals(16UL, sizeof(df::flagset16) * bits_per_byte);
    dftest_assert_equals(16UL, df::flagset16::length());

    flags = 0x0001;
    dftest_assert_true(flags.test<1>());
    dftest_assert_false(flags.test<2>());
    return true;
}

int main() {
    std::srand(unsigned(std::time(nullptr)));
    std::cout << "Report for Test Suite (dflib)" << df::nl;
    dftest::suite test_suite;
    test_suite.add("df::fibonacci, df::factorial", test_series_series);
    test_suite.add("df::min, df::max", test_utility_minmax);
    test_suite.add("df::bytes_of, df::clear", test_utility_bytes);
    test_suite.add("df::flagset", test_flagset_flagset);
    return test_suite.run() ? EXIT_SUCCESS : EXIT_FAILURE;
}
