
#include "pixie16app_export.h"
#include "pixie16app_globals.h"
#include "pixie16sys_export.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <math.h>
#include <signal.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <windows.h>

using namespace std;

typedef union {
    unsigned short W;
    struct {
        unsigned char LowByte;
        unsigned char HighByte;
    };
} ShortWord;

int main(int argc, char* argv[]) {
    FILE* eeprom_file_id;
    char ComFPGAConfigFile[80];
    char SPFPGAConfigFile[80];
    char DSPCodeFile[80];
    char DSPParFile[80];
    char DSPVarFile[80];
    char EEPROMFile[80];
    char DummyFile[80];
    char ErrMSG[256];
    char Rev = 0;
    const char config[100] = "Configuration/.cfgPixie";
    unsigned short SerialNumber = 0;
    unsigned short NumModules = 0;
    unsigned short ModNum = 0;
    unsigned short* PXISlotMap;
    unsigned short ByteCounter = 0;
    unsigned short ByteValues[8192];
    unsigned short Bytes = 0;
    int retval = 0;
    ShortWord SW;
    char ADCbits;
    unsigned short ADCrate;
    unsigned short count;

    if (argc < 6) {
        printf("Insufficient number of arguments used \n");
        printf("Usage: eeprom_prog  <module>  <S/N>  <Revision>  <ADC_bits>  <ADC_rate>\n");
        return (-5);
    } else {
        ModNum = (unsigned short) atoi(argv[1]);
        SerialNumber = (unsigned short) atoi(argv[2]);
        Rev = (char) atoi(argv[3]);
        ADCbits = (char) atoi(argv[4]);
        ADCrate = (unsigned short) atoi(argv[5]);
    }

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

    //input >> TrigFPGAConfigFile;
    //input.getline (temp, 80);
    //cout << "TrigFPGAConfigFile: " << TrigFPGAConfigFile << endl;

    input >> DSPCodeFile;
    input.getline(temp, 80);
    cout << "DSPCodeFile:        " << DSPCodeFile << endl;

    input >> DSPParFile;
    input.getline(temp, 80);
    cout << "DSPParFile:         " << DSPParFile << endl;

    input >> DummyFile;
    input.getline(temp, 80);
    cout << "DummyFile:         " << DummyFile << endl;

    input >> EEPROMFile;
    input.getline(temp, 80);
    cout << "EEPROMFile: " << EEPROMFile << endl;

    input >> DSPVarFile;
    input.getline(temp, 80);
    cout << "DSPVarFile:         " << DSPVarFile << endl;

    input.close();
    input.clear();

    ////////////////////////////////////////////////////////////////////
    cout << "-----------------------------------------\n";
    //cout<<"Booting....\n";
    cout << "Initializing....\n";

    retval = Pixie16InitSystem(NumModules, PXISlotMap, 0);
    if (retval < 0) {
        printf("*ERROR* Pixie16InitSystem failed, retval = %d", retval);
        sprintf(ErrMSG, "*ERROR* Pixie16InitSystem failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return (-1);
    } else {
        cout << "Init OK " << retval << endl;
    }

    retval = Pixie16BootModule(ComFPGAConfigFile, SPFPGAConfigFile, "", DSPCodeFile, DSPParFile, DSPVarFile, NumModules,
                               0x0);
    if (retval < 0) {
        printf("*ERROR* Pixie16BootModule failed, retval = %d", retval);
        sprintf(ErrMSG, "*ERROR* Pixie16BootModule failed, retval = %d", retval);
        Pixie_Print_MSG(ErrMSG);
        return (-2);
    } else {
        //cout<<"Boot OK "<<retval<<endl;
    }

    //==== Program EEPROM ====//
    cout << "Programming EEPROM " << endl;

    if ((eeprom_file_id = fopen(EEPROMFile, "rb")) == NULL) {
        printf("*ERROR* Cannot open EEPROM file");
        sprintf(ErrMSG, "*ERROR* Cannot open EEPROM file");
        Pixie_Print_MSG(ErrMSG);
        return (-3);
    }

    // Write serial number, revision

    if (SerialNumber > 255)
        ByteCounter = 3;
    else
        ByteCounter = 2;
    while (!feof(eeprom_file_id)) fscanf(eeprom_file_id, "%d", &ByteValues[ByteCounter++]);
    fclose(eeprom_file_id);

    if (SerialNumber > 255) {
        SW.W = SerialNumber;
        ByteValues[0] = SW.LowByte;
        ByteValues[1] = SW.HighByte;
        ByteValues[2] = Rev;
    } else {
        ByteValues[0] = SerialNumber;
        ByteValues[1] = Rev;
    }

    // Write gain and termination

    Bytes = 0;
    while (Bytes < ByteCounter) {
        if (I2CM24C64_Write_One_Byte(ModNum, Bytes, (char*) &ByteValues[Bytes]) != 0) {
            printf("*ERROR* Cannot write EEPROM byte %d", Bytes);
            sprintf(ErrMSG, "*ERROR* Cannot write EEPROM byte %d", Bytes);
            Pixie_Print_MSG(ErrMSG);
            fclose(eeprom_file_id);
            return (-4);
        }
        Sleep(50);
        Bytes++;
    }

    // Write ADC bits

    ByteValues[Bytes] = ADCbits;

    if (I2CM24C64_Write_One_Byte(ModNum, Bytes, (char*) &ByteValues[Bytes]) != 0) {
        printf("*ERROR* Cannot write EEPROM byte %d", Bytes);
        sprintf(ErrMSG, "*ERROR* Cannot write EEPROM byte %d", Bytes);
        Pixie_Print_MSG(ErrMSG);
        fclose(eeprom_file_id);
        return (-4);
    }
    Sleep(5);
    Bytes++;

    // Write ADC sampling rate

    ByteValues[Bytes] = ADCrate & 0xFF;

    if (I2CM24C64_Write_One_Byte(ModNum, Bytes, (char*) &ByteValues[Bytes]) != 0) {
        printf("*ERROR* Cannot write EEPROM byte %d", Bytes);
        sprintf(ErrMSG, "*ERROR* Cannot write EEPROM byte %d", Bytes);
        Pixie_Print_MSG(ErrMSG);
        fclose(eeprom_file_id);
        return (-4);
    }
    Sleep(5);
    Bytes++;

    if (ADCrate > 255)
        ByteValues[Bytes] = (ADCrate & 0xFF00) / 256;
    else
        ByteValues[Bytes] = 0;

    if (I2CM24C64_Write_One_Byte(ModNum, Bytes, (char*) &ByteValues[Bytes]) != 0) {
        printf("*ERROR* Cannot write EEPROM byte %d", Bytes);
        sprintf(ErrMSG, "*ERROR* Cannot write EEPROM byte %d", Bytes);
        Pixie_Print_MSG(ErrMSG);
        fclose(eeprom_file_id);
        return (-4);
    }
    Sleep(5);
    Bytes++;


    // Write 0xFF to remaining unused memory words

    for (count = Bytes; count < 8192; count++) {
        ByteValues[count] = 0xFF;

        if (I2CM24C64_Write_One_Byte(ModNum, Bytes, (char*) &ByteValues[count]) != 0) {
            printf("*ERROR* Cannot write EEPROM byte %d", count);
            sprintf(ErrMSG, "*ERROR* Cannot write EEPROM byte %d", count);
            Pixie_Print_MSG(ErrMSG);
            fclose(eeprom_file_id);
            return (-4);
        }
        Sleep(5);
    }


    cout << "Checking results displaying the first few bytes " << endl;

    Bytes = 0;
    while (Bytes < 5) {

        ByteValues[count] = 0;

        if (I2CM24C64_Read_One_Byte(ModNum, Bytes, (char*) &ByteValues[Bytes]) != 0) {
            printf("*ERROR* Cannot read EEPROM byte %d", Bytes);
            return (-4);
        }
        Sleep(5);
        printf("Byte #%d:  %d\n", Bytes, ByteValues[Bytes]);
        Bytes++;
    }

    Sleep(1000);

    return (1);
}
