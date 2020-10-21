
 #pragma rtGlobals=1		// Use modern global access method.

Macro InitTestGlobals()

	Variable/G ModNum = 0

End

Window testpanel() : Panel
	PauseUpdate; Silent 1		// building window...
	NewPanel /W=(2,62,778,450)
	ShowTools
	SetVariable NumberOfModules,pos={169,67},size={132,16},title="Number of Modules"
	SetVariable NumberOfModules,value= ModNum
	Button ProgramCPLDButton,pos={560,32},size={99,30},proc=CommonButtonControl,title="Program CPLD"
	Button ProgramCPLDButton,help={"Click here to program CPLD"},fStyle=1
	Button ProgramI2CEEPROMButton,pos={24,60},size={137,29},proc=CommonButtonControl,title="Program I2C EEPROM"
	Button ProgramI2CEEPROMButton,help={"Click here to program I2C EEPROM"},fStyle=1
	Button SlotsButton,pos={312,62},size={45,29},proc=CommonButtonControl,title="Slots"
	Button SlotsButton,help={"Click here to set slot parameters"},fStyle=1
	Button BootButton,pos={23,23},size={135,31},proc=CommonButtonControl,title="Boot Pixie-16 Modules"
	Button BootButton,help={"Click here to boot Pixie-16 modules"},fStyle=1
	Button ProgramPLXEEPROMButton,pos={23,98},size={142,29},proc=CommonButtonControl,title="Program PLX EEPROM"
	Button ProgramPLXEEPROMButton,help={"Click here to program PLX EEPROM"},fStyle=1
	Button AnalogTestButton,pos={26,138},size={107,29},proc=CommonButtonControl,title="Run Analog Test"
	Button AnalogTestButton,help={"Click here to run analog frontend test"},fStyle=1
	Button AnalyzeAnalogTestButton,pos={145,138},size={129,29},proc=CommonButtonControl,title="Analog Test Analisis"
	Button AnalyzeAnalogTestButton,help={"Click here to analyze analog frontend test results"}
	Button AnalyzeAnalogTestButton,fStyle=1
	Button MCATestButton,pos={27,179},size={107,29},proc=CommonButtonControl,title="Run MCA Test"
	Button MCATestButton,help={"Click here to run MCA test"},fStyle=1
	Button AnalyzeMCATestButton,pos={146,179},size={129,29},proc=CommonButtonControl,title="MCA Test Analisis"
	Button AnalyzeMCATestButton,help={"Click here to analyze MCA test results"}
	Button AnalyzeMCATestButton,fStyle=1
	Button LMTestButton,pos={27,220},size={107,29},proc=CommonButtonControl,title="Run LM Test"
	Button LMTestButton,help={"Click here to run LM test"},fStyle=1
	Button AnalyzeLMTestButton,pos={146,220},size={129,29},proc=CommonButtonControl,title="LM Test Analisis"
	Button AnalyzeLMTestButton,help={"Click here to analyze LM test results"}
	Button AnalyzeLMTestButton,fStyle=1
	
EndMacro

Function CommonButtonControl(ctrlName) : ButtonControl

	String ctrlName
	
	if(cmpstr(ctrlName,"ProgramCPLDButton")==0)
		ProgramCPLD()
	endif
	
	if(cmpstr(ctrlName,"ProgramI2CEEPROMButton")==0)
		ProgramI2C_EEPROM()
	endif
	
	if(cmpstr(ctrlName,"SlotsButton")==0)
		AssignSlotNumbers()
	endif
	
	if(cmpstr(ctrlName,"BootButton")==0)
		BootPixie()
	endif
	
	if(cmpstr(ctrlName,"ProgramPLXEEPROMButton")==0)
		ProgramPLX_EEPROM()
	endif
	
	if(cmpstr(ctrlName,"AnalogTestButton")==0)
		RunAnalogTest()
	endif
	
	if(cmpstr(ctrlName,"AnalyzeAnalogTestButton")==0)
		AnalyzeAnalogTest()
	endif
	
	if(cmpstr(ctrlName,"MCATestButton")==0)
		RunMCATest()
	endif
	
	if(cmpstr(ctrlName,"AnalyzeMCATestButton")==0)
		AnalyzeMCATest()
	endif
	
	if(cmpstr(ctrlName,"LMTestButton")==0)
		RunLMTest()
	endif
	
	if(cmpstr(ctrlName,"AnalyzeLMTestButton")==0)
		AnalyzeLMTest()
	endif
	
End


Function ProgramCPLD()

	print "Programming CPLD"
	print "WHEN FINISHED CLOSE THE TERMINAL BEFORE RETURNING TO IGOR"
	
	ExecuteScriptText "pixie16.bat"
	
End


Function ProgramI2C_EEPROM()

	String ComStr
	Variable i
	
	Nvar ModNum
	
	Wave Modules
	Wave ModSerNums
	Wave Revisions
	
	print "Programming I2C EEPROM"
	print "WHEN FINISHED CLOSE THE TERMINAL BEFORE RETURNING TO IGOR"
	
	for(i = 0; i < ModNum; i+=1)
		sprintf ComStr, "eeprom_prog.exe %d  %d  %d", Modules[i], ModSerNums[i], Revisions[i]
		print "Programming: ", ComStr
		ExecuteScriptText ComStr
		Sleep/T 60
	endfor
	
End

Function AssignSlotNumbers()

	Nvar ModNum
	
	if (ModNum == 0)
		print "Number of modules is zero"
		return 1
	endif

	print "Number of modules is ", ModNum
	make/o/u/w/n = (ModNum) Modules, Slots, ModSerNums, Revisions
	edit/n = table0  Modules, Slots, ModSerNums, Revisions
	
End

Function BootPixie()

	print "Booting ..."
	
	ExecuteScriptText "boot.exe"
	
End

Function ProgramPLX_EEPROM()

	String ComStr
	Variable i
	Nvar ModNum

	print "Programming PLX EEPROM"
	print "WHEN FINISHED CLOSE THE TERMINAL BEFORE RETURNING TO IGOR"
	
	for(i = 0; i < ModNum; i+=1)
		sprintf ComStr, "cmd /c cd prom & PlxEep.exe -v -i -l PLX9054EEPROM_P16RevBRevC.eep -d  %d", i + 1
		print "Programming: ", ComStr
		ExecuteScriptText ComStr
	endfor
	
End

Function RunAnalogTest()

	print "Running analog frontend test"
	
	ExecuteScriptText "subsystem_tests.exe"
	
End

Function AnalyzeAnalogTest()

	print "Analyzing analog frontend test"
	
	DoTests()
	
End

Function RunMCATest()

	print "Running MCA test"
	
	ExecuteScriptText "mca_tests.exe"
	
End

Function AnalyzeMCATest()

	print "Analyzing MCA test"
	
	MCAFits_AllFiles()
	
End

Function RunLMTest()

	print "Running LM test"
	
	ExecuteScriptText "Pixie16Test_RevDGeneral.exe"
	
End

Function AnalyzeLMTest()

	print "Analyzing MCA test"
	
	ExecuteScriptText "check_eventheader.exe"
	
End
