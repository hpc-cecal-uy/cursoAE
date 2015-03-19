include ../../../environment

all: MainSeq MainLan 

clean:
	rm -f MainLan MainSeq *.o *% *~

MainLan: SA.req.o SA.pro.o StopCondition.o MainLan.o OverlapDetector.o sga.o AdjKeys.o ContigBuilder.o ConsensusBuilder.o LayoutBuilder.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@ 

MainWan: SA.req.o SA.pro.o StopCondition.o MainWan.o OverlapDetector.o sga.o AdjKeys.o ContigBuilder.o ConsensusBuilder.o LayoutBuilder.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@ 

MainSeq: SA.req.o SA.pro.o StopCondition.o MainSeq.o OverlapDetector.o sga.o AdjKeys.o ContigBuilder.o ConsensusBuilder.o LayoutBuilder.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@ 

MainScore:  SA.req.o SA.pro.o StopCondition.o MainScore.o OverlapDetector.o sga.o AdjKeys.o ContigBuilder.o ConsensusBuilder.o LayoutBuilder.o
	$(CXX)  $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

LAN: 	
	$(RUN)  -v  -p4pg pgfileLan MainLan
WAN:
	$(RUN)  -v  -p4pg pgfileWan MainWan
SEQ:
	./MainSeq SA.cfg ../../../ProblemInstances/DNAFA-instances/pbm.txt res/dna.sa.seq.txt
