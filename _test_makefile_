RDControlMain: RDControlMain.o RDControl.o
		g++ -pthread -o RDControlMain RDControlMain.o RDControl.o
RDControl.o: RDControl.cpp RDControl.h VectorMatrix.h random.h
		g++ -c -O3 -flto RDControl.cpp
random.o: random.cpp random.h VectorMatrix.h
		g++ -c -O3 -flto random.cpp
RDControlMain.o: RDControlMain.cpp
		g++ -c -O3 -flto RDControlMain.cpp
clean:
		rm *.o RDControlMain
