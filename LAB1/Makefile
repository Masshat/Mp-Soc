OBJS    = fifo_gcd_coprocessor.o fifo_gcd_master.o tp1_top.o
CFLAGS  = -Wno-deprecated -I. 
LDFLAGS = -Wno-deprecated -L. 


all: simul.x

run: simul.x
	./simul.x 10000

simul.x: $(OBJS)
	g++ $(LDFLAGS) -o $@ $^ -lsystemc  | c++filt

%.o: %.cpp
	g++ $(CFLAGS) -c -o $@ $^

clean:
	rm -f *.o simul.x
