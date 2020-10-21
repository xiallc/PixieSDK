#pragma rtGlobals=1		// Use modern global access method.

Function GetMCAData(datafilenam1, datafilenam2)
	String datafilenam1, datafilenam2
	
	GBLoadWave/O/P=home/N=mca_g09/B/T={96,4}/W=1 datafilenam1
	GBLoadWave/O/P=home/N=mca_g40/B/T={96,4}/W=1 datafilenam2	
	
	Return(0)
End

Function DoMCAFit() 

	Wave data0 = mca_g090
	Wave data1 = mca_g400
	Wave W_coef
	
	Wave FWHM_g09, FWHM_g40, PeakPos_g09, PeakPos_g40
	
	Variable R0_x0, R0_x1, R1_x0, R1_x1, k
	
	Variable Threshold, m
	
	Threshold = 200
	
	for(k=0; k<16; k+=1)
		// Search for appropriate fitting range
		m = 32768 * (k + 1)
		do
			m -= 1
		while( (m > (32768 * k)) && (data0[m] < Threshold) )
		R0_x0 = m - 100 
		R0_x1 = m + 100 
		
//print 	R0_x0 - 32768*k, R0_x1 - 32768*k

		m = 32768 * (k + 1)
		do
			m -= 1
		while( (m > (32768 * k)) && (data1[m] < Threshold) )
		R1_x0 = m - 350 
		R1_x1 = m + 500 
		
		// Fit
		CurveFit/Q gauss data0[R0_x0, R0_x1] /D
		FWHM_g09[k] = 2*sqrt(ln(2))*W_coef[3]/(W_coef[2] - 32768 * k) * 100
		PeakPos_g09[k] = W_coef[2] - 32768 * k
		CurveFit/Q gauss data1[R1_x0, R1_x1] /D
		FWHM_g40[k] = 2*sqrt(ln(2))*W_coef[3]/(W_coef[2] - 32768 * k) * 100
		PeakPos_g40[k] = W_coef[2] - 32768 * k
	endfor

	Return(0)
End

Function SplitMCAs()

	Make/o/n=(32768, 16) SglMCA_g09, SglMCA_g40
	
	Wave data0 = mca_g090
	Wave data1 = mca_g400
	Variable k, n
	
	for(k=0; k<16; k+=1)
		for(n=0; n<32768; n+=1)
			SglMCA_g09[n][k] = data0[n + k*32768]
			SglMCA_g40[n][k] = data1[n + k*32768]
		endfor
	endfor
	
	Return(0)
End


Function DisplayMCAs()
	Wave SglMCA_g09, SglMCA_g40
	
	Display SglMCA_g09[][0]
	AppendToGraph SglMCA_g09[][1]
	AppendToGraph SglMCA_g09[][2]
	AppendToGraph SglMCA_g09[][3]
	AppendToGraph SglMCA_g09[][4]
	AppendToGraph SglMCA_g09[][5]
	AppendToGraph SglMCA_g09[][6]
	AppendToGraph SglMCA_g09[][7]
	AppendToGraph SglMCA_g09[][8]
	AppendToGraph SglMCA_g09[][9]
	AppendToGraph SglMCA_g09[][10]
	AppendToGraph SglMCA_g09[][11]
	AppendToGraph SglMCA_g09[][12]
	AppendToGraph SglMCA_g09[][13]
	AppendToGraph SglMCA_g09[][14]
	AppendToGraph SglMCA_g09[][15]
	
	Display SglMCA_g40[][0]
	AppendToGraph SglMCA_g40[][1]
	AppendToGraph SglMCA_g40[][2]
	AppendToGraph SglMCA_g40[][3]
	AppendToGraph SglMCA_g40[][4]
	AppendToGraph SglMCA_g40[][5]
	AppendToGraph SglMCA_g40[][6]
	AppendToGraph SglMCA_g40[][7]
	AppendToGraph SglMCA_g40[][8]
	AppendToGraph SglMCA_g40[][9]
	AppendToGraph SglMCA_g40[][10]
	AppendToGraph SglMCA_g40[][11]
	AppendToGraph SglMCA_g40[][12]
	AppendToGraph SglMCA_g40[][13]
	AppendToGraph SglMCA_g40[][14]
	AppendToGraph SglMCA_g40[][15]
	
	Return(0)
