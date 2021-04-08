/// @file test_utilities.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date November 23, 2020
#include "pixie16app_export.h"

#include "doctest.h"

TEST_SUITE("app/utilities.c") {
    TEST_CASE("Decimal2IEEEFloating"){
        CHECK((unsigned) 0 == Decimal2IEEEFloating(0));
        CHECK((unsigned) 1230546000 == doctest::Approx(Decimal2IEEEFloating(887365)));
        CHECK((unsigned) 3378029648 == doctest::Approx(Decimal2IEEEFloating(-887365)));
        CHECK((unsigned) 1118927127 == doctest::Approx(Decimal2IEEEFloating(88.7365)));
        CHECK((unsigned) 3266410775 == doctest::Approx(Decimal2IEEEFloating(-88.7365)));
    }
    TEST_CASE("IEEEFloating2Decimal"){
        CHECK(doctest::Approx(0) == IEEEFloating2Decimal(0));
        CHECK(doctest::Approx(887365) == IEEEFloating2Decimal(1230546000));
        CHECK(doctest::Approx(-887365) == IEEEFloating2Decimal(3378029648));
        CHECK(doctest::Approx(88.7365) == IEEEFloating2Decimal(1118927127));
        CHECK(doctest::Approx(-88.7365) == IEEEFloating2Decimal(3266410775));
    }
}