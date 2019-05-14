RDAgent: RDAgent.o RDControl.o
		g++ -pthread -o RDAgent RDAgent.o RDControl.o
RDControl.o: RDControl.cpp RDControl.h VectorMatrix.h random.h
		g++ -c -O3 -flto RDControl.cpp
random.o: random.cpp random.h VectorMatrix.h
		g++ -c -O3 -flto random.cpp
RDAgent.o: RDAgent.cpp VectorMatrix.h RDControl.h TSearch.h
		g++ -c -O3 -flto RDAgent.cpp
clean:
		rm *.o RDAgent