End

Function StartLogs_mca()

	Svar testfilename, testlogname
	
	testfilename = "HardwareTest_mca_12222009.ifn"
	sprintf testlogname,"TestLogBook1"

	NewNotebook/N=logbook/V=1/F=1
	DoWindow/F logbook
	DoWindow/C $(testlogname)
	DoWindow/B $(testlogname)
	SaveNotebook/P=home $(testlogname) as testfilename

	Return(0)
End

Function MCAFits_AllFiles()

	Svar testfilename, testlogname
	String linestr, outtext, datafilenam1, datafilenam2
	Variable NumModules, filnum1, k
	
	Nvar ModNum
	
	Wave ModSerNums
	
	NumModules = ModNum
	
//	Make/o/n=100 modsernums
//	Open/R/P=Home filnum1 as "modsernums.txt"

	StartLogs_mca()	
	OpenNoteBook/P=Home testfilename
	
//	NumModules = 0
//	do
//		freadline filnum1, linestr
//		modsernums[NumModules] = str2num(linestr)
//		NumModules += 1
//		Fstatus filnum1
//	while(V_filePos < V_logEOF)
//	
//	DeletePoints NumModules, (100 - NumModules), modsernums

	Make/o/n=16 FWHM_g09, FWHM_g40, PeakPos_g09, PeakPos_g40
	
	sprintf outtext, "Start to process mca data for %d modules ...\r", NumModules
	NoteBook $(testlogname), selection={endOfFile, endOfFile}
	Notebook $(testlogname), text=outtext

	for(k=0; k<NumModules; k+=1)
		sprintf outtext, "\r\rMCA gauss fit for module serial number %d\r", modsernums[k]
		Notebook $(testlogname), text=outtext

		sprintf datafilenam1, "mcahistogram_mod#%d_g09.bin", modsernums[k]
		sprintf datafilenam2, "mcahistogram_mod#%d_g40.bin", modsernums[k]
		
		GetMCAData(datafilenam1, datafilenam2)
		SplitMCAs()
		DoMCAFit()
		NoteBook $(testlogname), selection={endOfFile, endOfFile}
		NoteBook $(testlogname), frame = 1, picture = {Table_MCAFitData, 0, 0}
		KillWaves  mca_g090,mca_g400
	endfor
	
	SaveNoteBook $(testlogname) as testfilename
	
	Close filnum1
	Return(0)
End

Window Table_MCAFitData() : Table
	PauseUpdate; Silent 1		// building window...
	Edit/W=(24,41,438.75,277.25) FWHM_g09,FWHM_g40,PeakPos_g09,PeakPos_g40
	ModifyTable format(Point)=1,format(FWHM_g09)=3,digits(FWHM_g09)=6,format(PeakPos_g09)=3
	ModifyTable digits(PeakPos_g09)=1,format(PeakPos_g40)=3,digits(PeakPos_g40)=1,sigDigits(PeakPos_g40)=1
EndMacro

Function MCAFits_OneFile(modsernum)
Variable modsernum

	String datafilenam1, datafilenam2

	Make/o/n=16 FWHM_g09, FWHM_g40, PeakPos_g09, PeakPos_g40
	
	sprintf datafilenam1, "mcahistogram_mod#%d_g09.bin", modsernum
	sprintf datafilenam2, "mcahistogram_mod#%d_g40.bin", modsernum
		
	GetMCAData(datafilenam1, datafilenam2)
	SplitMCAs()
	DoMCAFit()
	//KillWaves  mca_g090,mca_g400

	Return(0)
End