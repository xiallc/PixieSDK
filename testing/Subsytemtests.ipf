#pragma rtGlobals=1		// Use modern global access method.

Macro InitGlobals()
	Variable/G NumLinesBefADCvsDAC = 8198
	Variable/G NumLinesBefFFT = (NumLinesBefADCvsDAC + 27000 * 16 + 3)
	String/G testfilename, testlogname
EndMacro


Function GetModuleInfo(datafilenam)
	String datafilenam
	
	Svar testlogname
	
	Variable filnum, modnum, revnum, k, num, gainmatch
	Variable val0, val1, val2
	String linestr, outtext
	
	Make/o/n=6 val
	
	open/R/P=Home filnum as datafilenam
	freadline filnum, linestr
	freadline filnum, linestr
	freadline filnum, linestr
	
	// Read module number, revision number
	freadline filnum, linestr
	sscanf linestr, "%d\r", num
	val[0] = num
	freadline filnum, linestr
	sscanf linestr, "%d\r", num
	val[1] = num
	freadline filnum, linestr
	sscanf linestr, "%d\r", num
	val[2] = num

	// Check module number storage: one-byte or two-byte
	// Here we use the fact that if module number is stored as one-byte value, the third byte
	// is the Ch0 gain value high byte, which is typically 0. For Rev-D modules with module
	// number greater than 255, the third byte stores the revision number, which is at least 0xD.
	if(val[2] >= 0xD)
		modnum = val[0] + val[1] * 256
		revnum = val[2]
	else
		modnum = val[0]
		revnum = val[1]
	endif
	sprintf outtext, "\r\rModule number = %d\r", modnum
	Notebook $(testlogname), text=outtext
	sprintf outtext, "Revision number = %x\r", revnum
	Notebook $(testlogname), text=outtext
	
	// Check module gain information
	gainmatch = 1
	// Check Ch0 first
	if(val[2] >= 0xD)
		freadline filnum, linestr
		sscanf linestr, "%d\r", num
		val[0] = num
	else
		val[0] = val[2]
	endif
	freadline filnum, linestr
	sscanf linestr, "%d\r", num
	val[1] = num
	freadline filnum, linestr
	sscanf linestr, "%d\r", num
	val[2] = num
	freadline filnum, linestr
	sscanf linestr, "%d\r", num
	val[3] = num
	freadline filnum, linestr
	sscanf linestr, "%d\r", num
	val[4] = num
	freadline filnum, linestr
	sscanf linestr, "%d\r", num
	val[5] = num
	if( (val[0] != 0) || (val[1] != 40) || (val[2] != 1) || (val[3] != 0) || (val[4] != 9) || (val[5] != 2) )
		sprintf outtext, "Module %d gain information mismatch\r", modnum
		Notebook $(testlogname), text=outtext
		gainmatch = 0
	endif
	// Check other 15 channels		
	for(k=1; k<16; k+=1)
		freadline filnum, linestr
		sscanf linestr, "%d\r", num
		val[0] = num
		freadline filnum, linestr
		sscanf linestr, "%d\r", num
		val[1] = num
		freadline filnum, linestr
		sscanf linestr, "%d\r", num
		val[2] = num
		freadline filnum, linestr
		sscanf linestr, "%d\r", num
		val[3] = num
		freadline filnum, linestr
		sscanf linestr, "%d\r", num
		val[4] = num
		freadline filnum, linestr
		sscanf linestr, "%d\r", num
		val[5] = num
		if( (val[0] != 0) || (val[1] != 40) || (val[2] != 1) || (val[3] != 0) || (val[4] != 9) || (val[5] != 2) )
			sprintf outtext, "Module %d gain information mismatch\r", modnum
			Notebook $(testlogname), text=outtext
			gainmatch = 0
		endif
	endfor
	
	if(gainmatch == 1)
		sprintf outtext, "Module gain information OK\r\r\r"
		Notebook $(testlogname), text=outtext
	endif
	
	close filnum
	
	Return(0)
End

