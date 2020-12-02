/// @file test_tools.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date November 23, 2020
#include "doctest.h"
#include "pixie16sys_common.h"

TEST_SUITE("sys/tools.c") {
    TEST_CASE("get_ns_per_cycle") { CHECK(false); }
    TEST_CASE("Pixie_Print_MSG") {}
    TEST_CASE("SYS16_ClrBit") {
        CHECK(10 == SYS16_ClrBit(10, 10));
        CHECK(8 == SYS16_ClrBit(1, 10));
        CHECK(8 == SYS16_ClrBit(64, 8));
        CHECK(8 == SYS16_ClrBit(32, 8));
        CHECK(8 == SYS16_ClrBit(-4, 8));
        CHECK(0 == SYS16_ClrBit(25, 134217728));
    }
    TEST_CASE("SYS16_SetBit") {
        CHECK(8 == SYS16_SetBit(3, 8));
        CHECK(10 == SYS16_SetBit(1, 8));
        CHECK(8 == SYS16_SetBit(400, 8));
        CHECK(8 == SYS16_SetBit(-4, 8));
        CHECK(0 == SYS16_SetBit(25, 134217728));
    }
    TEST_CASE("SYS16_TstBit") {
        CHECK(SYS16_TstBit(3, 8));
        CHECK_FALSE(SYS16_TstBit(10, 8));
        CHECK_FALSE(SYS16_TstBit(17, 65535));
    }
    TEST_CASE("SYS32_ClrBit") {
        CHECK(8 == SYS32_ClrBit(1, 8));
        CHECK(8 == SYS32_ClrBit(400, 8));
        CHECK(8 == SYS32_ClrBit(-4, 8));
        CHECK(134217728 == SYS32_ClrBit(25, 167772160));
        CHECK(0 == SYS32_ClrBit(33, 8589934592));
    }
    TEST_CASE("SYS32_SetBit") {
        CHECK(524290 == SYS32_SetBit(1, 524288));
        CHECK(8 == SYS32_SetBit(400, 8));
        CHECK(8 == SYS32_SetBit(-4, 8));
        CHECK(167772160 == SYS32_SetBit(25, 134217728));
        CHECK(0 == SYS32_SetBit(33, 8589934592));
    }
    TEST_CASE("SYS32_TstBit") {
        CHECK(SYS32_TstBit(19, 524288));
        CHECK_FALSE(SYS32_TstBit(10, 524288));
        CHECK_FALSE(SYS32_TstBit(33, 4294967295));
    }
    TEST_CASE("wait_for_a_short_time") { CHECK(false); }
}
