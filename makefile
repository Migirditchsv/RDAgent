#CXXFLAGS = -Og -g

RDTask: RDAgent.o AgentInterface.o RDControl.o random.o
	g++ -pthread -o RDTask RDTaskMain.cpp RDAgent.o AgentInterface.o RDControl.o random.o
RDTaskMain.o: RDTaskMain.cpp TSearch.h RDAgent.h VectorMatrix.h random.h
		g++ -c -Og -g -O3 RDTaskMain.cpp
RDAgent.o: RDAgent.cpp RDAgent.h AgentInterface.h VectorMatrix.h VisualObject.h random.h
		g++ -c -Og -g -O3 RDAgent.cpp
AgentInterface.o: AgentInterface.cpp AgentInterface.h RDAgent.h VectorMatrix.h VisualObject.h random.h
		g++ -c -Og -g -O3 -flto AgentInterface.cpp
RDControl.o: RDControl.cpp RDControl.h VectorMatrix.h random.h
		g++ -c -Og -g -O3 -flto RDControl.cpp
random.o: random.cpp random.h VectorMatrix.h
		g++ -c -Og -g -O3 -flto random.cpp
clean:
		rm *.o RDTask