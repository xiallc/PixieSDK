/// @file test_pixie16app.cpp
/// @brief Unittets
/// @author S. V. Paulauskas
/// @date November 23, 2020
#include "doctest.h"
#include "pixie16app_export.h"
#include "pixie16app_globals.h"

#include <complex>
#include <vector>

TEST_SUITE("app/pixie16app.c") {
    TEST_CASE("APP16_ClrBit") {
        CHECK(10 == APP16_ClrBit(10, 10));
        CHECK(8 == APP16_ClrBit(1, 10));
        CHECK(8 == APP16_ClrBit(64, 8));
        CHECK(8 == APP16_ClrBit(32, 8));
        CHECK(8 == APP16_ClrBit(-4, 8));
        CHECK(0 == APP16_ClrBit(25, 134217728));
    }
    TEST_CASE("APP16_SetBit") {
        CHECK(8 == APP16_SetBit(3, 8));
        CHECK(10 == APP16_SetBit(1, 8));
        CHECK(8 == APP16_SetBit(400, 8));
        CHECK(8 == APP16_SetBit(-4, 8));
        CHECK(0 == APP16_SetBit(25, 134217728));
    }
    TEST_CASE("APP16_TstBit") {
        CHECK(APP16_TstBit(3, 8));
        CHECK_FALSE(APP16_TstBit(10, 8));
        CHECK_FALSE(APP16_TstBit(17, 65535));
    }
    TEST_CASE("APP32_ClrBit") {
        CHECK(8 == APP32_ClrBit(1, 8));
        CHECK(8 == APP32_ClrBit(400, 8));
        CHECK(8 == APP32_ClrBit(-4, 8));
        CHECK(134217728 == APP32_ClrBit(25, 167772160));
        CHECK(0 == APP32_ClrBit(33, 8589934592));
    }
    TEST_CASE("APP32_SetBit") {
        CHECK(524290 == APP32_SetBit(1, 524288));
        CHECK(8 == APP32_SetBit(400, 8));
        CHECK(8 == APP32_SetBit(-4, 8));
        CHECK(167772160 == APP32_SetBit(25, 134217728));
        CHECK(0 == APP32_SetBit(33, 8589934592));
    }
    TEST_CASE("APP32_TstBit") {
        CHECK(APP32_TstBit(19, 524288));
        CHECK_FALSE(APP32_TstBit(10, 524288));
        CHECK_FALSE(APP32_TstBit(33, 4294967295));
    }
    TEST_CASE("Pixie16AcquireADCTrace") {
        /// Cannot test this function in its current form.
    }
    TEST_CASE("Pixie16AcquireBaselines") {
        CHECK(Pixie16AcquireBaselines(1) == -1);
    }
    TEST_CASE("Pixie16AdjustOffsets") {
        CHECK(Pixie16AdjustOffsets(1) == -1);
    }
    TEST_CASE("Pixie16BLcutFinder") {
        unsigned int retval;
        CHECK(Pixie16BLcutFinder(0, 1, &retval) == -1);
    }
    TEST_CASE("Pixie16BootModule") {
        CHECK(Pixie16BootModule(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 1, 0x7F) == -1);
    }
    TEST_CASE("Pixie16CheckExternalFIFOStatus") {
        CHECK(Pixie16CheckExternalFIFOStatus(nullptr, 1) == -1);
    }
    TEST_CASE("Pixie16CheckRunStatus") {
        CHECK(Pixie16CheckRunStatus(1) == -1);
    }
    TEST_CASE("Pixie16complexFFT") {
        static const size_t length = 256;
        double data[length] = {
                0.0,     0, 0.00394, 0, 0.00787, 0, 0.01181, 0, 0.01575, 0, 0.01968, 0, 0.02362, 0, 0.02756, 0,
                0.03149, 0, 0.03543, 0, 0.03936, 0, 0.04329, 0, 0.04723, 0, 0.05116, 0, 0.05509, 0, 0.05902, 0,
                0.06295, 0, 0.06688, 0, 0.07081, 0, 0.07473, 0, 0.07866, 0, 0.08258, 0, 0.08651, 0, 0.09043, 0,
                0.09435, 0, 0.09827, 0, 0.10218, 0, 0.1061,  0, 0.11001, 0, 0.11393, 0, 0.11784, 0, 0.12174, 0,
                0.12565, 0, 0.12956, 0, 0.13346, 0, 0.13736, 0, 0.14126, 0, 0.14515, 0, 0.14905, 0, 0.15294, 0,
                0.15683, 0, 0.16072, 0, 0.1646,  0, 0.16848, 0, 0.17236, 0, 0.17624, 0, 0.18011, 0, 0.18399, 0,
                0.18785, 0, 0.19172, 0, 0.19558, 0, 0.19944, 0, 0.2033,  0, 0.20715, 0, 0.211,   0, 0.21485, 0,
                0.21869, 0, 0.22253, 0, 0.22637, 0, 0.2302,  0, 0.23403, 0, 0.23786, 0, 0.24168, 0, 0.2455,  0,
                0.24931, 0, 0.25312, 0, 0.25693, 0, 0.26073, 0, 0.26453, 0, 0.26832, 0, 0.27212, 0, 0.2759,  0,
                0.27968, 0, 0.28346, 0, 0.28723, 0, 0.291,   0, 0.29477, 0, 0.29853, 0, 0.30228, 0, 0.30603, 0,
                0.30978, 0, 0.31352, 0, 0.31726, 0, 0.32099, 0, 0.32471, 0, 0.32843, 0, 0.33215, 0, 0.33586, 0,
                0.33957, 0, 0.34327, 0, 0.34696, 0, 0.35065, 0, 0.35434, 0, 0.35802, 0, 0.36169, 0, 0.36536, 0,
                0.36902, 0, 0.37267, 0, 0.37633, 0, 0.37997, 0, 0.38361, 0, 0.38724, 0, 0.39087, 0, 0.39449, 0,
                0.3981,  0, 0.40171, 0, 0.40531, 0, 0.40891, 0, 0.4125,  0, 0.41608, 0, 0.41966, 0, 0.42323, 0,
                0.42679, 0, 0.43035, 0, 0.4339,  0, 0.43745, 0, 0.44098, 0, 0.44451, 0, 0.44804, 0, 0.45155, 0,
                0.45506, 0, 0.45856, 0, 0.46206, 0, 0.46555, 0, 0.46903, 0, 0.4725,  0, 0.47597, 0, 0.47943, 0};

        CHECK(Pixie16complexFFT(&data[0], length * 0.5) == 0);
        std::vector<double> result;
        for (unsigned int i = 0; i < length - 1; i += 2)
            result.push_back(std::abs(std::complex<double>(data[i], data[i + 1])));

        std::vector<double> expected_result_from_numpy = {
                31.33370189, 9.90883821, 4.93118791, 3.28607334, 2.46549191, 1.97388292, 1.64658857, 1.41313889,
                1.23832497,  1.10259511, 0.9942218,  0.90574373, 0.83218754, 0.77011063, 0.71705435, 0.67121593,
                0.63124348,  0.59610335, 0.56499177, 0.53727427, 0.51244345, 0.49008863, 0.46987393, 0.451522,
                0.43480182,  0.41951945, 0.40551087, 0.39263649, 0.38077676, 0.36982873, 0.35970329, 0.35032291,
                0.34161991,  0.33353488, 0.32601554, 0.31901569, 0.3124944,  0.30641526, 0.30074583, 0.29545713,
                0.29052321,  0.28592076, 0.28162886, 0.27762867, 0.27390321, 0.27043715, 0.26721666, 0.26422927,
                0.26146369,  0.25890975, 0.25655826, 0.25440097, 0.25243045, 0.25064003, 0.24902378, 0.24757641,
                0.24629328,  0.2451703,  0.24420395, 0.24339122, 0.2427296,  0.24221709, 0.24185211, 0.24163356,
                0.24156079,  0.24163356, 0.24185211, 0.24221709, 0.2427296,  0.24339122, 0.24420395, 0.2451703,
                0.24629328,  0.24757641, 0.24902378, 0.25064003, 0.25243045, 0.25440097, 0.25655826, 0.25890975,
                0.26146369,  0.26422927, 0.26721666, 0.27043715, 0.27390321, 0.27762867, 0.28162886, 0.28592076,
                0.29052321,  0.29545713, 0.30074583, 0.30641526, 0.3124944,  0.31901569, 0.32601554, 0.33353488,
                0.34161991,  0.35032291, 0.35970329, 0.36982873, 0.38077676, 0.39263649, 0.40551087, 0.41951945,
                0.43480182,  0.451522,   0.46987393, 0.49008863, 0.51244345, 0.53727427, 0.56499177, 0.59610335,
                0.63124348,  0.67121593, 0.71705435, 0.77011063, 0.83218754, 0.90574373, 0.9942218,  1.10259511,
                1.23832497,  1.41313889, 1.64658857, 1.97388292, 2.46549191, 3.28607334, 4.93118791, 9.90883821};
        for (unsigned int i = 0; i < result.size(); i++)
            CHECK(expected_result_from_numpy[i] == doctest::Approx(result[i]).epsilon(1e-4));
    }
    TEST_CASE("Pixie16ComputeFastFiltersOffline") {
        CHECK(Pixie16ComputeFastFiltersOffline(nullptr, 0, 0, 0, 0, nullptr, nullptr, nullptr) == -1);
        unsigned short trc[1] = {0};
        CHECK(Pixie16ComputeFastFiltersOffline(nullptr, 0, 0, 0, 1, trc, nullptr, nullptr) == -2);
        double filter[1] = {0};
        CHECK(Pixie16ComputeFastFiltersOffline(nullptr, 0, 0, 0, 1, trc, filter, nullptr) == -3);
        double cfd[1] = {0};
        CHECK(Pixie16ComputeFastFiltersOffline(nullptr, 40, 0, 0, 1, trc, filter, cfd) == -4);

        ///Cannot check anything else for this function as the next step is to start accessing hardware.
    }
    TEST_CASE("Statistics Computations") {
        uint32_t stats[N_DSP_PAR - DSP_IO_BORDER] = {
                0,          1899999978, 0,          1899999930, 0,          0,          100,        0,
                1,          6,          3,          9,          2159411345, 2159411345, 2159411345, 2159411345,
                2159411345, 2159411345, 2159411345, 2159411345, 2159411345, 2159411345, 2159411345, 2159411345,
                2159411345, 2159411345, 2159411345, 2159411345, 327680,     65536,      0,          0,
                2306,       1,          16380,      0,          0,          512,        802,        0,
                1418,       0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          2362135268,
                2374999609, 2374999626, 2374999644, 2374999661, 2374999679, 2374999696, 2374999714, 2374999732,
                2374999748, 2374999766, 2374999783, 2374999801, 2374999818, 2374999836, 2374999853, 0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          396141,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          197961,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          0,
                0,          0,          0,          0,          0,          0,          0,          201795124};

        unsigned short module_number = 0;
        Module_Information[module_number].Module_ADCMSPS = 250;

        SUBCASE("Pixie16ComputeLiveTime") {
            LiveTimeA_Address[module_number] = 0x0004a37f;
            LiveTimeB_Address[module_number] = 0x0004a38f;
            CHECK(18.9 == doctest::Approx(Pixie16ComputeLiveTime(stats, module_number, 0)).epsilon(0.1));
        }

        SUBCASE("Pixie16ComputeInputCountRate") {
            /// NOTE: This function uses LiveTime to compute its values.
            FastPeaksA_Address[module_number] = 0x0004a39f;
            FastPeaksB_Address[module_number] = 0x0004a3af;
            CHECK(20963 == doctest::Approx(Pixie16ComputeInputCountRate(stats, module_number, 0)).epsilon(1));
        }

        SUBCASE("Pixie16ComputeRealTime") {
            RunTimeA_Address[module_number] = 0x0004a342;
            RunTimeB_Address[module_number] = 0x0004a343;
            CHECK(18.9 == doctest::Approx(Pixie16ComputeRealTime(stats, module_number)).epsilon(0.1));
        }

        SUBCASE("Pixie16ComputeOutputCountRate") {
            /// NOTE: This function uses RunTime.
            ChanEventsA_Address[module_number] = 0x0004a41f;
            ChanEventsB_Address[module_number] = 0x0004a42f;
            CHECK(10419 == doctest::Approx(Pixie16ComputeOutputCountRate(stats, module_number, 0)).epsilon(1));
        }

        SUBCASE("Pixie16ComputeProcessedEvents") {
            NumEventsA_Address[module_number] = 0x0004a345;
            NumEventsB_Address[module_number] = 0x0004a346;
            CHECK(100 == Pixie16ComputeProcessedEvents(stats, module_number));
        }
    }

    TEST_CASE("Pixie16ComputeSlowFiltersOffline") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ControlTaskRun") {
        CHECK(false);
    }
    TEST_CASE("Pixie16CopyDSPParameters") {
        CHECK(false);
    }
    TEST_CASE("Pixie16EMbufferIO") {
        CHECK(false);
    }
    TEST_CASE("Pixie16EndRun") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ExitSystem") {
        CHECK(Pixie16ExitSystem(40) == -1);
    }
    TEST_CASE("Pixie16GetEventsInfo") {
        CHECK(false);
    }
    TEST_CASE("Pixie16GetModuleEvents") {
        CHECK(false);
    }
    TEST_CASE("Pixie16IMbufferIO") {
        CHECK(false);
    }
    TEST_CASE("Pixie16InitSystem") {
        SUBCASE("Tests around module number checking") {
            CHECK(Pixie16InitSystem(0, nullptr, 1) == -1);
            CHECK(Pixie16InitSystem(PRESET_MAX_MODULES + 1, nullptr, 1) == -1);
        }

        SUBCASE("Tests related to PXISlotMap verification") {
            CHECK(Pixie16InitSystem(1, nullptr, 1) == -2);
        }

        unsigned short slot_map[1] = {5};
        SUBCASE("Tests related to Initialization ") {
            CHECK(Pixie16InitSystem(1, slot_map, 1) == 0);
        }
    }
    TEST_CASE("Pixie16LoadDSPParametersFromFile") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ProgramFippi") {
        CHECK(false);
    }
    TEST_CASE("Pixie16RampOffsetDACs") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ReadCSR") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ReadDataFromExternalFIFO") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ReadHistogramFromFile") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ReadHistogramFromModule") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ReadListModeTrace") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ReadModuleInfo") {
        unsigned short dummy;
        unsigned int dummy1;
        CHECK(Pixie16ReadModuleInfo(40, &dummy, &dummy1, &dummy, &dummy) == -1);
    }
    TEST_CASE("Pixie16ReadSglChanADCTrace") {
        unsigned short dummy;
        unsigned short slot_map[1] = {5};

        CHECK(Pixie16ReadSglChanADCTrace(&dummy, 0, 10, 0) == -1);
        Pixie16InitSystem(1, slot_map, 1);
        CHECK(Pixie16ReadSglChanADCTrace(&dummy, 0, 0, 50) == -2);
        CHECK(Pixie16ReadSglChanADCTrace(&dummy, MAX_ADC_TRACE_LEN + 1, 0, 15) == -3);
    }
    TEST_CASE("Pixie16ReadSglChanBaselines") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ReadSglChanPar") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ReadSglModPar") {
        CHECK(false);
    }
    TEST_CASE("Pixie16ReadStatisticsFromModule") {
        CHECK(false);
    }
    TEST_CASE("Pixie16RegisterIO") {
        CHECK(false);
    }
    TEST_CASE("Pixie16SaveDSPParametersToFile") {
        CHECK(false);
    }
    TEST_CASE("Pixie16SaveHistogramToFile") {
        CHECK(false);
    }
    TEST_CASE("Pixie16SetDACs") {
        CHECK(false);
    }
    TEST_CASE("Pixie16SetOfflineVariant") {
        CHECK(false);
    }
    TEST_CASE("Pixie16StartHistogramRun") {
        CHECK(false);
    }
    TEST_CASE("Pixie16StartListModeRun") {
        CHECK(false);
    }
    TEST_CASE("Pixie16TauFinder") {
        CHECK(false);
    }
    TEST_CASE("Pixie16WriteCSR") {
        CHECK(false);
    }
    TEST_CASE("Pixie16WriteSglChanPar") {
        CHECK(false);
    }
    TEST_CASE("Pixie16WriteSglModPar") {
        CHECK(false);
    }
}
