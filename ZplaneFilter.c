while(1) {
//	x3 = x2;
	x2 = x1;
	x1 = x0;
	x0 = A2D_Read(0);
	
//	y3 = y2;
	y2 = y1;
	y1 = y0;
	y0 = 1.93868*y1 + 0.291405*x1 - 0.961*y2 + 0.291405*x2;
	
	D2A(y0);
	Wait_T(); // set the sampling rate, presumably with interrupts
}