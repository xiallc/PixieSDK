# -*- Makefile -*-

# Building PIXIE-16 app and sys libraries
APPLIBNAME = pixieappdll
SYSLIBNAME = pixie16sysdll
TESTEXE    = p16test.exe p16lmr.exe p16REVA.exe p16REVC_Control.exe 

# Architecture-specific overrides here
# ARCH = win32, etc...
# include Makehead.$(ARCH), if we borrow Bill Lavender's syntax
# It is instructive to notice here that setting ARCH in the Makefile
# or passing it on the command line are not equivalent, since gmake sets
# MAKEFLAGS to the value of ARCH if command line arguments are present.
# This condition only affects WIN32 platform owing to the use of NMAKE in
# the build process on Microsoft Windows.

ARCH ?= WIN32
include Makearch.$(ARCH)

.SUFFIXES: .asm .obj .doj

ASM    = easm21k.exe
DSPOBJ = doj

# PIXIE-16 general test executable file to create
TESTGENOBJS = p16swtest/p16test.$(OBJ) 
# PIXIE-16 list mode run test executable file to create
TESTLMROBJS = p16swtest/p16lmr.$(OBJ)
# PIXIE-16 Coincidence pattern REVA test executable file to create
TESTREVAOBJS = p16swtest/p16REVA.$(OBJ)
# PIXIE-16 Control Task REVC test executable file to create
#TESTREVAOBJS = p16swtest/p16REVC_Control.$(OBJ)
# PIXIE-16 MCA hardware test executables to create
TESTMCAHWOBJS = p16hwtest/mca_tests.$(OBJ)
# PIXIE-16 analog hardware test executables to create
TESTANALOGHWOBJS = p16hwtest/subsystem_tests.$(OBJ)
# PIXIE-16 DSP object files to create
DSPOBJS = dsp/160HostLDR/160_host.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/dbl_sprt.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/double_add.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/double_mult.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/exp.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/expf.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/logsf.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/sqrtf.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/errno.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/float_divide.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/float_double.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/flt_sprt.$(DSPOBJ) \
          dsp/ADSP-Math-Lib-Files/mth_sprt.$(DSPOBJ) \
          dsp/Baseline/CollectADC.$(DSPOBJ) \
          dsp/Baseline/CollectBaselines.$(DSPOBJ) \
          dsp/Baseline/GetBaseWait.$(DSPOBJ) \
          dsp/Baseline/GetBaselines.$(DSPOBJ) \
          dsp/Baseline/GetBLcut.$(DSPOBJ) \
          dsp/Baseline/ComputeGammaCoeff.$(DSPOBJ) \
          dsp/CompCoefs/CompCoefs.$(DSPOBJ) \
          dsp/ControlTasks/ControlTasks.$(DSPOBJ) \
          dsp/Globals/Externs.$(DSPOBJ) \
          dsp/Globals/Globals.$(DSPOBJ) \
          dsp/Hardware/AdjustOffsets.$(DSPOBJ) \
          dsp/Hardware/FiPPIDecimatorWait.$(DSPOBJ) \
          dsp/Hardware/ProgramFippi.$(DSPOBJ) \
          dsp/Hardware/RampOffsetDACs.$(DSPOBJ) \
          dsp/Hardware/SetDACs.$(DSPOBJ) \
          dsp/Interrupt/IntVect.$(DSPOBJ) \
          dsp/Interrupt/ListRunEvent.$(DSPOBJ) \
          dsp/Interrupt/MCARunEvent.$(DSPOBJ) \
          dsp/Interrupt/Periodic.$(DSPOBJ) \
          dsp/Interrupt/Synchronize.$(DSPOBJ) \
          dsp/MCAProc/ProcessMCAEvent.$(DSPOBJ) \
          dsp/Multipar/ProcessMultiparEvent.$(DSPOBJ) \
          dsp/PowerUp/InitDataMemory.$(DSPOBJ) \
          dsp/PowerUp/PowerUpInit.$(DSPOBJ) \
          dsp/RunControl/Main.$(DSPOBJ) \
          dsp/RunControl/ReadFiPPIStat.$(DSPOBJ) \
          dsp/RunControl/RunFinish.$(DSPOBJ) \
          dsp/RunControl/RunStart.$(DSPOBJ) \
          dsp/RunControl/Pingpong.$(DSPOBJ) \
          dsp/RunControl/ZDTEventProc.$(DSPOBJ) \
          dsp/RunControl/ZDTRun.$(DSPOBJ) \
          dsp/RunControl/iThembaEventProc.$(DSPOBJ) \
          dsp/RunControl/iThembaRun.$(DSPOBJ) \
          dsp/RunControl/RevDGeneralEventProc.$(DSPOBJ) \
          dsp/RunControl/RevDGeneralRun.$(DSPOBJ) \
          dsp/RunControl/RevDMCAEventProc.$(DSPOBJ) \
          dsp/RunControl/RevDMCARun.$(DSPOBJ) \
          dsp/RunControl/RevDORNLPSDEventProc.$(DSPOBJ) \
          dsp/RunControl/RevDORNLPSDRun.$(DSPOBJ) \
          dsp/RunControl/EventProcNoSorting.$(DSPOBJ) \
          dsp/RunControl/EventProcWithSorting.$(DSPOBJ) \
          dsp/TauFinder/SumsBuilder.$(DSPOBJ) \
          dsp/TauFinder/SumsResetter.$(DSPOBJ) \
          dsp/TauFinder/TauComputer.$(DSPOBJ) \
          dsp/TauFinder/TauFinder.$(DSPOBJ) \

