Procedure for making Pixie-16 testing programs

NOTE: Pixie-16 testing programs should ONLY be compiled with Pixie-16 static libraries.

1. Open a command line window, go to C:\Program Files\Microsoft Visual Studio 8\VC\bin, and then type 'vcvars32.bat' to set up VC tools environment.

2. Go to C:\XIA\SVN\pixie16\trunk\software, and type 'make clean'

3. Go to C:\XIA\SVN\pixie16\trunk\software\testing, and type 'rm *.exe'

4. Go back to C:\XIA\SVN\pixie16\trunk\software, and type 'make tests'.
