default:
	make buildp1
	make buildp2
	make buildp3

	echo "See Other Commands"

buildp1:
	gcc -O3 -DENABLE_PAPI -lpapi -ftree-vectorizer-verbose=5 pa1-p1.c >& pa1-p1.gccvecrpt
	mv a.out pa1-p1.gcc
	icc -O3 -DENABLE_PAPI -lpapi -qopt-report=5 -qopt-report-phase=vec pa1-p1.c
	mv a.out pa1-p1.icc

buildp2:
	gcc -O3 -DENABLE_PAPI -lpapi -ftree-vectorizer-verbose=5 pa1-p2.c >& pa1-p2.gccvecrpt
	mv a.out pa1-p2.gcc
	icc -O3 -DENABLE_PAPI -lpapi -qopt-report=5 -qopt-report-phase=vec pa1-p2.c
	mv a.out pa1-p2.icc

buildp3:
	gcc -O3 -DENABLE_PAPI -lpapi -ftree-vectorizer-verbose=5 pa1-p3.c >& pa1-p3.gccvecrpt
	mv a.out pa1-p3.gcc
	icc -O3 -DENABLE_PAPI -lpapi -qopt-report=5 -qopt-report-phase=vec pa1-p3.c
	mv a.out pa1-p3.icc

clean:
	rm *.icc
	rm *.gcc

testp1:
	./pa1-p1.gcc
	./pa1-p1.icc

testp2:
	./pa1-p2.gcc
	./pa1-p2.icc

testp3:
	./pa1-p3.gcc
	./pa1-p3.icc
