include ../../../environment
       
all: MainSeq MainLan MainWan

clean:
	rm -f MainLan MainSeq MainWan *.o *% *~

MainLan: SA.req.o SA.pro.o StopCondition.o MainLan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@ 

MainWan: SA.req.o SA.pro.o StopCondition.o MainWan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@ 

MainSeq: SA.req.o SA.pro.o StopCondition.o MainSeq.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@ 

LAN: 	
	$(RUN)  -v  -p4pg pgfileLan MainLan
WAN: 	
	$(RUN) -v  -p4pg pgfileWan MainWan
SEQ:
	./MainSeq SA.cfg ../../../ProblemInstances/instance.txt res/res.txt
