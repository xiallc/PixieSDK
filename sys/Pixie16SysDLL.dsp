# Microsoft Developer Studio Project File - Name="Pixie16SysDLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Pixie16SysDLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Pixie16SysDLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pixie16SysDLL.mak" CFG="Pixie16SysDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pixie16SysDLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Pixie16SysDLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Pixie16SysDLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PIXIE16SYSDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /Od /I "inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PIXIE16SYSDLL_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "Pixie16SysDLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PIXIE16SYSDLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "PIXIE16SYSDLL_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Pixie16SysDLL - Win32 Release"
# Name "Pixie16SysDLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\communication.c
# End Source File
# Begin Source File

SOURCE=.\i2cm24c64.c
# End Source File
# Begin Source File

SOURCE=.\pixie16sys.c
# End Source File
# Begin Source File

SOURCE=.\tools.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "inc"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=.\inc\def21160.h
# End Source File
# Begin Source File

SOURCE=.\inc\PciRegs.h
# End Source File
# Begin Source File

SOURCE=.\inc\PciTypes.h
# End Source File
# Begin Source File

SOURCE=.\inc\PexApi.h
# End Source File
# Begin Source File

SOURCE=.\inc\Plx.h
# End Source File
# Begin Source File

SOURCE=.\inc\Plx_sysdep.h
# End Source File
# Begin Source File

SOURCE=.\inc\PlxApi.h
# End Source File
# Begin Source File

SOURCE=.\inc\PlxDefCk.h
# End Source File
# Begin Source File

SOURCE=.\inc\PlxIoctl.h
# End Source File
# Begin Source File

SOURCE=.\inc\PlxStat.h
# End Source File
# Begin Source File

SOURCE=.\inc\PlxTypes.h
# End Source File
# Begin Source File

SOURCE=.\inc\Reg9054.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\pixie16sys_common.h
# End Source File
# Begin Source File

SOURCE=.\pixie16sys_defs.h
# End Source File
# Begin Source File

SOURCE=.\pixie16sys_export.h
# End Source File
# Begin Source File

SOURCE=.\pixie16sys_globals.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "lib"

# PROP Default_Filter ".lib"
# Begin Source File

SOURCE=.\lib\PlxApi.lib
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\..\..\..\Program Files\Microsoft Visual Studio\VC98\Lib\WINMM.LIB"
# End Source File
# End Group
# End Target
# End Project
