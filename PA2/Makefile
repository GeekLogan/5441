default:
	make buildp2
	make testp2

buildp1:
	gcc -O3 -DENABLE_PAPI -lpapi -fopenmp -ftree-vectorizer-verbose=5 pa2-p1.c >& pa2-p1.gccvecrpt
	mv a.out pa2-p1.gcc
	icc -O3 -DENABLE_PAPI -lpapi -qopenmp -qopt-report=5 -qopt-report-phase=vec pa2-p1.c
	mv a.out pa2-p1.icc

buildp2:
	gcc -O3 -g -DENABLE_PAPI -lpapi -fopenmp -ftree-vectorizer-verbose=5 pa2-p2.c >& pa2-p2.gccvecrpt
	mv a.out pa2-p2.gcc
	icc -O3 -DENABLE_PAPI -lpapi -qopenmp -qopt-report=5 -qopt-report-phase=vec pa2-p2.c
	mv a.out pa2-p2.icc

clean:
	rm *.icc
	rm *.gcc

testp1:
	./pa2-p1.gcc 6
	./pa2-p1.icc 6

testp2:
	./pa2-p2.gcc 12
	./pa2-p2.icc 12