Function GetADCvsDAC_Data(numpoints, datafilenam)
Variable numpoints
String datafilenam

	Variable filnum, k, modread, chanread, offsetdacread, adcavgread, adcdevread, n
	String linestr
	Nvar NumLinesBefADCvsDAC
	
	Make/o/n=(65536, 16) offsetdac, adcavg, adcdev
	offsetdac = 0
	adcavg = 0
	adcdev = 0
	
	open/R/P=Home filnum as datafilenam
	freadline filnum, linestr
	freadline filnum, linestr
	freadline filnum, linestr
	
	// Read module number
	freadline filnum, linestr
	sscanf linestr, "%d\r", modread
	printf "GetADCvsDAC_Data: module number = %d\r", modread
	
	// Skip lines
	for(k=0; k<(NumLinesBefADCvsDAC - 4); k+=1)
		freadline filnum, linestr
	endfor

	for(k=0; k<(numpoints*16); k+=1)
		freadline filnum, linestr
		sscanf linestr, "%d\t%d\t%d\t%f\t%f\r", modread, chanread, offsetdacread, adcavgread, adcdevread		
		offsetdac[offsetdacread][chanread] = offsetdacread
		adcavg[offsetdacread][chanread] = adcavgread
		adcdev[offsetdacread][chanread] = adcdevread
	endfor
	
	for(k=0; k<numpoints; k+=1)
		for(n=0; n<16; n+=1)
			if(offsetdac[k][n] != k)
				print k, n
				close filnum
				Return(-1)
			endif
		endfor
	endfor
	
	close filnum

	Return(0)
End


Function GetADC_FFT_Data(datafilenam)
String datafilenam

	Variable filnum, k, modread, chanread, adcfft_read, n
	String linestr
	Nvar NumLinesBefFFT
	
	Make/o/n=(8192, 16) adc_fft
	
	open/R/P=Home filnum as datafilenam
	freadline filnum, linestr
	freadline filnum, linestr
	freadline filnum, linestr
	
	// Read module number
	freadline filnum, linestr
	sscanf linestr, "%d\r", modread
	printf "GetADC_FFT_Data: module number = %d\r", modread
	
	// Skip lines
	for(k=0; k<(NumLinesBefFFT - 4); k+=1)
		freadline filnum, linestr
	endfor

	n = 0
	for(k=0; k<(8192*16); k+=1)
		freadline filnum, linestr
		sscanf linestr, "%d\t%d\t%f\r", modread, chanread, adcfft_read	
		adc_fft[n][chanread] = adcfft_read
		n += 1
		
		if(mod(n, 8192) == 0)
			n=0
		endif
	endfor
	
	close filnum

	Return(0)
End


Function FitADCvsDAC_Slope(numpoints)
Variable numpoints

	Wave adcavg
	Variable k, m, n
	
	Make/o/n=16 ADCvsDAC_Slope, ADCvsDAC_Start, ADCvsDAC_End, ADCvsDAC_Intercept
	Make/o/n=(numpoints) FitDataWave
	
	for(k=0; k<16; k+=1)
		// Find the first non-zero point
		m=15500
		do
			if(adcavg[m][k] > 0)
				break
			endif
			m += 1
		while(m < numpoints)
		if(m == numpoints)
			print "ERROR: adcavg wave all zeros"
			//Return(-1)
		endif
		
		// Find the first non-4095 point
		//n=numpoints-1
		n=42499
		do
			if(adcavg[n][k] < 4095)
				break
			endif
			n -= 1
		while(n > 0)
		if(n == 0)
			print "ERROR: adcavg wave all 4095s"
			//Return(-2)
		endif

		ADCvsDAC_Start[k] = m
		ADCvsDAC_End[k] = n

		// Relax two end points a little
		m += 5
		n -= 5
				
		// Linear fit
		FitDataWave = adcavg[p][k]
		
		CurveFit/Q line, FitDataWave[m, n]
		Wave W_coef
		ADCvsDAC_Intercept[k] = W_coef[0]
		ADCvsDAC_Slope[k] = W_coef[1]
		
		// Check if slope is negative
		if(ADCvsDAC_Slope[k] < 0)
			// re-do the fit
			
			// Find the first non-4095 point
			m=15500
			do
				if(adcavg[m][k] < 4095)
					break
				endif
				m += 1
			while(m < numpoints)
			if(m == numpoints)
				print "ERROR: adcavg wave all 4095s"
				//Return(-1)
			endif
			
			// Find the first non-zero point
			//n=numpoints-1
			n=42499
			do
				if(adcavg[n][k] > 0)
					break
				endif
				n -= 1
			while(n > 0)
			if(n == 0)
				print "ERROR: adcavg wave all zeros"
				//Return(-2)
			endif
	
			ADCvsDAC_Start[k] = m
			ADCvsDAC_End[k] = n
	
			// Relax two end points a little
			m += 5
			n -= 5
					
			// Linear fit
			FitDataWave = adcavg[p][k]
			
			CurveFit/Q line, FitDataWave[m, n]
			Wave W_coef
			ADCvsDAC_Intercept[k] = W_coef[0]
			ADCvsDAC_Slope[k] = W_coef[1]
		endif		
	endfor

	Return(0)
