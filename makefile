RDTask: RDAgent.o AgentInterface.o RDControl.o random.o
	g++ -pthread -o RDTask RDTask.cpp	
RDAgenti.o: RDAgent.cpp RDAgent.h AgentInterface.h VectorMatrix.h VisualObject.h random.h
		g++ -c -O3 RDAgent.cpp
AgentInterface.o: AgentInterface.cpp AgentInterface.h RDControl.h VisualObject.h
		g++ -c -O3 -flto AgentInterface.cpp
RDControl.o: RDControl.cpp RDControl.h VectorMatrix.h random.h
		g++ -c -O3 -flto RDControl.cpp
random.o: random.cpp random.h VectorMatrix.h
		g++ -c -O3 -flto random.cpp
RDAgent.o: RDAgent.cpp VectorMatrix.h RDControl.h TSearch.h
		g++ -c -O3 -flto RDAgent.cpp
clean:
		rm *.o RDAgent
