default:
	gcc -O3 -ftree-vectorizer-verbose=2 vectest.c >& filename.gccvecrpt
	icc -O3 -qopt-report=2 -qopt-report-phase=vec vectest.c