End


Function FindMaxMinADCDev()

	Wave adcdev, ADCvsDAC_Start, ADCvsDAC_End
	Variable k, m, n
	
	Make/o/n=16 MaxADCDev, MinADCDev
	
	MaxADCDev = 0
	MinADCDev = 1e9
	
	for(k=0; k<16; k+=1)
		for(m=ADCvsDAC_Start[k]; m<=ADCvsDAC_End[k]; m+=1)
			if(adcdev[m][k] > MaxADCDev[k])
				MaxADCDev[k] = adcdev[m][k]
			endif
			if(adcdev[m][k] < MinADCDev[k])
				MinADCDev[k] = adcdev[m][k]
			endif
		endfor
	endfor
	
	Return(0)
End


Function FindMaxMinADCFFT()

	Wave adc_fft
	Variable k, m, n
	
	Make/o/n=16 MaxADCFFT, MinADCFFT
	Make/o/n=8192 tempdata
	
	for(k=0; k<16; k+=1)
		tempdata = adc_fft[p][k]
		WaveStats/Q/R=(1, 8191) tempdata
		MaxADCFFT[k] = V_max
		MinADCFFT[k] = V_min
	endfor
		
	Return(0)
End


Function StartLogs()

	Svar testfilename, testlogname
	
	testfilename = "Pixie16AnalogFrontEndTest_02162012.ifn"
	sprintf testlogname,"TestLogBook0"

	NewNotebook/N=logbook/V=1/F=1
	DoWindow/F logbook
	DoWindow/C $(testlogname)
	DoWindow/B $(testlogname)
	SaveNotebook/P=home $(testlogname) as testfilename

	Return(0)
End


Function DoTests()

	Svar testfilename, testlogname
	String linestr, outtext, datafilenam
	Variable NumModules, filnum1, k
	
	Nvar ModNum
	
	Wave ModSerNums
	
	NumModules = ModNum
	
	StartLogs()	
	OpenNoteBook/P=Home testfilename

	sprintf outtext, "Start to process test data for %d modules ...\r", NumModules
	Notebook $(testlogname), text=outtext

	for(k=0; k<NumModules; k+=1)
		sprintf datafilenam, "analogfrontend_test_mod#%d.dat", modsernums[k]
		GetModuleInfo(datafilenam)
		GetADCvsDAC_Data(65536, datafilenam)
		FitADCvsDAC_Slope(65536)
		FindMaxMinADCDev()
		GetADC_FFT_Data(datafilenam)
		FindMaxMinADCFFT()
		NoteBook $(testlogname), selection={endOfFile, endOfFile}
		NoteBook $(testlogname), frame = 1, picture = {Table_ADCvsDAC, 0, 0}
		NoteBook $(testlogname), selection={endOfFile, endOfFile}
		NoteBook $(testlogname), frame = 1, picture = {Table_MinMaxADCDev, 0, 0}
		NoteBook $(testlogname), selection={endOfFile, endOfFile}
		NoteBook $(testlogname), frame = 1, picture = {Table_MinMaxADCFFT, 0, 0}
	endfor
	
	SaveNoteBook $(testlogname) as testfilename
	Return(0)
End


Window Table_ADCvsDAC() : Table
	PauseUpdate; Silent 1		// building window...
	Edit/W=(3.75,304.25,510.75,568.25) ADCvsDAC_Slope,ADCvsDAC_Intercept,ADCvsDAC_Start
	AppendToTable ADCvsDAC_End
	ModifyTable format(Point)=1,format(ADCvsDAC_Slope)=3,digits(ADCvsDAC_Slope)=6,width(ADCvsDAC_Slope)=110
	ModifyTable format(ADCvsDAC_Intercept)=3,width(ADCvsDAC_Intercept)=114,width(ADCvsDAC_Start)=98
	ModifyTable width(ADCvsDAC_End)=98
EndMacro

Window Table_MinMaxADCDev() : Table
	PauseUpdate; Silent 1		// building window...
	Edit/W=(5.25,42.5,252,305.75) MinADCDev,MaxADCDev
	ModifyTable format(Point)=1,format(MaxADCDev)=3,digits(MaxADCDev)=6
EndMacro

Window Table_MinMaxADCFFT() : Table
	PauseUpdate; Silent 1		// building window...
	Edit/W=(5.25,42.5,252,305.75) MinADCFFT,MaxADCFFT
	ModifyTable format(Point)=1,format(MinADCFFT)=3,digits(MinADCFFT)=6,sigDigits(MinADCFFT)=10
EndMacro

