
#pragma rtGlobals=1		// Use modern global access method.

Function GetMCAData(datafilenam1)
	String datafilenam1
	
	GBLoadWave/O/P=home/N=mca_g09/B/T={96,4}/W=1 datafilenam1
	//GBLoadWave/O/P=home/N=mca_g40/B/T={96,4}/W=1 datafilenam2	
	
	Return(0)
End

Function DoWind()

	//Make/o/n=16 FWHM_g09, FWHM_g40, PeakPos_g09, PeakPos_g40
	//Make/o/n=(16,10) data
	Wave W_coef
	//Make/o/n=(16) W_coef
	Wave sglMCA_g09
	//Wave W_coef
	Wave FWHM_g09, FWHM_g40, PeakPos_g09, PeakPos_g40
	
	Variable k,l, ppos,m, Threshold, FWHM
	Variable R0_x0, R0_x1, R1_x0, R1_x1
	make/o/d/n=(33,16) Peak_pos
	
	for(k=0;k<16;k+=1)
		
		//wave Peak_pos
		wave mca = $("MCA"+num2str(k))
		mca = sglMCA_g09[p][k]

			setscale /P x, 0, 1, mca
			CurveFit/q gauss mca[14115,15675]
			setscale /P x, 0, (1332.5/W_coef[2]), mca

	print W_coef[2]
//	CurveFit/q gauss mca(71.4,73.5)/D
//	Peak_pos[0][k] = W_Coef[2]
////	print W_Coef[0], W_coef[1], W_coef[2]
//	CurveFit/q gauss mca(73.5,76.25)/D
////	print W_Coef[0], W_coef[1], W_coef[2]
//	Peak_pos[1][k] = W_Coef[2]
//	CurveFit/q gauss mca(78.5,81.98)/D
//	Peak_pos[2][k] = W_Coef[2]
//	CurveFit/q gauss mca(120.4, 123.1)/D
//	Peak_pos[3][k] = W_Coef[2]
//	CurveFit/q gauss mca(135.0,137.3)/D
//	Peak_pos[4][k] = W_Coef[2]
//	CurveFit/q gauss mca(208.42,210.15)/D
//	Peak_pos[5][k] = W_Coef[2]
//	CurveFit/q gauss mca(235.63,239.86)/D
//	Peak_pos[6][k] = W_Coef[2]
//	CurveFit/q gauss mca(239.86,242.68)/D
//	Peak_pos[7][k] = W_Coef[2]
//	CurveFit/q gauss mca(267.7, 271.7)/D
//	Peak_pos[8][k] = W_Coef[2]
//	CurveFit/q gauss mca(298.78,301.12)/D
//      Peak_pos[9][k] = W_Coef[2]
//      CurveFit/q gauss mca(301.12,305.0)/D
//      Peak_pos[10][k] = W_Coef[2]		
//	CurveFit/q gauss mca(325.1,329.59)/D
//	Peak_pos[11][k] = W_Coef[2]
//	CurveFit/q gauss mca(336.12,340.79)/D
//	Peak_pos[12][k] = W_Coef[2]
//	CurveFit/q gauss mca(351.81,360.09)/D
//	Peak_pos[13][k] = W_Coef[2]	
//	CurveFit/q gauss mca(380.1,386.72)/D
//	Peak_pos[14][k] = W_Coef[2]
//	CurveFit/q gauss mca(407.52,412.37)/D
//	Peak_pos[15][k] = W_Coef[2]
//	CurveFit/q gauss mca(459.0,468.8)/D
//	Peak_pos[16][k] = W_Coef[2]
//	CurveFit/q gauss mca(499.02,518.94)/D
//	Peak_pos[17][k] = W_Coef[2]
//	CurveFit/q gauss mca(571.4,596.6)/D
//	Peak_pos[18][k] = W_Coef[2]
//	CurveFit/q gauss mca(650.8,671.27)/D
//	Peak_pos[19][k] = W_Coef[2]
//	CurveFit/q gauss mca(714.5,736.6)/D
//	Peak_pos[20][k] = W_Coef[2]
//	CurveFit/q gauss mca(789.8,802.1)/D
//	Peak_pos[21][k] = W_Coef[2]
//	CurveFit/q gauss mca(850.56,869.69)/D
//	Peak_pos[22][k] = W_Coef[2]
//	CurveFit/q gauss mca(909.63,920.55)/D
//	Peak_pos[23][k] = W_Coef[2]
//	CurveFit/q gauss mca(962.34, 966.66)/D
//	Peak_pos[24][k] = W_Coef[2]
//	CurveFit/q gauss mca(966.66, 971.68)/D
//	Peak_pos[25][k] = W_Coef[2]
//	CurveFit/q gauss mca(1165.5, 1181.8)/D
//	Peak_pos[26][k] = W_Coef[2]
//	CurveFit/q gauss mca(1259.2,1293.2)/D
//	Peak_pos[27][k] = W_Coef[2]
//	CurveFit/q gauss mca(1319.5, 1347.4)/D
//	Peak_pos[28][k] = W_Coef[2]
//	CurveFit/q gauss mca(1586.4,1590.4)/D
//	Peak_pos[29][k] = W_Coef[2]
//	CurveFit/q gauss mca(1590.5,1594.9)/D
//	Peak_pos[30][k] = W_Coef[2]
//	CurveFit/q gauss mca(2093.8,2113.8)/D
//	Peak_pos[31][k] = W_Coef[2]
//	CurveFit/q gauss mca(2600.4,2626.5)/D
//	Peak_pos[32][k] = W_Coef[2]
	endfor
	
	DoWindow/F MCA_Overlay
	if (V_flag!=1)
		k=0
		wave mca = $("MCA"+num2str(k))
		Display/K=1 mca
		DoWindow/C MCA_Overlay
		Legend/C/N=text0/F=0/A=MC
		
		for(k=1;k<16;k+=1)
			wave mca = $("MCA"+num2str(k))
			print "MCA"+num2str(k)
			AppendtoGraph mca
		endfor
		ModifyGraph rgb(MCA1)=(0,12800,52224)
		ModifyGraph rgb(MCA2)=(0,0,0)
	endif
	

	Return(0)
