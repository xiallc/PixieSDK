/*----------------------------------------------------------------------
* Copyright (c) 2005 - 2020, XIA LLC
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification, are permitted provided
* that the following conditions are met:
*
*   * Redistributions of source code must retain the above
*     copyright notice, this list of conditions and the
*     following disclaimer.
*   * Redistributions in binary form must reproduce the
*     above copyright notice, this list of conditions and the
*     following disclaimer in the documentation and/or other
*     materials provided with the distribution.
*   * Neither the name of XIA LLC nor the names of its
*     contributors may be used to endorse or promote
*     products derived from this software without
*     specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
* TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*----------------------------------------------------------------------*/
/******************************************************************************
*
* File Name:
*
*		Pixie16Boot.c
*
* Description:
*
*		This file contains the code necessary to boot a crate of Pixie modules.
******************************************************************************/
#include "def21160.h"
#include "pixie16app_export.h"
#include "pixie16sys_export.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <sstream>
#include <string>

/////////////timing stuff//////////////

#include <sys/time.h>

//////////////////////////////////////

using namespace std;

int main(int argc, char* argv[]) {
    unsigned short NumModules;
    unsigned short* PXISlotMap;
    char ComFPGAConfigFile[80];
    char SPFPGAConfigFile[80];
    char TrigFPGAConfigFile[80];
    char DSPCodeFile[80];
    char DSPParFile[80];
    char DSPVarFile[80];
    char ErrMSG[256];
    int retval = 0;


    ////////read cfg file (cfgPixie16.txt)///////////////////////////////////
    const char config[20] = "cfgPixie16.txt";

    ifstream input;
    char* temp = new char[80];
    input.open(config, ios::in);

    if (input.fail()) {
        cout << "can't open the config file ! " << config << endl << flush;
        return false;
    }

    input >> NumModules;
    cout << "\n\n" << NumModules << " modules, in slots:";
    input.getline(temp, 80);
    PXISlotMap = new unsigned short[NumModules + 1];
    for (int i = 0; i < NumModules; i++) {
        input >> PXISlotMap[i];
        input.getline(temp, 80);
        cout << PXISlotMap[i] << " ";
    }

    //==== This code is necessary if modules are installed in two crates ====//
    //input >> PXISlotMap[NumModules];
    //input.getline (temp, 80);
    //cout << PXISlotMap[NumModules] << " ";

    cout << endl << "Firmware files: \n";
    input >> ComFPGAConfigFile;
    input.getline(temp, 80);
    cout << "ComFPGAConfigFile:  " << ComFPGAConfigFile << endl;
    input >> SPFPGAConfigFile;
    input.getline(temp, 80);
    cout << "SPFPGAConfigFile:   " << SPFPGAConfigFile << endl;
    input >> TrigFPGAConfigFile;
    input.getline(temp, 80);
    cout << "TrigFPGAConfigFile: " << TrigFPGAConfigFile << endl;
    input >> DSPCodeFile;
    input.getline(temp, 80);
    cout << "DSPCodeFile:        " << DSPCodeFile << endl;
    input >> DSPParFile;
    input.getline(temp, 80);
    cout << "DSPParFile:         " << DSPParFile << endl;
    input >> DSPVarFile;
    input.getline(temp, 80);
    cout << "DSPVarFile:         " << DSPVarFile << endl;
    input.close();
    input.clear();

    ////////////////////////////////////////////////////////////////////
    cout << "-----------------------------------------\n";
    cout << "Booting....\n";

    retval = Pixie16InitSystem(NumModules, PXISlotMap, 0);
    if (retval < 0) {
        sprintf(ErrMSG, "*ERROR* Pixie16InitSystem failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return -1;
    } else {
        cout << "Init OK " << retval << endl;
    }

    retval = Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile, TrigFPGAConfigFile, DSPCodeFile, DSPParFile,
                               DSPVarFile, NumModules, 0x7F);
    if (retval < 0) {
        sprintf(ErrMSG, "*ERROR* Pixie16BootModule failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return -2;
    }

    sprintf(ErrMSG, "Pixie16BootModule succeeded, retval = %d", retval);
    Pixie_Print_MSG(ErrMSG);
    cout << "Boot OK " << retval << endl;

    return 1;
}
