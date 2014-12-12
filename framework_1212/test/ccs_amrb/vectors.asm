*****************************************************************************
* initialization the interrupt service routine for hfinal1.asm&sfinal1.asm
*****************************************************************************
		.global	_c_int00
		.sect 	".vectors"
RESET: 	B.S2 	_c_int00
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
NMI: 	B 		NRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
RESV1:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
RESV2:  B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
INT4:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
INT5:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP		
		NOP
INT6:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
INT7:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
INT8:	B		IRP
		NOP
		NOP
		NOP		
		NOP
		NOP
		NOP		
		NOP				
INT9:	B  		IRP	
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
INT10:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
INT11:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
INT12:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
INT13:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
INT14:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
INT15:	B		IRP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
    	.end
	