End

Function DoMCAFit() 

	Wave data0 = mca_g090
	//Wave data1 = mca_g400
	Wave W_coef
	
	Wave FWHM_g09, FWHM_g40, PeakPos_g09, PeakPos_g40
	
	Variable R0_x0, R0_x1, R1_x0, R1_x1, k
	
	Variable Threshold, m
	
	Threshold = 200
	
	for(k=0; k<16; k+=1)
		// Search for appropriate fitting range
		//m = 32768 * (k + 1)
		do
			m -= 1
		while(data0[m] < Threshold)
		R0_x0 = m - 10
		R0_x1 = m + 10 
		
//print 	R0_x0 - 32768*k, R0_x1 - 32768*k

//		m = 32768 * (k + 1)
//		do
//			m -= 1
//		while( (m > (32768 * k)) && (data1[m] < Threshold) )
//		R1_x0 = m - 350 
//		R1_x1 = m + 500 
		
//	R1_x0 = 14542
//	R1_x1 = 15190

		// Fit
		CurveFit/Q gauss data0[R0_x0, R0_x1] /D
		FWHM_g09[k] = 2*sqrt(ln(2))*W_coef[3]/(W_coef[2] - 32768 * k) * 100
		PeakPos_g09[k] = W_coef[2] - 32768 * k
//		CurveFit/Q gauss data1[R1_x0, R1_x1] /D
		FWHM_g40[k] = 2*sqrt(ln(2))*W_coef[3]/(W_coef[2] - 32768 * k) * 100
		PeakPos_g40[k] = W_coef[2] - 32768 * k
		printf " The peak position is %d and the FWHM is %d\r", PeakPos_g40[k], FWHM_g40[k]
	endfor

	Return(0)
End

Function SplitMCAs()

	Make/o/n=(32768, 16) SglMCA_g09, SglMCA_g40
	Wave data0 = mca_g090
//	Wave data1 = mca_g400
	Variable k, n
	
	for(k=0; k<16; k+=1)
		for(n=0; n<32768; n+=1)
			SglMCA_g09[n][k] = data0[n + k*32768]
//			SglMCA_g40[n][k] = data1[n + k*32768]
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
		
		GetMCAData(datafilenam1)
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
		
	GetMCAData(datafilenam1)
	SplitMCAs()
	DoMCAFit()
	//KillWaves  mca_g090,mca_g400

	Return(0)
End