HOSTDSPOBJS = dsp/160HostLDR/160_host.$(DSPOBJ)

# PIXIE-16 app library object files to create
APPLIBOBJS = software/app/pixie16app.$(OBJ) \
             software/app/utilities.$(OBJ)

# PIXIE-16 sys library object files to create
SYSLIBOBJS = software/sys/pixie16sys.$(OBJ)    \
             software/sys/communication.$(OBJ) \
             software/sys/i2cm24c64.$(OBJ)     \
             software/sys/tools.$(OBJ)

DYNAPPLIB = $(PREFIX_LIB)$(APPLIBNAME).$(DYNLIB)
DYNSYSLIB = $(PREFIX_LIB)$(SYSLIBNAME).$(DYNLIB)
STATSYSLIB = $(PREFIX_LIB)$(SYSLIBNAME).$(STATLIB)
STATAPPLIB = $(PREFIX_LIB)$(APPLIBNAME).$(STATLIB)

# Targets
all: testexe
.PHONY: all

.asm.$(DSPOBJ): 
	 $(ASM) -I dsp -I dsp/Globals $< -proc ADSP-21160 -g -o $@ -MM

.c.$(OBJ): 
	$(CC) $(CFLAGS) $(INCFLAGS) $(CFLAG_COMPILE) $< $(CFLAG_OUT)$@

.cc.$(OBJ): 
	$(CC) $(CFLAGS) $(INCFLAGS) $(CFLAG_COMPILE) $< $(CFLAG_OUT)$@

lib: $(DYNSYSLIB) $(DYNAPPLIB)
.PHONY: lib

$(DYNSYSLIB): $(SYSLIBOBJS)
	$(LINK) $(LINK_PRE_FLAGS) $(DYN_LIB_LINK_FLAGS) \
	        $(LINK_FLAG_OUT)$(DYNSYSLIB) \
		$(SYSLIBOBJS) \
		$(PLX_LIB) $(EXTRA_LIBS)

$(DYNAPPLIB): $(APPLIBOBJS)
	$(LINK) $(LINK_PRE_FLAGS) $(DYN_LIB_LINK_FLAGS) \
	        $(LINK_FLAG_OUT)$(DYNAPPLIB) \
	        $(APPLIBOBJS) \
		$(STATSYSLIB)

dsp: Pixie16DSP.ldr


Pixie16DSP.ldr: Pixie16DSP.dxe 160_host.dxe
	elfloader.exe Pixie16DSP.dxe \
		     -proc ADSP-21160 \
	             -bHOST -fBINARY \
	             -l 160_host.dxe \
		     -o Pixie16DSP.ldr \
		     -t32765 -MM
	xmlmap2html   Pixie16DSP.map.xml
	./html2VarLst Pixie16DSP.map.html

Pixie16DSP.dxe: $(DSPOBJS)
	cc21k.exe $(DSPOBJS) \
	          -proc ADSP-21160 \
	          -T dsp/ADSP-21160.ldf \
		  -o Pixie16DSP.dxe \
		  -map Pixie16DSP.map.xml \
		  -flags-link -MM

160_host.dxe: $(HOSTDSPOBJS)
	cc21k.exe $(HOSTDSPOBJS) \
		  -proc ADSP-21160 \
		  -T dsp/160HostLDR/160_ldr.ldf \
		  -o 160_host.dxe \
		  -flags-link -MM

testobjs: $(TESTEXEOBJS)
.PHONY: testobjs

testexe: $(TESTGENOBJS) $(TESTMCAHWOBJS) $(TESTANALOGHWOBJS) $(TESTLMROBJS) $(TESTREVAOBJS) $(DYNSYSLIB) $(DYNAPPLIB) dsp
	$(LINK) $(LINK_PRE_FLAGS) $(TESTGENOBJS) $(STATSYSLIB) $(STATAPPLIB)
	$(LINK) $(LINK_PRE_FLAGS) $(TESTLMROBJS) $(STATSYSLIB) $(STATAPPLIB)
	$(LINK) $(LINK_PRE_FLAGS) $(TESTREVAOBJS) $(STATSYSLIB) $(STATAPPLIB)
	$(LINK) $(LINK_PRE_FLAGS) $(TESTMCAHWOBJS) $(STATSYSLIB) $(STATAPPLIB)
	$(LINK) $(LINK_PRE_FLAGS) $(TESTANALOGHWOBJS) $(STATSYSLIB) $(STATAPPLIB)
.PHONY: testexe

# Shortcut for the continuous integration build server.
ci: lib testobjs
.PHONY: ci

cleanall: clean
	-rm $(APPLIBOBJS) $(SYSLIBOBJS) $(EXTRA_FILES) *.$(STATLIB) \
	    $(APPLIBNAME).$(DYNLIB) $(SYSLIBNAME).$(DYNLIB) \
	    $(TESTEXE) $(TESTEXEOBJS) *.txt \
	    $(DSPOBJS) $(HOSTDSPOBJS) *.dxe *.xml *.ldr *.lst *.var *.html *.combvar
.PHONY: cleanall

clean:
	-rm $(TESTEXE) $(TESTGENOBJS) $(TESTLMROBJS) $(TESTREVAOBJS) *.txt *.bin

.PHONY: clean

