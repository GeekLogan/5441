default:
	gcc -O3 -DENABLE_PAPI -lpapi -ftree-vectorizer-verbose=2 pa1-p1.c >& pa1-p1.gccvecrpt
	mv a.out pa1-p1.gcc
	icc -O3 -DENABLE_PAPI -lpapi -qopt-report=2 -qopt-report-phase=vec pa1-p1.c
	mv a.out pa1-p1.icc
