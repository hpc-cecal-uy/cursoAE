include ../environment

OBJS = States.o netstream.o

all: $(OBJS)

States.o: States.cc States.hh
	$(CXX) $(CPPFLAGS) States.cc -c

netstream.o: netstream.cc netstream.hh
	$(CXX) $(CPPFLAGS) netstream.cc -c
	
clean:
	rm -f *.o *~ *% 
