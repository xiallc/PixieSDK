/// @file test_pixie16app.cpp
/// @brief Unittets
/// @author S. V. Paulauskas
/// @date November 23, 2020
#include "pixie16app_export.h"

#include "doctest.h"

TEST_SUITE("Pixie16App") {
    TEST_CASE("APP16_ClrBit"){CHECK(false);}
    TEST_CASE("APP16_SetBit"){CHECK(false);}
    TEST_CASE("APP16_TstBit"){CHECK(false);}
    TEST_CASE("APP32_ClrBit"){CHECK(false);}
    TEST_CASE("APP32_SetBit"){CHECK(false);}
    TEST_CASE("APP32_TstBit"){CHECK(false);}
    TEST_CASE("Pixie16AcquireADCTrace"){CHECK(false);}
    TEST_CASE("Pixie16AcquireBaselines"){CHECK(false);}
    TEST_CASE("Pixie16AdjustOffsets"){CHECK(false);}
    TEST_CASE("Pixie16BLcutFinder"){CHECK(false);}
    TEST_CASE("Pixie16BootModule"){CHECK(false);}
    TEST_CASE("Pixie16CheckExternalFIFOStatus"){CHECK(false);}
    TEST_CASE("Pixie16CheckRunStatus"){CHECK(false);}
    TEST_CASE("Pixie16complexFFT"){CHECK(false);}
    TEST_CASE("Pixie16ComputeFastFiltersOffline"){CHECK(false);}
    TEST_CASE("Pixie16ComputeLiveTime"){CHECK(false);}
    TEST_CASE("Pixie16ComputeInputCountRate"){CHECK(false);}
    TEST_CASE("Pixie16ComputeOutputCountRate"){CHECK(false);}
    TEST_CASE("Pixie16ComputeProcessedEvents"){CHECK(false);}
    TEST_CASE("Pixie16ComputeRealTime"){CHECK(false);}
    TEST_CASE("Pixie16ComputeSlowFiltersOffline"){CHECK(false);}
    TEST_CASE("Pixie16ControlTaskRun"){CHECK(false);}
    TEST_CASE("Pixie16CopyDSPParameters"){CHECK(false);}
    TEST_CASE("Pixie16EMbufferIO"){CHECK(false);}
    TEST_CASE("Pixie16EndRun"){CHECK(false);}
    TEST_CASE("Pixie16ExitSystem"){CHECK(false);}
    TEST_CASE("Pixie16GetEventsInfo"){CHECK(false);}
    TEST_CASE("Pixie16GetModuleEvents"){CHECK(false);}
    TEST_CASE("Pixie16IMbufferIO"){CHECK(false);}
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
    TEST_CASE("Pixie16LoadDSPParametersFromFile"){CHECK(false);}
    TEST_CASE("Pixie16ProgramFippi"){CHECK(false);}
    TEST_CASE("Pixie16RampOffsetDACs"){CHECK(false);}
    TEST_CASE("Pixie16ReadCSR"){CHECK(false);}
    TEST_CASE("Pixie16ReadDataFromExternalFIFO"){CHECK(false);}
    TEST_CASE("Pixie16ReadHistogramFromFile"){CHECK(false);}
    TEST_CASE("Pixie16ReadHistogramFromModule"){CHECK(false);}
    TEST_CASE("Pixie16ReadListModeTrace"){CHECK(false);}
    TEST_CASE("Pixie16ReadModuleInfo"){CHECK(false);}
    TEST_CASE("Pixie16ReadSglChanADCTrace"){CHECK(false);}
    TEST_CASE("Pixie16ReadSglChanBaselines"){CHECK(false);}
    TEST_CASE("Pixie16ReadSglChanPar"){CHECK(false);}
    TEST_CASE("Pixie16ReadSglModPar"){CHECK(false);}
    TEST_CASE("Pixie16ReadStatisticsFromModule"){CHECK(false);}
    TEST_CASE("Pixie16RegisterIO"){CHECK(false);}
    TEST_CASE("Pixie16SaveDSPParametersToFile"){CHECK(false);}
    TEST_CASE("Pixie16SaveHistogramToFile"){CHECK(false);}
    TEST_CASE("Pixie16SetDACs"){CHECK(false);}
    TEST_CASE("Pixie16SetOfflineVariant"){CHECK(false);}
    TEST_CASE("Pixie16StartHistogramRun"){CHECK(false);}
    TEST_CASE("Pixie16StartListModeRun"){CHECK(false);}
    TEST_CASE("Pixie16TauFinder"){CHECK(false);}
    TEST_CASE("Pixie16WriteCSR"){CHECK(false);}
    TEST_CASE("Pixie16WriteSglChanPar"){CHECK(false);}
    TEST_CASE("Pixie16WriteSglModPar"){CHECK(false);}
}
