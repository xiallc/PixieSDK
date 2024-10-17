/* SPDX-License-Identifier: Apache-2.0 */

/*
 * Copyright 2024 XIA LLC, All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file test_pixie16.c
 * @brief Tests the C API in a full C implementation.
 */
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include <pixie16/pixie16.h>

#include <acutest/acutest.h>

// Variables used in multiple test cases
#define MSGLEN (256)
static char errmsg[MSGLEN];
static const char settings[] = "data/capi/in.set";

struct test_module {
    char tag[9];
    struct module_config cfg;
    unsigned int hist_len;
    unsigned int adc_len;
    unsigned int baseline_len;
    unsigned int stats_len;
};

#define NUM_TEST_MODULES (4)
#define NUM_TEST_CHANS (80)
static struct test_module test_modules[NUM_TEST_MODULES] = {
    {"15-250-16", {16, 250, {}, {}, {}, {}, {}, 0, 16, 15, 250, 2}, 32768, 8192, 3640, 2600},
    {"15-500-14", {14, 500, {}, {}, {}, {}, {}, 1, 16, 15, 1000, 3}, 32768, 8192, 3640, 2600},
    {"15-250-14", {14, 250, {}, {}, {}, {}, {}, 2, 16, 15, 1001, 4}, 32768, 8192, 3640, 2600},
    {"17-250-14", {14, 250, {}, {}, {}, {}, {}, 3, 32, 17, 1002, 5}, 16384, 8192, 1927, 2600}};

static bool approx(const double lhs, const double rhs, const double epsilon) {
    if (fabs(lhs - rhs) > epsilon) {
        return false;
    }
    return true;
}

static bool compare_dbl_ary(const double* lhs, const double* rhs, const unsigned int len,
                            const double epsilon) {
    if (lhs == NULL || rhs == NULL) {
        return false;
    }
    for (unsigned int i = 0; i < len; i++) {
        if (!approx(lhs[i], rhs[i], epsilon)) {
            return false;
        }
    }
    return true;
}

