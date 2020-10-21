# Pixie Software API
Software used to access Pixie hardware using the PLX 9054 driver. Provides
user functions to perform common tasks on the hardware (ex. set parameters,
start runs, load firmware).

## Procedure for making Pixie-16 testing programs
*NOTE:* Pixie-16 testing programs should ONLY be compiled with Pixie-16
static libraries.

1. Open a command line window, go to C:\Program Files\Microsoft Visual Studio 8\VC\bin, and then type 'vcvars32.bat' to set up VC tools environment.
2. Go to C:\path\to\project\root\software, and type 'make clean'
3. Go to C:\path\to\project\root\software\testing, and type 'rm *.exe'
4. Go back to C:\path\to\project\root\software, and type 'make tests'.
