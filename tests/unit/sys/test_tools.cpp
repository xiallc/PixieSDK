/// @file test_tools.cpp
/// @brief
/// @author S. V. Paulauskas
/// @date November 23, 2020
#include "doctest.h"
#include "pixie16sys_common.h"
#include "pixie16sys_export.h"

#include <chrono>
#include <fstream>
#include <random>
#include <string>

TEST_SUITE("sys/tools.c") {
    TEST_CASE("get_ns_per_cycle") {
        //NOTE: This function isn't really testable, since there's no independent way to verify the results. We'll work
        //        under the assumption that we get a result that's greater than 0 and be happy with it.
        auto* result = new double;
        CHECK(0 == get_ns_per_cycle(result));
        CHECK(*result > 0);
        delete result;
    }
    TEST_CASE("Pixie_Print_MSG") {
        std::random_device rd;
        auto random_val = std::fabs(rd());
        char msg[1024];
        sprintf(msg, "Unit test: %f", random_val);
        CHECK(Pixie_Print_MSG(msg) == 0);
        std::ifstream file("Pixie16msg.txt", std::ios::in);
        REQUIRE(file.is_open());
        //From: http://cplusplus.com/forum/general/108679/#msg591060
        std::string line;
        while (file >> std::ws && std::getline(file, line));
        CHECK(line.find(std::to_string(random_val)) != std::string::npos);
        file.close();
    }
    TEST_CASE("SYS16_ClrBit") {
        CHECK(10 == SYS16_ClrBit(10, 10));
        CHECK(8 == SYS16_ClrBit(1, 10));
        CHECK(8 == SYS16_ClrBit(64, 8));
        CHECK(8 == SYS16_ClrBit(32, 8));
        CHECK(8 == SYS16_ClrBit(-4, 8));
    }
    TEST_CASE("SYS16_SetBit") {
        CHECK(8 == SYS16_SetBit(3, 8));
        CHECK(10 == SYS16_SetBit(1, 8));
        CHECK(8 == SYS16_SetBit(400, 8));
        CHECK(8 == SYS16_SetBit(-4, 8));
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
    }
    TEST_CASE("SYS32_SetBit") {
        CHECK(8 == SYS32_SetBit(400, 8));
        CHECK(8 == SYS32_SetBit(-4, 8));
    }
    TEST_CASE("SYS32_TstBit") {
        CHECK(SYS32_TstBit(19, 1 << 19));
    }
    TEST_CASE("wait_for_a_short_time") {
        /// NOTE: This function doesn't have a good way to test. Just as the other time related function above.
        ///          We'll just make sure that what we are both less than the value we passed in in the event we used
        ///          the usleep option, and greater than 0.
        auto start = std::chrono::steady_clock::now();
        wait_for_a_short_time(10);
        double result =
                std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now() - start)
                        .count();
        CHECK(result <= 10.5);
        CHECK(result > 0);
    }
}