static bool compare_uint_ary(const unsigned int* lhs, const unsigned int* rhs,
                             const unsigned int len) {
    if (lhs == NULL || rhs == NULL) {
        return false;
    }
    for (unsigned int i = 0; i < len; i++) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

static bool compare_ushort_ary(const unsigned short* lhs, const unsigned short* rhs,
                               const unsigned int len) {
    if (lhs == NULL || rhs == NULL) {
        return false;
    }
    for (unsigned int i = 0; i < len; i++) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

static char* concat(const char* s1, const char* s2) {
    char* result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

static char* tst_msg(char* msg, const unsigned int len, const int code_a, const int code_b) {
    memset(msg, 0, len);

    char txt_a[40] = {0};
    PixieGetReturnCodeText(code_a, txt_a, sizeof(txt_a));
    char txt_b[40] = {0};
    PixieGetReturnCodeText(code_b, txt_b, sizeof(txt_b));

    snprintf(msg, len, "%i: %s != %i: %s", code_a, txt_a, code_b, txt_b);
    return msg;
}

static void fill_dbl_ary(double* ary, const unsigned int len, const double val) {
    if (ary != NULL) {
        for (unsigned int i = 0; i < len; i++) {
            ary[i] = val;
        }
    }
}

static void fill_uint_ary(unsigned int* ary, const unsigned int len, const unsigned int val) {
    if (ary != NULL) {
        for (unsigned int i = 0; i < len; i++) {
            ary[i] = val;
        }
    }
}

static void fill_ushort_ary(unsigned short* ary, const unsigned int len, const unsigned short val) {
    if (ary != NULL) {
        for (unsigned int i = 0; i < len; i++) {
            ary[i] = val;
        }
    }
}

void init(void) {
    TEST_ASSERT(Pixie16InitSystem(NUM_TEST_MODULES, NULL, 1) == 0);
}

void boot(void) {
    int retval = PixieBootCrate(settings, PIXIE_BOOT_RESET_LOAD);
    TEST_ASSERT(retval == 0);
    TEST_MSG("PixieBootCrate | %s", tst_msg(errmsg, MSGLEN, retval, 0));
}

/*
 * All tests following this comment should be organized in alphabetical order to ease
 * test maintenance. Init and boot (above) are exceptions since many of the other tests
 * rely on those functions to operate.
 */

void adc(void) {
    int retval;
    unsigned int dummy_len;
    unsigned short* dummy = malloc(1 * sizeof(unsigned short));

    TEST_CASE("Uninitialized");
    {
        retval = PixieGetTraceLength(0, 0, &dummy_len);
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieGetTraceLength | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16AcquireADCTrace(0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16AcquireADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16ReadSglChanADCTrace(dummy, 0, 0, 0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16ReadSglChanADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        retval = PixieGetTraceLength(0, 0, &dummy_len);
        TEST_CHECK(retval == -204);
        TEST_MSG("PixieGetTraceLength | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16AcquireADCTrace(0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16AcquireADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16ReadSglChanADCTrace(dummy, 0, 0, 0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16ReadSglChanADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();

        TEST_CASE("Online.NullTraceArray");
        {
            retval = Pixie16ReadSglChanADCTrace(NULL, 0, 0, 5);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16ReadSglChanADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("Online.Read Before Acquire");
        {
            retval = Pixie16ReadSglChanADCTrace(dummy, 1, 1, 5);
            TEST_CHECK(retval == -206);
            TEST_MSG("Pixie16ReadSglChanADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -206));
        }

        TEST_CASE("Online.BadModule");
        {
            retval = PixieGetTraceLength(100, 1, &dummy_len);
            TEST_CHECK(retval == -200);
            TEST_MSG("PixieGetTraceLength | %s", tst_msg(errmsg, MSGLEN, retval, -200));

            retval = Pixie16AcquireADCTrace(100);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16AcquireADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -200));

            retval = Pixie16ReadSglChanADCTrace(dummy, 0, 100, 5);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16ReadSglChanADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }

        TEST_CASE("Online.BadChannel");
        {
            retval = PixieGetTraceLength(0, 1000, &dummy_len);
            TEST_CHECK(retval == -300);
            TEST_MSG("PixieGetTraceLength | %s", tst_msg(errmsg, MSGLEN, retval, -300));

            retval = Pixie16ReadSglChanADCTrace(dummy, 0, 0, 100);
            TEST_CHECK(retval == -300);
            TEST_MSG("Pixie16ReadSglChanADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -300));
        }

        for (unsigned int i = 0; i < NUM_TEST_MODULES; i++) {
            struct test_module* mod = &test_modules[i];

            unsigned int trc_len = 0;
            retval = PixieGetTraceLength(mod->cfg.number, 5, &trc_len);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieGetTraceLength | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            TEST_CHECK(trc_len == mod->adc_len);
            TEST_MSG("%i != %i", trc_len, mod->adc_len);

            retval = Pixie16AcquireADCTrace(mod->cfg.number);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16AcquireADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            unsigned short* trc = malloc(trc_len * sizeof(unsigned short));
            fill_ushort_ary(trc, trc_len, 0xBEEF);
            retval = Pixie16ReadSglChanADCTrace(trc, trc_len, mod->cfg.number, 5);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ReadSglChanADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            unsigned short* check = malloc(trc_len * sizeof(unsigned short));
            fill_ushort_ary(check, trc_len, 0);
            TEST_CHECK(compare_ushort_ary(trc, check, trc_len));
            TEST_MSG("trace data mismatch");

            free(trc);
            free(check);
        }
    }
    free(dummy);
}

void adjust_offsets(void) {
    int retval;
    TEST_CASE("Uninitialized");
    {
        retval = Pixie16AdjustOffsets(0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16AdjustOffsets | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();
        retval = Pixie16AdjustOffsets(0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16AdjustOffsets | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();

        TEST_CASE("Online.BadModule");
        {
            retval = Pixie16AdjustOffsets(100);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16AdjustOffsets | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }

        TEST_CASE("Online.AllModules");
        {
            retval = Pixie16AdjustOffsets(NUM_TEST_MODULES);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16AdjustOffsets | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }

        TEST_CASE("Online.HappyPath");
        {
            retval = Pixie16AdjustOffsets(0);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16AdjustOffsets | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void baseline(void) {
    int retval;
    double* dummy = malloc(0 * sizeof(double));

    TEST_CASE("NullArg");
    {
        TEST_CASE("NullArg.Baseline");
        {
            retval = Pixie16ReadSglChanBaselines(NULL, dummy, 0, 0, 5);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16ReadSglChanBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("NullArg.Timestamp");
        {
            retval = Pixie16ReadSglChanBaselines(dummy, NULL, 0, 0, 5);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16ReadSglChanBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }
    }

    TEST_CASE("Uninitialized");
    {
        retval = PixieGetMaxNumBaselines(0, 0, NULL);
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieGetMaxNumBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16AcquireBaselines(0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16AcquireBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        retval = PixieGetMaxNumBaselines(0, 0, NULL);
        TEST_CHECK(retval == -204);
        TEST_MSG("PixieGetMaxNumBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16AcquireBaselines(0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16AcquireBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16ReadSglChanBaselines(dummy, dummy, 0, 0, 0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16ReadSglChanBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();


        TEST_CASE("Online.AllModsWithRevH");
        {
            retval = Pixie16AcquireBaselines(NUM_TEST_MODULES);
            TEST_CHECK(retval == -803);
            TEST_MSG("Pixie16AcquireBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -803));
        }

        TEST_CASE("Online.ChanTooHigh");
        {
            retval = Pixie16ReadSglChanBaselines(dummy, dummy, 0, 0, 100);
            TEST_CHECK(retval == -300);
            TEST_MSG("Pixie16ReadSglChanBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -300));
        }

        for (unsigned int i = 0; i < NUM_TEST_MODULES; i++) {
            struct test_module* mod = &test_modules[i];
            TEST_CASE(concat("Online.", mod->tag));

            unsigned int baseline_len = 0;
            retval = PixieGetMaxNumBaselines(mod->cfg.number, 5, &baseline_len);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieGetMaxNumBaselines | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            TEST_CHECK(baseline_len == mod->baseline_len);
            TEST_MSG("%i != %i", baseline_len, mod->baseline_len);

            if (mod->cfg.revision < 17) {
                retval = Pixie16AcquireBaselines(mod->cfg.number);
                TEST_CHECK(retval == 0);
                TEST_MSG("Pixie16AcquireBaselines | %s", tst_msg(errmsg, MSGLEN, retval, 0));

                double* baselines = malloc(baseline_len * sizeof(double));
                fill_dbl_ary(baselines, baseline_len, 0xDEADBEEF);
                double* timestamps = malloc(baseline_len * sizeof(double));
                fill_dbl_ary(timestamps, baseline_len, 0xDEADBEEF);
                retval = Pixie16ReadSglChanBaselines(baselines, timestamps, baseline_len,
                                                     mod->cfg.number, 5);
                TEST_CHECK(retval == 0);
                TEST_MSG("Pixie16ReadSglChanBaselines | %s", tst_msg(errmsg, MSGLEN, retval, 0));

                double* check = malloc(baseline_len * sizeof(double));
                fill_dbl_ary(check, baseline_len, 0);
                TEST_CHECK(compare_dbl_ary(baselines, check, baseline_len, 0.001));
                TEST_MSG("baselines data mismatch");

                TEST_CHECK(compare_dbl_ary(timestamps, check, baseline_len, 0.001));
                TEST_MSG("timestamps data mismatch");

                free(baselines);
                free(timestamps);
                free(check);
            } else {
                retval = Pixie16AcquireBaselines(mod->cfg.number);
                TEST_CHECK(retval == -803);
                TEST_MSG("Pixie16AcquireBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -803));

                retval = Pixie16ReadSglChanBaselines(dummy, dummy, 0, mod->cfg.number, 5);
                TEST_CHECK(retval == -206);
                TEST_MSG("Pixie16ReadSglChanBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -206));
            }
        }
    }
    free(dummy);
}

void blcut(void) {
    int retval;
    unsigned int val = 0;
    TEST_CASE("Uninitialized");
    {
        retval = Pixie16BLcutFinder(0, 0, &val);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16BLcutFinder | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();
        retval = Pixie16BLcutFinder(0, 0, &val);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16BLcutFinder | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();

        TEST_CASE("Online.BadModule");
        {
            retval = Pixie16BLcutFinder(100, 0, &val);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16BLcutFinder | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }

        TEST_CASE("Online.BadChannel");
        {
            retval = Pixie16BLcutFinder(0, 100, &val);
            TEST_CHECK(retval == -300);
            TEST_MSG("Pixie16BLcutFinder | %s", tst_msg(errmsg, MSGLEN, retval, -300));
        }

        TEST_CASE("Online.NullValPointer");
        {
            retval = Pixie16BLcutFinder(0, 0, NULL);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16BLcutFinder | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        for (unsigned int i = 0; i < NUM_TEST_MODULES; i++) {
            struct test_module* mod = &test_modules[i];
            TEST_CASE(concat("Online.", mod->tag));
            int expected = 0;
            if (mod->cfg.revision == 17) {
                expected = -803;
            }
            retval = Pixie16BLcutFinder(mod->cfg.number, 0, &val);
            TEST_CHECK(retval == expected);
            TEST_MSG("Pixie16BLcutFinder | %s", tst_msg(errmsg, MSGLEN, retval, expected));
        }
    }
}

void boot_crate(void) {
    int retval;
    TEST_CASE("Uninitialized");
    {
        retval = PixieBootCrate(settings, PIXIE_BOOT_RESET_LOAD);
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieBootCrate | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();
        retval = PixieBootCrate(settings, PIXIE_BOOT_RESET_LOAD);
        TEST_CHECK(retval == 0);
        TEST_MSG("PixieBootCrate | %s", tst_msg(errmsg, MSGLEN, retval, 0));
    }
}

void boot_mod_fw(void) {
    int retval;
    TEST_CASE("Uninitialized");
    {
        retval = Pixie16BootModuleFirmware(settings, 0, 0x7F);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16BootModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        TEST_CASE("Offline.Nulls");
        {
            retval = Pixie16BootModuleFirmware(NULL, 0, 0x7F);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16BootModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("Offline.Bad Mod Num");
        {
            retval = Pixie16BootModuleFirmware(settings, 700, 0x7F);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }

        TEST_CASE("Offline.Bad Boot Pattern");
        {
            retval = Pixie16BootModuleFirmware(settings, 0, 0x99);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16BootModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("Offline.Happy Path");
        {
            retval = Pixie16BootModuleFirmware(settings, 0, 0x7F);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void boot_module(void) {
    int retval;
    TEST_CASE("Uninitialized");
    {
        retval = Pixie16BootModule(" ", " ", NULL, " ", settings, " ", 0, 0x7F);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        TEST_CASE("Offline.Nulls");
        {
            TEST_CASE("Offline.Nulls.System");
            {
                retval = Pixie16BootModule(NULL, "tdee", NULL, "tdum", settings, "walrus", 0, 0x7F);
                TEST_CHECK(retval == -802);
                TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
            TEST_CASE("Offline.Nulls.FIPPI");
            {
                retval =
                    Pixie16BootModule("rqueen", NULL, NULL, "tdum", settings, "walrus", 0, 0x7F);
                TEST_CHECK(retval == -802);
                TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
            TEST_CASE("Offline.Nulls.DSP Code");
            {
                retval =
                    Pixie16BootModule("rqueen", "tdee", NULL, NULL, settings, "walrus", 0, 0x7F);
                TEST_CHECK(retval == -802);
                TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
            TEST_CASE("Offline.Nulls.Settings");
            {
                retval = Pixie16BootModule("rqueen", "tdee", NULL, "tdum", NULL, "walrus", 0, 0x7F);
                TEST_CHECK(retval == -802);
                TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
            TEST_CASE("Offline.Nulls.DSP Var");
            {
                retval = Pixie16BootModule("rqueen", "tdee", NULL, "tdum", settings, NULL, 0, 0x7F);
                TEST_CHECK(retval == -802);
                TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
        }

        TEST_CASE("Offline.Bad Mod Num");
        {
            retval = Pixie16BootModule("sys.bin", "fippi.bin", NULL, "dsp.ldr", settings, "dsp.var",
                                       700, 0x7F);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }

        TEST_CASE("Offline.Bad Boot Pattern");
        {
            retval = Pixie16BootModule("rqueen", "tdee", NULL, "tdum", settings, "walrus", 0, 0x99);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("Offline.Happy Path");
        {
            retval = Pixie16BootModule("sys.bin", "fippi.bin", NULL, "dsp.ldr", settings, "dsp.var",
                                       0, 0x7F);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void chan_par(void) {
    int retval;
    double dummy = 0;

    TEST_CASE("Nulls.Name");
    {
        retval = Pixie16ReadSglChanPar(NULL, &dummy, 0, 0);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16ReadSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -802));

        retval = Pixie16WriteSglChanPar(NULL, 0.3, 0, 0);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16WriteSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Nulls.Value");
    {
        retval = Pixie16ReadSglChanPar("VOFFSET", NULL, 0, 0);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16ReadSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Uninitialized");
    {
        retval = Pixie16ReadSglChanPar("VOFFSET", &dummy, 0, 0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16ReadSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16WriteSglChanPar("VOFFSET", 0.3, 0, 0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16WriteSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();
        retval = Pixie16ReadSglChanPar("VOFFSET", &dummy, 0, 0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16ReadSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16WriteSglChanPar("VOFFSET", 0.3, 0, 0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16WriteSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();
        double voffset = -1000.0;
        retval = Pixie16ReadSglChanPar("VOFFSET", &voffset, 3, 5);
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16ReadSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        TEST_CHECK(approx(voffset, 0.0999, 0.0001));
        TEST_MSG("%f != %f", voffset, 0.099);

        retval = Pixie16WriteSglChanPar("VOFFSET", 0.3, 3, 5);
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16WriteSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, 0));

        retval = Pixie16ReadSglChanPar("VOFFSET", &voffset, 3, 5);
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16ReadSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        TEST_CHECK(approx(voffset, 0.299, 0.001));
        TEST_MSG("%f != %f", voffset, 0.299);
    }
}

void cleanup(void) {
    int retval;

    TEST_CASE("Uninitialized");
    {
        retval = Pixie16ExitSystem(0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16ExitSystem | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();
        retval = Pixie16ExitSystem(0);
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16ExitSystem | %s", tst_msg(errmsg, MSGLEN, retval, 0));
    }

    TEST_CASE("Online");
    {
        boot();

        TEST_CASE("Online.CloseOnline");
        {
            retval = Pixie16ExitSystem(1);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ExitSystem | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }

        TEST_CASE("Online.CloseOffline");
        {
            retval = Pixie16ExitSystem(0);
            TEST_CHECK(retval == -204);
            TEST_MSG("Pixie16ExitSystem | %s", tst_msg(errmsg, MSGLEN, retval, -204));
        }

        TEST_CASE("Online.BadModNum");
        {
            retval = Pixie16ExitSystem(100);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16ExitSystem | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }
    }
}

void cleanup_all(void) {
    int retval;

    init();
    boot();

    TEST_CASE("Close All Modules");
    {
        retval = Pixie16ExitSystem(NUM_TEST_MODULES);
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16ExitSystem | %s", tst_msg(errmsg, MSGLEN, retval, 0));
    }

    TEST_CASE("Close All Modules...Again");
    {
        retval = Pixie16ExitSystem(NUM_TEST_MODULES);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16ExitSystem | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }
}

void copy_par(void) {
    int retval;
    unsigned short dest[NUM_TEST_CHANS] = {0};

    TEST_CASE("Null.Destination");
    {
        retval = Pixie16CopyDSPParameters(0, 0, 0, NULL);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16CopyDSPParameters | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Uninitialized");
    {
        retval = Pixie16CopyDSPParameters(0, 0, 0, dest);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16CopyDSPParameters | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        retval = Pixie16CopyDSPParameters(0, 0, 0, dest);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16CopyDSPParameters | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();

        TEST_CASE("Online.Bad Mod Num");
        {
            retval = Pixie16CopyDSPParameters(8, 100, 0, dest);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16CopyDSPParameters | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }

        TEST_CASE("Online.Bad Chan Num");
        {
            retval = Pixie16CopyDSPParameters(8, 0, 1000, dest);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16CopyDSPParameters | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("Online.Short Dest Mask");
        {
            unsigned short short_mask[1] = {1337};
            retval = Pixie16CopyDSPParameters(8, 2, 14, short_mask);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16CopyDSPParameters | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }

        TEST_CASE("Online.Happy Path");
        {
            unsigned short mask[NUM_TEST_CHANS];
            fill_ushort_ary(mask, NUM_TEST_CHANS, 0);
            /* Sets the copy destination to Module 3 Channel 20 */
            mask[68] = 1;

            double expected = 0.1337;
            retval = Pixie16WriteSglChanPar("TAU", expected, 1, 14);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16WriteSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            double val = 0;
            retval = Pixie16ReadSglChanPar("TAU", &val, 1, 14);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ReadSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(approx(val, expected, 0.0001));
            TEST_MSG("%f != %f", val, expected);

            retval = Pixie16CopyDSPParameters(8, 1, 14, mask);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16CopyDSPParameters | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            val = 0;
            retval = Pixie16ReadSglChanPar("TAU", &val, 3, 20);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ReadSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(approx(val, expected, 0.0001));
            TEST_MSG("%f != %f", val, expected);
        }
    }
}

void fifo(void) {
    int retval;
    struct module_fifo_config fifo_cfg;

    TEST_CASE("Nulls");
    {
        retval = PixieGetFifoConfiguration(0, NULL);
        TEST_CHECK(retval == -205);
        TEST_MSG("PixieGetFifoConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, -205));

        retval = PixieSetFifoConfiguration(0, NULL);
        TEST_CHECK(retval == -205);
        TEST_MSG("PixieSetFifoConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, -205));
    }

    TEST_CASE("Uninitialized");
    {
        retval = PixieGetFifoConfiguration(0, &fifo_cfg);
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieGetFifoConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = PixieSetFifoConfiguration(0, &fifo_cfg);
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieSetFifoConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        struct module_fifo_config exp = {10, 150, 2048, 14000, 15000, 6000};

        TEST_CASE("Offline.Happy Path");
        {
            retval = PixieSetFifoConfiguration(0, &exp);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSetFifoConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = PixieGetFifoConfiguration(0, &fifo_cfg);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieGetFifoConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            TEST_CHECK(fifo_cfg.bandwidth_mb_per_sec == exp.bandwidth_mb_per_sec);
            TEST_MSG("%zu != %zu", fifo_cfg.bandwidth_mb_per_sec, exp.bandwidth_mb_per_sec);

            TEST_CHECK(fifo_cfg.buffers == exp.buffers);
            TEST_MSG("%zu != %zu", fifo_cfg.buffers, exp.buffers);

            TEST_CHECK(fifo_cfg.dma_trigger_level_bytes == exp.dma_trigger_level_bytes);
            TEST_MSG("%zu != %zu", fifo_cfg.dma_trigger_level_bytes, exp.dma_trigger_level_bytes);

            TEST_CHECK(fifo_cfg.hold_usecs == exp.hold_usecs);
            TEST_MSG("%zu != %zu", fifo_cfg.hold_usecs, exp.hold_usecs);

            TEST_CHECK(fifo_cfg.idle_wait_usecs == exp.idle_wait_usecs);
            TEST_MSG("%zu != %zu", fifo_cfg.idle_wait_usecs, exp.idle_wait_usecs);

            TEST_CHECK(fifo_cfg.run_wait_usecs == exp.run_wait_usecs);
            TEST_MSG("%zu != %zu", fifo_cfg.run_wait_usecs, exp.run_wait_usecs);
        }

        TEST_CASE("Offline.Out Of Bounds");
        {
            exp.buffers = 5;
            retval = PixieSetFifoConfiguration(0, &exp);
            TEST_CHECK(retval == -210);
            TEST_MSG("PixieSetFifoConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, -210));
        }
    }
}

void ieee_float(void) {
    double val = 133.7;
    unsigned int ieee_val = 0x4305b333;
    TEST_CASE("IEEEFloating2Decimal");
    {
        double result = IEEEFloating2Decimal(ieee_val);
        TEST_CHECK(approx(result, val, 0.01));
        TEST_MSG("%f != %f", result, val);
    }

    TEST_CASE("Decimal2IEEEFloating");
    {
        unsigned int result = Decimal2IEEEFloating(val);
        TEST_CHECK(result == ieee_val);
        TEST_MSG("%i != %i", result, ieee_val);
    }
}

void init_system(void) {
    int retval;

    TEST_CASE("Too Many Mods");
    {
        retval = Pixie16InitSystem(100, NULL, 1);
        TEST_CHECK(retval == -201);
        TEST_MSG("Pixie16InitSystem | %s", tst_msg(errmsg, MSGLEN, retval, -201));
    }

    TEST_CASE("Zero Mods");
    {
        retval = Pixie16InitSystem(0, NULL, 1);
        TEST_CHECK(retval == -201);
        TEST_MSG("Pixie16InitSystem | %s", tst_msg(errmsg, MSGLEN, retval, -201));
    }

    TEST_CASE("Happy Path");
    {
        retval = Pixie16InitSystem(NUM_TEST_MODULES, NULL, 1);
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16InitSystem | %s", tst_msg(errmsg, MSGLEN, retval, 0));
    }

    TEST_CASE("Init again");
    {
        retval = Pixie16InitSystem(NUM_TEST_MODULES, NULL, 1);
        TEST_CHECK(retval == -100);
        TEST_MSG("Pixie16InitSystem | %s", tst_msg(errmsg, MSGLEN, retval, -100));
    }
}

void init_order(void) {
    unsigned short order[] = {5, 4, 3, 2};
    int retval = Pixie16InitSystem(NUM_TEST_MODULES, order, 1);
    TEST_CHECK(retval == 0);
    TEST_MSG("Pixie16InitSystem | %s", tst_msg(errmsg, MSGLEN, retval, 0));

    unsigned int expected_serial_nums[] = {1002, 1001, 1000, 250};
    for (unsigned int i = 0; i < NUM_TEST_MODULES; i++) {
        struct module_config returned;
        retval = PixieGetModuleInfo(i, &returned);
        TEST_CHECK(retval == 0);
        TEST_MSG("PixieGetModuleInfo | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        TEST_CHECK(returned.serial_number == expected_serial_nums[i]);
        TEST_MSG("%i != %i", returned.serial_number, expected_serial_nums[i]);
    }
}

void install_path(void) {
    TEST_CASE("Happy Path");
    {
        const char* exp =
#ifdef XIA_PIXIE_WINDOWS
            "C:\\Program Files\\xia\\pixie-16\\firmware";
#else
            "/usr/local/xia/pixie-16/firmware";
#endif
        const char* path = PixieGetInstallationPath(PIXIE_PATH_FIRMWARE_DEFAULT);
        TEST_CHECK(strcmp(path, exp) == 0);
        TEST_MSG("PIXIE_PATH_FIRMWARE_DEFAULT | %s != %s", path, exp);
    }

    TEST_CASE("BadVal");
    {
        const char* path = PixieGetInstallationPath(100);
        TEST_CHECK(!path);
        TEST_MSG("%s != NULL", path);
    }
}

void list_mode(void) {
    int retval;

    TEST_CASE("NullArgs");
    {
        TEST_CASE("NullArgs.FifoStatus");
        {
            retval = Pixie16CheckExternalFIFOStatus(NULL, 0);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16CheckExternalFIFOStatus | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("NullArgs.ReadData");
        {
            retval = Pixie16ReadDataFromExternalFIFO(NULL, 0, 0);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16ReadDataFromExternalFIFO | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }
    }

    TEST_CASE("Uninitialized");
    {
        retval = Pixie16CheckRunStatus(0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16CheckRunStatus | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16StartListModeRun(0, 0x100, 1);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16StartListModeRun | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16EndRun(0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16EndRun | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();
        retval = Pixie16StartListModeRun(0, 0x100, 1);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16StartListModeRun | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16EndRun(0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16EndRun | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();

        TEST_CASE("Online.BadRunType");
        {
            retval = Pixie16StartListModeRun(NUM_TEST_MODULES, 0xDEAD, 1);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("Online.BadMode");
        {
            retval = Pixie16StartListModeRun(NUM_TEST_MODULES, 0x100, 1337);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("Online.FifoStatus.BadMod");
        {
            unsigned int num_fifo_words = 999999;
            retval = Pixie16CheckExternalFIFOStatus(&num_fifo_words, 100);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16CheckExternalFIFOStatus | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }

        TEST_CASE("Online.RunAllMods");
        {
            retval = Pixie16StartListModeRun(NUM_TEST_MODULES, 0x100, 1);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            for (unsigned int i = 0; i < NUM_TEST_MODULES; i++) {
                retval = Pixie16CheckRunStatus(i);
                TEST_CHECK(retval == 1);
                TEST_MSG("Pixie16CheckRunStatus(%i) | %i != 1", i, retval);
            }

            retval = Pixie16EndRun(NUM_TEST_MODULES);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16EndRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }

        TEST_CASE("Online.ReadTooMuchData");
        {
            unsigned int* dummy = malloc(1 * sizeof(unsigned int));
            retval = Pixie16ReadDataFromExternalFIFO(dummy, 100, 0);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ReadDataFromExternalFIFO | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            free(dummy);
        }

        TEST_CASE("Online.HappyPath");
        {
            retval = Pixie16CheckRunStatus(3);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16CheckRunStatus | %i != 0", retval);

            retval = Pixie16StartListModeRun(3, 0x100, 1);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16StartListModeRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = Pixie16CheckRunStatus(3);
            TEST_CHECK(retval == 1);
            TEST_MSG("Pixie16CheckRunStatus | %i != 1", retval);

            retval = Pixie16EndRun(3);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16EndRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = Pixie16CheckRunStatus(3);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16CheckRunStatus | %i != 0", retval);

            unsigned int num_fifo_words = 999999;
            retval = Pixie16CheckExternalFIFOStatus(&num_fifo_words, 3);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16CheckExternalFIFOStatus | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            unsigned int* data = malloc(num_fifo_words * sizeof(unsigned int));
            fill_uint_ary(data, num_fifo_words, 0xDEADBEEF);
            retval = Pixie16ReadDataFromExternalFIFO(data, num_fifo_words, 3);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ReadDataFromExternalFIFO | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            unsigned int* check = malloc(num_fifo_words * sizeof(unsigned int));
            fill_uint_ary(check, num_fifo_words, 0);
            TEST_CHECK(compare_uint_ary(data, check, num_fifo_words));
            TEST_MSG("fifo data mismatch");

            free(data);
            free(check);
        }
    }
}

void load_mod_fw(void) {
    int retval;
    TEST_CASE("Uninitialized");
    {
        retval = Pixie16LoadModuleFirmware("");
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16LoadModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();
        TEST_CASE("Offline.Null");
        {
            retval = Pixie16LoadModuleFirmware(NULL);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16LoadModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("Offline.Empty Path");
        {
            retval = Pixie16LoadModuleFirmware("");
            TEST_CHECK(retval == -900);
            TEST_MSG("Pixie16LoadModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -900));
        }

        TEST_CASE("Offline.Happy Path");
        {
            retval = Pixie16LoadModuleFirmware("./");
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16LoadModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void load_settings(void) {
    int retval;
    TEST_CASE("Uninitialized");
    {
        retval = Pixie16LoadDSPParametersFromFile(settings);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16LoadDSPParametersFromFile | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();
        retval = Pixie16LoadDSPParametersFromFile(settings);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16LoadDSPParametersFromFile | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();
        retval = Pixie16LoadDSPParametersFromFile(settings);
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16LoadDSPParametersFromFile | %s", tst_msg(errmsg, MSGLEN, retval, 0));
    }
}

void mca(void) {
    int retval;
    unsigned int dummy_len;
    unsigned int* dummy = malloc(1 * sizeof(unsigned short));

    TEST_CASE("Null Length");
    {
        retval = PixieGetHistogramLength(0, 0, NULL);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieGetHistogramLength | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Uninitialized");
    {
        retval = PixieGetHistogramLength(0, 0, &dummy_len);
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieGetHistogramLength | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16CheckRunStatus(0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16CheckRunStatus | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16StartHistogramRun(0, 1);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16EndRun(0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16EndRun | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16ReadHistogramFromModule(dummy, 0, 0, 0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16ReadHistogramFromModule | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        retval = PixieGetHistogramLength(0, 0, dummy);
        TEST_CHECK(retval == -204);
        TEST_MSG("PixieGetHistogramLength | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16CheckRunStatus(0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16CheckRunStatus | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16StartHistogramRun(0, 1);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16EndRun(0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16EndRun | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16ReadHistogramFromModule(dummy, 0, 0, 0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16ReadHistogramFromModule | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();
        unsigned int hist_len = 0;

        TEST_CASE("Online.Bad Module");
        {
            retval = PixieGetHistogramLength(100, 1, &hist_len);
            TEST_CHECK(retval == -200);
            TEST_MSG("PixieGetHistogramLength | %s", tst_msg(errmsg, MSGLEN, retval, -200));

            retval = Pixie16CheckRunStatus(100);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16CheckRunStatus | %s", tst_msg(errmsg, MSGLEN, retval, -200));

            retval = Pixie16StartHistogramRun(100, 1);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, -200));

            retval = Pixie16EndRun(100);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16EndRun | %s", tst_msg(errmsg, MSGLEN, retval, -200));

            retval = Pixie16ReadHistogramFromModule(dummy, hist_len, 100, 1);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16ReadHistogramFromModule | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }

        TEST_CASE("Online.Bad Channel");
        {
            retval = PixieGetHistogramLength(0, 100, &hist_len);
            TEST_CHECK(retval == -300);
            TEST_MSG("PixieGetHistogramLength | %s", tst_msg(errmsg, MSGLEN, retval, -300));

            retval = Pixie16ReadHistogramFromModule(dummy, hist_len, 0, 100);
            TEST_CHECK(retval == -300);
            TEST_MSG("Pixie16ReadHistogramFromModule | %s", tst_msg(errmsg, MSGLEN, retval, -300));
        }

        TEST_CASE("Online.Bad Mode");
        {
            retval = Pixie16StartHistogramRun(0, 12);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("Online.Run All Mods");
        {
            retval = Pixie16StartHistogramRun(NUM_TEST_MODULES, 1);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            for (unsigned int i = 0; i < NUM_TEST_MODULES; i++) {
                retval = Pixie16CheckRunStatus(i);
                TEST_CHECK(retval == 1);
                TEST_MSG("Pixie16CheckRunStatus(%i) | %i != 1", i, retval);
            }

            retval = Pixie16EndRun(NUM_TEST_MODULES);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16EndRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }

        for (unsigned int i = 0; i < NUM_TEST_MODULES; i++) {
            struct test_module* mod = &test_modules[i];
            TEST_CASE(concat("Online.Individual.", mod->tag));

            hist_len = 0;
            retval = PixieGetHistogramLength(mod->cfg.number, 5, &hist_len);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieGetHistogramLength | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            TEST_CHECK(hist_len == mod->hist_len);
            TEST_MSG("%i != %i", hist_len, mod->hist_len);

            retval = Pixie16CheckRunStatus(mod->cfg.number);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16CheckRunStatus | %i != 0", retval);

            retval = Pixie16StartHistogramRun(mod->cfg.number, 1);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = Pixie16CheckRunStatus(mod->cfg.number);
            TEST_CHECK(retval == 1);
            TEST_MSG("Pixie16CheckRunStatus | %i != 1", retval);

            retval = Pixie16EndRun(mod->cfg.number);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16EndRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = Pixie16CheckRunStatus(mod->cfg.number);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16CheckRunStatus | %i != 0", retval);

            unsigned int* hist = malloc(hist_len * sizeof(unsigned int));
            fill_uint_ary(hist, hist_len, 0xDEADBEEF);
            retval = Pixie16ReadHistogramFromModule(hist, hist_len, mod->cfg.number, 5);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ReadHistogramFromModule | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            unsigned int* check = malloc(hist_len * sizeof(unsigned int));
            fill_uint_ary(check, hist_len, 0);
            TEST_CHECK(compare_uint_ary(hist, check, hist_len));
            TEST_MSG("histogram data mismatch");

            free(hist);
            free(check);
        }
    }
    free(dummy);
}

void mod_info(void) {
    int retval;
    unsigned short dummyA = 0;
    unsigned int dummyB = 0;
    struct module_config mod_cfg;

    TEST_CASE("Nulls");
    {
        retval = Pixie16ReadModuleInfo(0, NULL, &dummyB, &dummyA, &dummyA);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16ReadModuleInfo | %s", tst_msg(errmsg, MSGLEN, retval, -802));

        retval = Pixie16ReadModuleInfo(0, &dummyA, NULL, &dummyA, &dummyA);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16ReadModuleInfo | %s", tst_msg(errmsg, MSGLEN, retval, -802));

        retval = Pixie16ReadModuleInfo(0, &dummyA, &dummyB, NULL, &dummyA);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16ReadModuleInfo | %s", tst_msg(errmsg, MSGLEN, retval, -802));

        retval = Pixie16ReadModuleInfo(0, &dummyA, &dummyB, &dummyA, NULL);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16ReadModuleInfo | %s", tst_msg(errmsg, MSGLEN, retval, -802));

        retval = PixieGetModuleInfo(0, NULL);
        TEST_CHECK(retval == -205);
        TEST_MSG("PixieGetModuleInfo | %s", tst_msg(errmsg, MSGLEN, retval, -205));
    }

    TEST_CASE("Uninitialized");
    {
        retval = Pixie16ReadModuleInfo(0, &dummyA, &dummyB, &dummyA, &dummyA);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16ReadModuleInfo | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = PixieGetModuleInfo(0, &mod_cfg);
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieGetModuleInfo | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        TEST_CASE("Offline.Get");
        {
            for (unsigned int i = 0; i < NUM_TEST_MODULES; i++) {
                struct module_config* cfg = &test_modules[i].cfg;
                struct module_config returned;

                TEST_CASE(concat("Offline.", test_modules[i].tag));

                retval = PixieGetModuleInfo(i, &returned);
                TEST_CHECK(retval == 0);
                TEST_MSG("PixieGetModuleInfo | %s", tst_msg(errmsg, MSGLEN, retval, 0));

                TEST_CHECK(returned.adc_bit_resolution == cfg->adc_bit_resolution);
                TEST_MSG("%i != %i", returned.adc_bit_resolution, cfg->adc_bit_resolution);

                TEST_CHECK(returned.adc_sampling_frequency == cfg->adc_sampling_frequency);
                TEST_MSG("%i != %i", returned.adc_sampling_frequency, cfg->adc_sampling_frequency);

                TEST_CHECK(returned.number == cfg->number);
                TEST_MSG("%i != %i", returned.number, cfg->number);

                TEST_CHECK(returned.number_of_channels == cfg->number_of_channels);
                TEST_MSG("%i != %i", returned.number_of_channels, cfg->number_of_channels);

                TEST_CHECK(returned.revision == cfg->revision);
                TEST_MSG("%i != %i", returned.revision, cfg->revision);

                TEST_CHECK(returned.serial_number == cfg->serial_number);
                TEST_MSG("%i != %i", returned.serial_number, cfg->serial_number);

                TEST_CHECK(returned.slot == cfg->slot);
                TEST_MSG("%i != %i", returned.slot, cfg->slot);
            }
        }

        TEST_CASE("Offline.Read");
        {
            unsigned short rev = 0;
            unsigned short adc_bits = 0;
            unsigned short adc_freq = 0;
            unsigned int serial = 0;

            for (unsigned int i = 0; i < NUM_TEST_MODULES; i++) {
                struct module_config* cfg = &test_modules[i].cfg;

                TEST_CASE(concat("Offline.", test_modules[i].tag));

                retval = Pixie16ReadModuleInfo(cfg->number, &rev, &serial, &adc_bits, &adc_freq);
                TEST_CHECK(retval == 0);
                TEST_MSG("Pixie16ReadModuleInfo | %s", tst_msg(errmsg, MSGLEN, retval, 0));

                TEST_CHECK(rev == cfg->revision);
                TEST_MSG("%i != %i", rev, cfg->revision);

                TEST_CHECK(adc_bits == cfg->adc_bit_resolution);
                TEST_MSG("%i != %i", adc_bits, cfg->adc_bit_resolution);

                TEST_CHECK(adc_freq == cfg->adc_sampling_frequency);
                TEST_MSG("%i != %i", adc_freq, cfg->adc_sampling_frequency);

                TEST_CHECK(serial == cfg->serial_number);
                TEST_MSG("%i != %i", serial, cfg->serial_number);
            }
        }
    }
}

void mod_par(void) {
    int retval;
    unsigned int dummy = 0;

    TEST_CASE("Nulls.Name");
    {
        retval = Pixie16ReadSglModPar(NULL, &dummy, 0);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16ReadSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -802));

        retval = Pixie16WriteSglModPar(NULL, 0, 0);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16WriteSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Nulls.Value");
    {
        retval = Pixie16ReadSglModPar("SLOW_FILTER_RANGE", NULL, 0);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16ReadSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }


    TEST_CASE("Uninitialized");
    {
        retval = Pixie16ReadSglModPar("SLOW_FILTER_RANGE", &dummy, 0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16ReadSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        retval = Pixie16WriteSglModPar("SLOW_FILTER_RANGE", 4, 0);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16WriteSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }


    TEST_CASE("Offline");
    {
        init();
        retval = Pixie16ReadSglModPar("SLOW_FILTER_RANGE", &dummy, 0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16ReadSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        retval = Pixie16WriteSglModPar("SLOW_FILTER_RANGE", 4, 0);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16WriteSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();

        TEST_CASE("Online.HappyPath");
        {
            unsigned int slow_filter_range = 9999;
            retval = Pixie16ReadSglModPar("SLOW_FILTER_RANGE", &slow_filter_range, 3);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ReadSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(slow_filter_range == 3);
            TEST_MSG("%i != %i", slow_filter_range, 3);

            retval = Pixie16WriteSglModPar("SLOW_FILTER_RANGE", 4, 3);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16WriteSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = Pixie16ReadSglModPar("SLOW_FILTER_RANGE", &slow_filter_range, 3);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ReadSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(slow_filter_range == 4);
            TEST_MSG("%i != %i", slow_filter_range, 4);
        }

        TEST_CASE("Online.Broadcast");
        {
            retval = Pixie16WriteSglModPar("SLOW_FILTER_RANGE", 4, NUM_TEST_MODULES);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16WriteSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            for (unsigned int i = 0; i < NUM_TEST_MODULES; i++) {
                retval = Pixie16ReadSglModPar("SLOW_FILTER_RANGE", &dummy, i);
                TEST_CHECK(retval == 0);
                TEST_MSG("Pixie16ReadSglModPar(%i) | %s", i, tst_msg(errmsg, MSGLEN, retval, 0));
                TEST_CHECK(dummy == 4);
                TEST_MSG("%i != %i", dummy, 4);
            }
        }
    }
}

void read_fifo_stats(void) {
    int retval;
    struct module_fifo_stats stats;

    TEST_CASE("Null Stats");
    {
        retval = PixieReadRunFifoStats(0, NULL);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieReadRunFifoStats | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Uninitialized");
    {
        retval = PixieReadRunFifoStats(0, &stats);
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieReadRunFifoStats | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Online");
    {
        init();
        boot();

        TEST_CASE("Online.Bad Module");
        {
            retval = PixieReadRunFifoStats(100, &stats);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16ReadDataFromExternalFIFO | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }

        TEST_CASE("Online.Happy Path");
        {
            retval = PixieReadRunFifoStats(0, &stats);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ReadDataFromExternalFIFO | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void reg_crate_fw(void) {
    int retval;
    TEST_CASE("Uninitialized");
    {
        retval = PixieRegisterCrateFirmware(0, 0, 0, 0, "", "");
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieRegisterCrateFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        TEST_CASE("Offline.Nulls");
        {
            TEST_CASE("Offline.Nulls.Device");
            {
                retval = PixieRegisterCrateFirmware(0, 0, 0, 0, NULL, "");
                TEST_CHECK(retval == -802);
                TEST_MSG("PixieRegisterCrateFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
            TEST_CASE("Offline.Nulls.Path");
            {
                retval = PixieRegisterCrateFirmware(0, 0, 0, 0, "", NULL);
                TEST_CHECK(retval == -802);
                TEST_MSG("PixieRegisterCrateFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
        }

        TEST_CASE("Offline.Bad Revision");
        {
            retval = PixieRegisterCrateFirmware(0, 1, 250, 14, "fippi", "fippi.bin");
            TEST_CHECK(retval == -207);
            TEST_MSG("PixieRegisterCrateFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -207));
        }

        TEST_CASE("Offline.Happy Path");
        {
            retval = PixieRegisterCrateFirmware(1, 17, 250, 14, "fippi", "fippi.bin");
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieRegisterCrateFirmware | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void reg_fw(void) {
    int retval;
    TEST_CASE("Uninitialized");
    {
        retval = PixieRegisterFirmware(0, 0, 0, 0, "", "", 0);
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        TEST_CASE("Offline.Nulls");
        {
            TEST_CASE("Offline.Nulls.Device");
            {
                retval = PixieRegisterFirmware(0, 0, 0, 0, NULL, "", 0);
                TEST_CHECK(retval == -802);
                TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
            TEST_CASE("Offline.Nulls.Path");
            {
                retval = PixieRegisterFirmware(0, 0, 0, 0, "", NULL, 0);
                TEST_CHECK(retval == -802);
                TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
        }

        TEST_CASE("Offline.Bad Revision");
        {
            retval = PixieRegisterFirmware(0, 1, 250, 14, "fippi", "fippi.bin", 0);
            TEST_CHECK(retval == -207);
            TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -207));
        }

        TEST_CASE("Offline.Mismatch Revisions");
        {
            retval = PixieRegisterFirmware(0, 17, 250, 14, "fippi", "fippi.bin", 0);
            TEST_CHECK(retval == -207);
            TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -207));
        }

        TEST_CASE("Offline.Happy Path");
        {
            retval = PixieRegisterFirmware(1, 17, 250, 14, "fippi", "fippi.bin", 3);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void run_guards(void) {
    int retval;
    init();
    boot();

    unsigned short chan = 1;
    unsigned int dum_uint;
    double dum_dbl;
    unsigned short* dum_ushort_ary = malloc(256 * sizeof(unsigned short));
    double* dum_dbl_ary = malloc(256 * sizeof(double));

    TEST_CASE("MCA");
    {
        unsigned short mod = 0;
        retval = Pixie16StartHistogramRun(mod, 1);
        TEST_ASSERT(retval == 0);
        TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));

        retval = Pixie16AcquireADCTrace(mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16AcquireADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16AcquireBaselines(mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16AcquireBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16BLcutFinder(mod, chan, &dum_uint);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16BLcutFinder | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16BootModule(" ", " ", NULL, " ", settings, " ", mod, 0x7F);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16BootModuleFirmware(settings, mod, 0x7F);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16BootModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        unsigned short dest[NUM_TEST_CHANS] = {0};
        retval = Pixie16CopyDSPParameters(0, 0, 0, dest);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16CopyDSPParameters | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16LoadDSPParametersFromFile(settings);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16LoadDSPParametersFromFile | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16LoadModuleFirmware("./");
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16LoadModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16ReadSglChanADCTrace(dum_ushort_ary, 0, 0, 0);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16ReadSglChanADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16ReadSglChanBaselines(dum_dbl_ary, dum_dbl_ary, 0, 0, 0);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16ReadSglChanBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16ReadSglChanPar("VOFFSET", &dum_dbl, mod, chan);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16ReadSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16ReadSglModPar("MOD_CSRB", &dum_uint, mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16ReadSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16SetDACs(mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16SetDACs | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16SetModuleFirmware("dsp.ldr", mod, "dsp");
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16SetModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16StartHistogramRun(mod, 1);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16StartListModeRun(mod, 0x100, 1);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16StartListModeRun | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16TauFinder(mod, dum_dbl_ary);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16TauFinder | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16WriteSglChanPar("VOFFSET", 0.3, mod, chan);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16WriteSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16WriteSglModPar("MOD_CSRB", 0, mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16WriteSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = PixieBootCrate(settings, PIXIE_BOOT_RESET_LOAD);
        TEST_CHECK(retval == -218);
        TEST_MSG("PixieBootCrate | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = PixieRegisterCrateFirmware(1, 17, 250, 14, "fippi", "fippi.bin");
        TEST_CHECK(retval == -218);
        TEST_MSG("PixieRegisterCrateFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = PixieRegisterFirmware(1, 15, 500, 14, "fippi", "fippi.bin", mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        struct module_fifo_config exp = {10, 150, 2048, 14000, 15000, 6000};
        retval = PixieSetFifoConfiguration(mod, &exp);
        TEST_CHECK(retval == -218);
        TEST_MSG("PixieSetFifoConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        struct fifo_worker_config wkr = {10, 150, 2048, 14000, 15000, 6000};
        retval = PixieSetWorkerConfiguration(mod, &wkr);
        TEST_CHECK(retval == -218);
        TEST_MSG("PixieSetWorkerConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, -218));
    }

    TEST_CASE("List-mode");
    {
        unsigned short mod = 1;
        retval = Pixie16StartListModeRun(mod, 0x100, 1);
        TEST_ASSERT(retval == 0);
        TEST_MSG("Pixie16StartListModeRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));

        retval = Pixie16AcquireADCTrace(mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16AcquireADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16AcquireBaselines(mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16AcquireBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16BLcutFinder(mod, chan, &dum_uint);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16BLcutFinder | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16BootModule(" ", " ", NULL, " ", settings, " ", mod, 0x7F);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16BootModule | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16BootModuleFirmware(settings, mod, 0x7F);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16BootModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        unsigned short dest[NUM_TEST_CHANS] = {0};
        retval = Pixie16CopyDSPParameters(0, 0, 0, dest);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16CopyDSPParameters | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16LoadDSPParametersFromFile(settings);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16LoadDSPParametersFromFile | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16LoadModuleFirmware("./");
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16LoadModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16ReadSglChanADCTrace(dum_ushort_ary, 0, 0, 0);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16ReadSglChanADCTrace | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16ReadSglChanBaselines(dum_dbl_ary, dum_dbl_ary, 0, 0, 0);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16ReadSglChanBaselines | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16ReadSglChanPar("VOFFSET", &dum_dbl, mod, chan);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16ReadSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16ReadSglModPar("MOD_CSRB", &dum_uint, mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16ReadSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16SetDACs(mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16SetDACs | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16SetModuleFirmware("dsp.ldr", mod, "dsp");
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16SetModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16StartHistogramRun(mod, 1);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16StartHistogramRun | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16StartListModeRun(mod, 0x100, 1);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16StartListModeRun | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16TauFinder(mod, dum_dbl_ary);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16TauFinder | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16WriteSglChanPar("VOFFSET", 0.3, mod, chan);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16WriteSglChanPar | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = Pixie16WriteSglModPar("MOD_CSRB", 0, mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("Pixie16WriteSglModPar | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = PixieBootCrate(settings, PIXIE_BOOT_RESET_LOAD);
        TEST_CHECK(retval == -218);
        TEST_MSG("PixieBootCrate | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = PixieRegisterCrateFirmware(1, 17, 250, 14, "fippi", "fippi.bin");
        TEST_CHECK(retval == -218);
        TEST_MSG("PixieRegisterCrateFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        retval = PixieRegisterFirmware(1, 15, 500, 14, "fippi", "fippi.bin", mod);
        TEST_CHECK(retval == -218);
        TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        struct module_fifo_config exp = {10, 150, 2048, 14000, 15000, 6000};
        retval = PixieSetFifoConfiguration(mod, &exp);
        TEST_CHECK(retval == -218);
        TEST_MSG("PixieSetFifoConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, -218));

        struct fifo_worker_config wkr = {10, 150, 2048, 14000, 15000, 6000};
        retval = PixieSetWorkerConfiguration(mod, &wkr);
        TEST_CHECK(retval == -218);
        TEST_MSG("PixieSetWorkerConfiguration | %s", tst_msg(errmsg, MSGLEN, retval, -218));
    }

    retval = Pixie16EndRun(NUM_TEST_MODULES);
    TEST_ASSERT(retval == 0);
    TEST_MSG("Pixie16EndRun | %s", tst_msg(errmsg, MSGLEN, retval, 0));

    free(dum_ushort_ary);
    free(dum_dbl_ary);
}

void save_settings(void) {
    int retval;
    TEST_CASE("Uninitialized");
    {
        retval = Pixie16SaveDSPParametersToFile("data/capi/out.set");
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16SaveDSPParametersToFile | %s", tst_msg(errmsg, MSGLEN, retval, 0));
    }

    TEST_CASE("Offline");
    {
        init();
        retval = Pixie16SaveDSPParametersToFile("data/capi/out.set");
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16SaveDSPParametersToFile | %s", tst_msg(errmsg, MSGLEN, retval, 0));
    }

    TEST_CASE("Online");
    {
        boot();
        retval = Pixie16SaveDSPParametersToFile("data/capi/out.set");
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16SaveDSPParametersToFile | %s", tst_msg(errmsg, MSGLEN, retval, 0));

        retval = Pixie16SaveDSPParametersToFile(" ");
        TEST_CHECK(retval == 0);
        TEST_MSG("Pixie16SaveDSPParametersToFile | %s", tst_msg(errmsg, MSGLEN, retval, 0));
    }
}

void set_dacs(void) {
    int retval;

    TEST_CASE("Uninitialized");
    {
        retval = Pixie16SetDACs(0);
        TEST_CHECK(retval == -101);
        TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();
        retval = Pixie16SetDACs(0);
        TEST_CHECK(retval == -204);
        TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();
        TEST_CASE("Online.Bad Mod Num");
        {
            retval = Pixie16SetDACs(100);
            TEST_CHECK(retval == -200);
            TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }
        TEST_CASE("Online.Happy Path");
        {
            retval = Pixie16SetDACs(0);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieRegisterFirmware | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void set_mod_fw(void) {
    int retval;
    TEST_CASE("Uninitialized");
    {
        retval = Pixie16SetModuleFirmware("", 2, "");
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16SetModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        TEST_CASE("Offline.Nulls");
        {
            TEST_CASE("Offline.Nulls.FwFile");
            {
                retval = Pixie16SetModuleFirmware(NULL, 2, "");
                TEST_CHECK(retval == -802);
                TEST_MSG("Pixie16SetModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
            TEST_CASE("Offline.Nulls.Device");
            {
                retval = Pixie16SetModuleFirmware("", 2, NULL);
                TEST_CHECK(retval == -802);
                TEST_MSG("Pixie16SetModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -802));
            }
        }

        TEST_CASE("Offline.Bad Slot");
        {
            TEST_CASE("Offline.Bad Slot.Too Big");
            {
                retval = Pixie16SetModuleFirmware("", 100, "");
                TEST_CHECK(retval == -200);
                TEST_MSG("Pixie16SetModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -200));
            }
            TEST_CASE("Offline.Bad Slot.Too Small");
            {
                retval = Pixie16SetModuleFirmware("", 0, "");
                TEST_CHECK(retval == -216);
                TEST_MSG("Pixie16SetModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -216));
            }
        }

        TEST_CASE("Offline.Bad Device");
        {
            retval = Pixie16SetModuleFirmware("dsp.ldr", 2, "jabberwocky");
            TEST_CHECK(retval == -207);
            TEST_MSG("Pixie16SetModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, -207));
        }

        TEST_CASE("Offline.Happy Path");
        {
            retval = Pixie16SetModuleFirmware("dsp.ldr", 2, "dsp");
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16SetModuleFirmware | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void stats(void) {
    int retval;
    double val;
    unsigned int stats_len;
    unsigned int* stats = NULL;
    unsigned short mod = 1;
    unsigned short chan = 5;

    TEST_CASE("Statistics.Uninitialized");
    {
        stats_len = Pixie16GetStatisticsSize();
        TEST_CHECK(stats_len == 2600);
        TEST_MSG("Pixie16GetStatisticsSize | %i != 2600", stats_len);

        stats = malloc(stats_len * sizeof(unsigned int));
        retval = Pixie16ReadStatisticsFromModule(stats, mod);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16ReadStatisticsFromModule | %s", tst_msg(errmsg, MSGLEN, retval, -101));

        val = Pixie16ComputeInputCountRate(stats, mod, chan);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeInputCountRate | %f != %i", val, 0);

        val = Pixie16ComputeLiveTime(stats, mod, chan);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeLiveTime | %f != %i", val, 0);

        val = Pixie16ComputeOutputCountRate(stats, mod, chan);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeOutputCountRate | %f != %i", val, 0);

        val = Pixie16ComputeRealTime(stats, mod);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeRealTime | %f != %i", val, 0);

        val = Pixie16ComputeRawInputCount(stats, mod, chan);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeRawInputCount | %f != %i", val, 0);

        val = Pixie16ComputeRawOutputCount(stats, mod, chan);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeRawOutputCount | %f != %i", val, 0);
    }

    TEST_CASE("Offline");
    {
        init();

        retval = Pixie16ReadStatisticsFromModule(stats, mod);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16ReadStatisticsFromModule | %s", tst_msg(errmsg, MSGLEN, retval, -204));

        val = Pixie16ComputeInputCountRate(stats, mod, chan);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeInputCountRate | %f != %i", val, 0);

        val = Pixie16ComputeLiveTime(stats, mod, chan);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeLiveTime | %f != %i", val, 0);

        val = Pixie16ComputeOutputCountRate(stats, mod, chan);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeOutputCountRate | %f != %i", val, 0);

        val = Pixie16ComputeRealTime(stats, mod);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeRealTime | %f != %i", val, 0);

        val = Pixie16ComputeRawInputCount(stats, mod, chan);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeRawInputCount | %f != %i", val, 0);

        val = Pixie16ComputeRawOutputCount(stats, mod, chan);
        TEST_CHECK(approx(val, -802, 0.001));
        TEST_MSG("Pixie16ComputeRawOutputCount | %f != %i", val, 0);
    }

    TEST_CASE("Online");
    {
        boot();

        TEST_CASE("Online.NullStats");
        {
            retval = Pixie16ReadStatisticsFromModule(NULL, 0);
            TEST_CHECK(retval == -802);
            TEST_MSG("Pixie16ReadStatisticsFromModule | %s", tst_msg(errmsg, MSGLEN, retval, -802));

            val = Pixie16ComputeInputCountRate(NULL, mod, chan);
            TEST_CHECK(val == -802);
            TEST_MSG("Pixie16ComputeInputCountRate | %s", tst_msg(errmsg, MSGLEN, retval, -802));

            val = Pixie16ComputeLiveTime(NULL, mod, chan);
            TEST_CHECK(val == -802);
            TEST_MSG("Pixie16ComputeLiveTime | %s", tst_msg(errmsg, MSGLEN, retval, -802));

            val = Pixie16ComputeOutputCountRate(NULL, mod, chan);
            TEST_CHECK(val == -802);
            TEST_MSG("Pixie16ComputeOutputCountRate | %s", tst_msg(errmsg, MSGLEN, retval, -802));

            val = Pixie16ComputeRealTime(NULL, mod);
            TEST_CHECK(val == -802);
            TEST_MSG("Pixie16ComputeRealTime | %s", tst_msg(errmsg, MSGLEN, retval, -802));

            val = Pixie16ComputeRawInputCount(NULL, mod, chan);
            TEST_CHECK(val == -802);
            TEST_MSG("Pixie16ComputeRawInputCount | %s", tst_msg(errmsg, MSGLEN, retval, -802));

            val = Pixie16ComputeRawOutputCount(NULL, mod, chan);
            TEST_CHECK(val == -802);
            TEST_MSG("Pixie16ComputeRawOutputCount | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }

        TEST_CASE("Online.BadChannel");
        {
            fill_uint_ary(stats, stats_len, 0xDEADBEEF);
            retval = Pixie16ReadStatisticsFromModule(stats, mod);

            val = Pixie16ComputeInputCountRate(stats, mod, 100);
            TEST_CHECK(val == -300);
            TEST_MSG("Pixie16ComputeInputCountRate | %s", tst_msg(errmsg, MSGLEN, retval, -300));

            val = Pixie16ComputeLiveTime(stats, mod, 100);
            TEST_CHECK(val == -300);
            TEST_MSG("Pixie16ComputeLiveTime | %s", tst_msg(errmsg, MSGLEN, retval, -300));

            val = Pixie16ComputeOutputCountRate(stats, mod, 100);
            TEST_CHECK(val == -300);
            TEST_MSG("Pixie16ComputeOutputCountRate | %s", tst_msg(errmsg, MSGLEN, retval, -300));

            val = Pixie16ComputeRawInputCount(stats, mod, 100);
            TEST_CHECK(val == -300);
            TEST_MSG("Pixie16ComputeRawInputCount | %s", tst_msg(errmsg, MSGLEN, retval, -300));

            val = Pixie16ComputeRawOutputCount(stats, mod, 100);
            TEST_CHECK(val == -300);
            TEST_MSG("Pixie16ComputeRawOutputCount | %s", tst_msg(errmsg, MSGLEN, retval, -300));
        }

        TEST_CASE("Online.HappyPath");
        {
            fill_uint_ary(stats, stats_len, 0xDEADBEEF);
            retval = Pixie16ReadStatisticsFromModule(stats, mod);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16ReadStatisticsFromModule | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            val = Pixie16ComputeInputCountRate(stats, mod, chan);
            TEST_CHECK(approx(val, 0, 0.001));
            TEST_MSG("Pixie16ComputeInputCountRate | %f != %i", val, 0);

            val = Pixie16ComputeLiveTime(stats, mod, chan);
            TEST_CHECK(approx(val, 0, 0.001));
            TEST_MSG("Pixie16ComputeLiveTime | %f != %i", val, 0);

            val = Pixie16ComputeOutputCountRate(stats, mod, chan);
            TEST_CHECK(approx(val, 0, 0.001));
            TEST_MSG("Pixie16ComputeOutputCountRate | %f != %i", val, 0);

            val = Pixie16ComputeRealTime(stats, mod);
            TEST_CHECK(approx(val, 0, 0.001));
            TEST_MSG("Pixie16ComputeRealTime | %f != %i", val, 0);

            val = Pixie16ComputeRawInputCount(stats, mod, chan);
            TEST_CHECK(approx(val, 0, 0.001));
            TEST_MSG("Pixie16ComputeRawInputCount | %f != %i", val, 0);

            val = Pixie16ComputeRawOutputCount(stats, mod, chan);
            TEST_CHECK(approx(val, 0, 0.001));
            TEST_MSG("Pixie16ComputeRawOutputCount | %f != %i", val, 0);
        }
    }
    free(stats);
}

void sysctl_close(void) {
    int retval;

    TEST_CASE("Close before open");
    {
        retval = PixieSysControlClose();
        TEST_CHECK(retval == 0);
        TEST_MSG("PixieSysControlClose | %s", tst_msg(errmsg, MSGLEN, retval, 0));
    }

    TEST_CASE("Happy Path");
    {
        retval = PixieSysControlOpen("module.2.sim.test*", PIXIE_SYSCTL_FORMAT_TEXT);
        TEST_CHECK(retval == 0);
        TEST_MSG("PixieSysControlOpen | %s", tst_msg(errmsg, MSGLEN, retval, 0));

        retval = PixieSysControlClose();
        TEST_CHECK(retval == 0);
        TEST_MSG("PixieSysControlClose | %s", tst_msg(errmsg, MSGLEN, retval, 0));

        TEST_CASE("Close again");
        {
            retval = PixieSysControlClose();
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlClose | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void sysctl_str(void) {
    int retval;
    char query[] = {"module.2.sim.test.str"};
    char dummy[1] = {"\0"};

    TEST_CASE("Null path");
    {
        retval = PixieSysControlGet(NULL, dummy, 0);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlGet | %s", tst_msg(errmsg, MSGLEN, retval, -802));

        retval = PixieSysControlSet(NULL, dummy);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlSet | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Null value");
    {
        retval = PixieSysControlGet(query, NULL, 0);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlGet | %s", tst_msg(errmsg, MSGLEN, retval, -802));

        retval = PixieSysControlSet(query, NULL);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlSet | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Online");
    {
        init();
        boot();

        TEST_CASE("Online.Happy Path");
        {
            char rev[3] = {"\0"};
            retval = PixieSysControlGet("module.2.revision", rev, 3);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGet | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(strcmp(rev, "15") == 0);
            TEST_MSG("%s != %s", rev, "15");

            TEST_CASE("Online.Happy Path.Int");
            {
                char val[] = {"1337"};
                retval = PixieSysControlSet("module.2.sim.test.int", val);
                TEST_CHECK(retval == 0);
                TEST_MSG("PixieSysControlSet | %s", tst_msg(errmsg, MSGLEN, retval, 0));

                char result[5] = {0};
                retval = PixieSysControlGet("module.2.sim.test.int", result, 5);
                TEST_CHECK(retval == 0);
                TEST_MSG("PixieSysControlGet | %s", tst_msg(errmsg, MSGLEN, retval, 5));
                TEST_CHECK(strcmp(result, "1337") == 0);
                TEST_MSG("%s != %s", result, "1337");
            }

            TEST_CASE("Online.Happy Path.Double");
            {
                char val[] = {"20.1337"};
                retval = PixieSysControlSet("module.2.sim.test.dbl", val);
                TEST_CHECK(retval == 0);
                TEST_MSG("PixieSysControlSet | %s", tst_msg(errmsg, MSGLEN, retval, 0));

                char result[8] = {};
                retval = PixieSysControlGet("module.2.sim.test.dbl", result, 8);
                TEST_CHECK(retval == 0);
                TEST_MSG("PixieSysControlGet | %s", tst_msg(errmsg, MSGLEN, retval, 0));
                TEST_CHECK(strcmp(result, "20.1337") == 0);
                TEST_MSG("%s != %s", result, "20.1337");
            }

            TEST_CASE("Online.Happy Path.String");
            {
                char val[] = "jabberwocky";
                retval = PixieSysControlSet("module.2.sim.test.str", val);
                TEST_CHECK(retval == 0);
                TEST_MSG("PixieSysControlSet | %s", tst_msg(errmsg, MSGLEN, retval, 0));

                char result[12] = {};
                retval = PixieSysControlGet("module.2.sim.test.str", result, 12);
                TEST_CHECK(retval == 0);
                TEST_MSG("PixieSysControlGet | %s", tst_msg(errmsg, MSGLEN, retval, 0));
                TEST_CHECK(strcmp(result, "jabberwocky") == 0);
                TEST_MSG("%s != %s", result, "jabberwocky");
            }
        }

        TEST_CASE("Online.Write to RO");
        {
            char nval[3] = {"100"};
            retval = PixieSysControlSet("module.2.max-channels", nval);
            TEST_CHECK(retval == -903);
            TEST_MSG("PixieSysControlSet | %s", tst_msg(errmsg, MSGLEN, retval, -903));

            char mchan[4] = {"\0"};
            retval = PixieSysControlGet("module.2.max-channels", mchan, 4);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGet | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(strcmp(mchan, "16") == 0);
            TEST_MSG("%s != %s", mchan, "16");
        }

        TEST_CASE("Online.Bad Len");
        {
            retval = PixieSysControlGet("module.2.revision", dummy, 0);
            TEST_CHECK(retval == -802);
            TEST_MSG("PixieSysControlGet | %s", tst_msg(errmsg, MSGLEN, retval, -802));
        }
    }
}

void sysctl_double(void) {
    int retval;
    char path[] = {"module.2.sim.test.dbl"};
    double dblval = 0.1337;

    TEST_CASE("Null path");
    {
        retval = PixieSysControlGetDouble(NULL, 0);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlGetDouble | %s", tst_msg(errmsg, MSGLEN, retval, -802));

        retval = PixieSysControlSetDouble(NULL, dblval);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlSetDouble | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Null value");
    {
        retval = PixieSysControlGetDouble(path, NULL);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlGetDouble | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Online");
    {
        init();
        boot();

        TEST_CASE("Online.Happy Path");
        {
            double val = 0.;
            retval = PixieSysControlGetDouble(path, &val);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGetDouble | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(approx(val, 0.0, 0.001));
            TEST_MSG("%f != %f", val, 0.00);

            retval = PixieSysControlSetDouble(path, dblval);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlSetDouble | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = PixieSysControlGetDouble(path, &val);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGetDouble | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(approx(val, dblval, 0.0001));
            TEST_MSG("%f != %f", val, dblval);
        }
    }
}

void sysctl_int(void) {
    int retval;
    char query[] = {"module.2.sim.test.int"};
    int num = 1337;

    TEST_CASE("Null query");
    {
        int dummy;
        retval = PixieSysControlGetInt(NULL, &dummy);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlGetInt | %s", tst_msg(errmsg, MSGLEN, retval, -802));

        retval = PixieSysControlSetInt(NULL, num);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlSetInt | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Null value");
    {
        retval = PixieSysControlGetInt(query, NULL);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlGetInt | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Online");
    {
        init();
        boot();

        TEST_CASE("Online.Happy Path");
        {
            int val = 0;
            retval = PixieSysControlGetInt(query, &val);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGetInt | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(val == 0);
            TEST_MSG("%i != %i", val, 0);

            retval = PixieSysControlSetInt(query, num);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlSetInt | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = PixieSysControlGetInt(query, &val);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGetInt | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(val == num);
            TEST_MSG("%i != %i", val, num);
        }

        TEST_CASE("Online.Write to RO");
        {
            retval = PixieSysControlSetInt("module.2.max-channels", 100);
            TEST_CHECK(retval == -903);
            TEST_MSG("PixieSysControlSetInt | %s", tst_msg(errmsg, MSGLEN, retval, -903));
        }
    }
}

void sysctl_open(void) {
    int retval;

    TEST_CASE("Bad Format");
    {
        retval = PixieSysControlOpen(NULL, 200);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlOpen | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Happy Path");
    {
        retval = PixieSysControlOpen("module.*.serial-number", PIXIE_SYSCTL_FORMAT_TEXT);
        TEST_CHECK(retval == 0);
        TEST_MSG("PixieSysControlOpen | %s", tst_msg(errmsg, MSGLEN, retval, 0));

        retval = PixieSysControlClose();
        TEST_CHECK(retval == 0);
        TEST_MSG("PixieSysControlClose | %s", tst_msg(errmsg, MSGLEN, retval, 0));

        retval = PixieSysControlOpen("module.*.serial-number", PIXIE_SYSCTL_FORMAT_JSON);
        TEST_CHECK(retval == 0);
        TEST_MSG("PixieSysControlOpen | %s", tst_msg(errmsg, MSGLEN, retval, 0));
    }

    TEST_CASE("Open Again");
    {
        retval = PixieSysControlOpen("module.*", PIXIE_SYSCTL_FORMAT_TEXT);
        TEST_CHECK(retval == -701);
        TEST_MSG("PixieSysControlOpen | %s", tst_msg(errmsg, MSGLEN, retval, -701));
    }
}

void sysctl_read(void) {
    int retval;

    TEST_CASE("Null buffer");
    {
        size_t len = 0;
        retval = PixieSysControlRead(NULL, &len);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlRead | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Null len");
    {
        char buf[] = {'\0'};
        retval = PixieSysControlRead(buf, NULL);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlRead | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Online");
    {
        init();
        boot();

        char query[] = {"module.2.serial-num"};
        char expected[] = {"module.2.serial-num = 250"};
        size_t expected_len = 25;

        TEST_CASE("Online.Big Read");
        {
            retval = PixieSysControlOpen(query, PIXIE_SYSCTL_FORMAT_TEXT);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlOpen | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            char buf[4096] = {'\0'};
            size_t len = 4096;
            retval = PixieSysControlRead(buf, &len);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlRead | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(strcmp(buf, expected) == 0);
            TEST_MSG("%s != %s", buf, expected);

            retval = PixieSysControlClose();
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlClose | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }

        TEST_CASE("Online.Zero Len");
        {
            retval = PixieSysControlOpen(query, PIXIE_SYSCTL_FORMAT_TEXT);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlOpen | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            char buf[4096] = {0};
            size_t len = 0;
            retval = PixieSysControlRead(buf, &len);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlRead | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(strcmp(buf, "") == 0);
            TEST_MSG("%s != %s", buf, "");

            retval = PixieSysControlClose();
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlClose | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }

        TEST_CASE("Online.Partial Reads");
        {
            retval = PixieSysControlOpen(query, PIXIE_SYSCTL_FORMAT_TEXT);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlOpen | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            char buf[4] = {0};
            size_t len = 3;
            retval = PixieSysControlRead(buf, &len);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlRead | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(strcmp(buf, "mod") == 0);
            TEST_MSG("%s != %s", buf, "mod");

            retval = PixieSysControlRead(buf, &len);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlRead | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(strcmp(buf, "ule") == 0);
            TEST_MSG("%s != %s", buf, "ule");

            retval = PixieSysControlClose();
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlClose | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }

        TEST_CASE("Online.Full Read");
        {
            retval = PixieSysControlOpen(query, PIXIE_SYSCTL_FORMAT_TEXT);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlOpen | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            size_t len = 0;
            retval = PixieSysControlSize(&len);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlSize | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(len == expected_len);
            TEST_MSG("%zu != %zu", len, expected_len);

            char* buffer = calloc(len + 1, sizeof(char));
            retval = PixieSysControlRead(buffer, &len);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlRead | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            TEST_CHECK(strcmp(buffer, expected) == 0);
            TEST_MSG("%s != %s", buffer, expected);

            retval = PixieSysControlClose();
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlClose | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            free(buffer);
        }
    }
}

void sysctl_set_file_values(void) {
    int retval;

    TEST_CASE("Null Filename");
    {
        retval = PixieSysControlSetFileValues(NULL, PIXIE_SYSCTL_FORMAT_JSON);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlSetInt | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Bad Format Arg");
    {
        retval = PixieSysControlSetFileValues("", 100);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlSetInt | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Missing file");
    {
        retval = PixieSysControlSetFileValues("jabber.wok", PIXIE_SYSCTL_FORMAT_JSON);
        TEST_CHECK(retval == -700);
        TEST_MSG("PixieSysControlSetInt | %s", tst_msg(errmsg, MSGLEN, retval, -700));
    }

    TEST_CASE("JSON syntax error");
    {
        retval =
            PixieSysControlSetFileValues("data/capi/sysctl-in.badfmt", PIXIE_SYSCTL_FORMAT_JSON);
        TEST_CHECK(retval == -602);
        TEST_MSG("PixieSysControlSetInt | %s", tst_msg(errmsg, MSGLEN, retval, -602));
    }

    TEST_CASE("Online");
    {
        init();
        boot();

        TEST_CASE("Online.JSON");
        {
            retval =
                PixieSysControlSetFileValues("data/capi/sysctl-in.json", PIXIE_SYSCTL_FORMAT_JSON);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlSetFileValues | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }

        TEST_CASE("Online.Text");
        {
            retval =
                PixieSysControlSetFileValues("data/capi/sysctl-in.txt", PIXIE_SYSCTL_FORMAT_TEXT);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlSetFileValues | %s", tst_msg(errmsg, MSGLEN, retval, 0));
        }
    }
}

void sysctl_set_values(void) {
    int retval;

    TEST_CASE("Null Values");
    {
        retval = PixieSysControlSetValues(NULL, PIXIE_SYSCTL_FORMAT_TEXT);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlSetValues | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Bad Format");
    {
        retval = PixieSysControlSetValues("", 100);
        TEST_CHECK(retval == -802);
        TEST_MSG("PixieSysControlSetValues | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Online");
    {
        init();
        boot();

        double dblval = 0.;
        int intval = 0;
        TEST_CASE("Online.Text.Unix");
        {
            char textbuf[] = {"module.2.sim.test.dbl=0.1337\nmodule.2.sim.test.int=1337"};
            retval = PixieSysControlSetValues(textbuf, PIXIE_SYSCTL_FORMAT_TEXT);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlSetValues | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = PixieSysControlGetDouble("module.2.sim.test.dbl", &dblval);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGetDouble | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(approx(dblval, 0.1337, 0.0001));
            TEST_MSG("%f != %f", dblval, 0.1337);

            retval = PixieSysControlGetInt("module.2.sim.test.int", &intval);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGetInt | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(intval == 1337);
            TEST_MSG("%i != %i", intval, 1337);
        }

        TEST_CASE("Online.Text.Windows");
        {
            char textbuf[] = {"module.2.sim.test.dbl=0.9317\r\nmodule.2.sim.test.int=9317"};
            retval = PixieSysControlSetValues(textbuf, PIXIE_SYSCTL_FORMAT_TEXT);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlSetValues | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = PixieSysControlGetDouble("module.2.sim.test.dbl", &dblval);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGetDouble | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(approx(dblval, 0.9317, 0.0001));
            TEST_MSG("%f != %f", dblval, 0.9317);

            retval = PixieSysControlGetInt("module.2.sim.test.int", &intval);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGetInt | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(intval == 9317);
            TEST_MSG("%i != %i", intval, 9317);
        }

        TEST_CASE("Online.JSON");
        {
            char jsonbuf[] = {
                "{\"module\":{\"2\":{\"sim\":{\"test\":{\"dbl\":\"0.2122\",\"int\":\"2122\"}}}}}"};
            retval = PixieSysControlSetValues(jsonbuf, PIXIE_SYSCTL_FORMAT_JSON);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlSetValues | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            retval = PixieSysControlGetDouble("module.2.sim.test.dbl", &dblval);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGetDouble | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(approx(dblval, 0.2122, 0.0001));
            TEST_MSG("%f != %f", dblval, 0.2122);

            retval = PixieSysControlGetInt("module.2.sim.test.int", &intval);
            TEST_CHECK(retval == 0);
            TEST_MSG("PixieSysControlGetInt | %s", tst_msg(errmsg, MSGLEN, retval, 0));
            TEST_CHECK(intval == 2122);
            TEST_MSG("%i != %i", intval, 2122);
        }
    }
}

void tau_finder(void) {
    int retval;
    double dummy[] = {0.};

    TEST_CASE("Null Array");
    {
        retval = Pixie16TauFinder(1, NULL);
        TEST_CHECK(retval == -802);
        TEST_MSG("Pixie16TauFinder | %s", tst_msg(errmsg, MSGLEN, retval, -802));
    }

    TEST_CASE("Uninitialized");
    {
        retval = Pixie16TauFinder(0, dummy);
        TEST_CHECK(retval == -101);
        TEST_MSG("Pixie16TauFinder | %s", tst_msg(errmsg, MSGLEN, retval, -101));
    }

    TEST_CASE("Offline");
    {
        init();

        retval = Pixie16TauFinder(0, dummy);
        TEST_CHECK(retval == -204);
        TEST_MSG("Pixie16TauFinder | %s", tst_msg(errmsg, MSGLEN, retval, -204));
    }

    TEST_CASE("Online");
    {
        boot();

        TEST_CASE("Online.Bad Module");
        {
            retval = Pixie16TauFinder(100, dummy);
            TEST_CHECK(retval == -200);
            TEST_MSG("Pixie16TauFinder | %s", tst_msg(errmsg, MSGLEN, retval, -200));
        }

        TEST_CASE("Online.Happy Path.Revision F");
        {
            double* taus = malloc(16 * sizeof(double));
            fill_dbl_ary(taus, 16, 1337.);
            retval = Pixie16TauFinder(1, taus);
            TEST_CHECK(retval == 0);
            TEST_MSG("Pixie16TauFinder | %s", tst_msg(errmsg, MSGLEN, retval, 0));

            double* check = malloc(16 * sizeof(double));
            fill_dbl_ary(check, 16, 0.);
            TEST_CHECK(compare_dbl_ary(taus, check, 16, 0.001));
            TEST_MSG("taus do not match");
            free(taus);
            free(check);
        }

        TEST_CASE("Online.Happy Path.Revision H");
        {
            retval = Pixie16TauFinder(3, dummy);
            TEST_CHECK(retval == -803);
            TEST_MSG("Pixie16TauFinder | %s", tst_msg(errmsg, MSGLEN, retval, -803));
        }
    }
}

TEST_LIST = {
    {"ADC", adc},
    {"Adjust Offsets", adjust_offsets},
    {"Baselines", baseline},
    {"BL Cut Finder", blcut},
    {"Boot Crate", boot_crate},
    {"Boot Module Firmware", boot_mod_fw},
    {"Boot Module", boot_module},
    {"Channel Parameters", chan_par},
    {"Cleanup", cleanup},
    {"Cleanup All", cleanup_all},
    {"Copy Parameters", copy_par},
    {"Fifo Config", fifo},
    {"IEEE FLoating", ieee_float},
    {"Init.Order", init_order},
    {"Init.System", init_system},
    {"Install Paths", install_path},
    {"List-Mode", list_mode},
    {"Load Module Firmware", load_mod_fw},
    {"Load Settings", load_settings},
    {"MCA", mca},
    {"Read Module Info", mod_info},
    {"Module Parameters", mod_par},
    {"Read FIFO Stats", read_fifo_stats},
    {"Register Crate Firmware", reg_crate_fw},
    {"Register Firmware", reg_fw},
    {"Run Guards", run_guards},
    {"Save Settings", save_settings},
    {"Set DACS", set_dacs},
    {"Set Module Firmware", set_mod_fw},
    {"Statistics", stats},
    {"Sysctl.Close", sysctl_close},
    {"Sysctl.String", sysctl_str},
    {"Sysctl.Double", sysctl_double},
    {"Sysctl.Int", sysctl_int},
    {"Sysctl.Open", sysctl_open},
    {"Sysctl.Set Values.File", sysctl_set_file_values},
    {"Sysctl.Set Values", sysctl_set_values},
    {"Sysctl.Read", sysctl_read},
    {"Tau Finder", tau_finder},
    {NULL, NULL} /* zeroed record marking the end of the list */
};
