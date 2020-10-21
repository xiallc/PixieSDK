# -*- Makefile -*-
#
# Building PIXIE-16 app and sys libraries
APPLIBNAME = pixieappdll
SYSLIBNAME = pixie16sysdll

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

.SUFFIXES: .obj .exe

# PIXIE-16 app library object files
APPLIBOBJS = app/pixie16app.$(OBJ) \
             app/utilities.$(OBJ)

# PIXIE-16 sys library object files
SYSLIBOBJS = sys/pixie16sys.$(OBJ)    \
             sys/communication.$(OBJ) \
             sys/i2cm24c64.$(OBJ)     \
             sys/tools.$(OBJ)

# PIXIE-16 test xop object files
TESTEXE = testing/boot.exe \
          testing/subsystem_tests.exe \
          testing/mca_tests.exe \
          testing/eeprom_prog.exe \
          testing/Pixie16Test_RevDGeneral.exe \
          testing/check_eventheader.exe \
		  testing/AcqADCTrace.exe \
		  testing/Acq40000ADCTraces_Mod0Ch0Ch1.exe \
		  testing/DSPWrHostRd_ExtFifoTest.exe

DYNAPPLIB = $(PREFIX_LIB)$(APPLIBNAME).$(DYNLIB)
DYNSYSLIB = $(PREFIX_LIB)$(SYSLIBNAME).$(DYNLIB)
STATSYSLIB = $(PREFIX_LIB)$(SYSLIBNAME).$(STATLIB)
STATAPPLIB = $(PREFIX_LIB)$(APPLIBNAME).$(STATLIB)

# Targets
all: lib
.PHONY: all

.c.$(OBJ): 
	$(CC) $(CFLAGS) $(INCFLAGS) $(CFLAG_COMPILE) $< $(CFLAG_OUT)$@

.cc.$(OBJ): 
	$(CC) $(CFLAGS) $(INCFLAGS) $(CFLAG_COMPILE) $< $(CFLAG_OUT)$@

.$(OBJ).exe: 
	$(LINK) $(EXE_LINK_FLAGS) $< $(STATAPPLIB) $(STATSYSLIB) $(PLX_LIB) $(EXTRA_LIBS) $(LINK_FLAG_OUT)$@


lib: $(DYNSYSLIB) $(DYNAPPLIB)
.PHONY: lib

statlib: $(STATSYSLIB) $(STATAPPLIB)
.PHONY: statlib

$(STATSYSLIB): $(SYSLIBOBJS)
	$(AR) $(LINK_FLAG_OUT)$(STATSYSLIB) $(SYSLIBOBJS)

$(STATAPPLIB): $(APPLIBOBJS)
	$(AR) $(LINK_FLAG_OUT)$(STATAPPLIB) $(APPLIBOBJS)

$(DYNSYSLIB): $(SYSLIBOBJS)
	$(LINK) $(LINK_PRE_FLAGS) $(DYN_LIB_LINK_FLAGS) \
	        $(LINK_FLAG_OUT)$(DYNSYSLIB) \
		$(SYSLIBOBJS) \
		$(PLX_LIB) $(EXTRA_LIBS)

$(DYNAPPLIB): $(APPLIBOBJS)
	$(LINK) $(LINK_PRE_FLAGS) $(DYN_LIB_LINK_FLAGS) \
	        $(LINK_FLAG_OUT)$(DYNAPPLIB) \
	        $(APPLIBOBJS) \
		$(APP_EXTRA_LIBS)

tests: statlib $(TESTEXE)
.PHONY: tests

clean:
	-rm $(APPLIBOBJS) $(SYSLIBOBJS) $(EXTRA_FILES) *.$(STATLIB) *.$(DYNLIB) *.manifest
	-rm testing/*.$(OBJ) testing/*.manifest 
	-rm testing/*.lib testing/*.exp
.PHONY: clean

cleandat:
	-rm testing/*.dat testing/*.bin testing/*.txt testing/*.set
.PHONY: cleandat
