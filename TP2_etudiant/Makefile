SRCS_CPP=vci_gcd_master.cpp \
	 vci_gcd_coprocessor.cpp \
	 mapping_table.cpp \
	 segment.cpp \
	 address_decoding_table.cpp \
	 vci_vgsb.cpp \
	 address_masking_table.cpp 

OBJS=$(SRCS_CPP:.cpp=.o)

SRCS_CPP+=tp2_multi_top.cpp
OUTPUTF =simul.x

CFLAGS  =-Wno-deprecated -fpermissive
LFLAGS  =-ldl



all : simul 

%.o : %.cpp
	g++ $(CFLAGS) -I. -I/opt/systemcass/include -c -o $@ $<

simul : $(OBJS)
	g++ $(CFLAGS) $(LFLAGS) -L. -L/opt/systemcass/lib-linux64 -o $(OUTPUTF) $(OBJS) -lsystemc 2>&1 | c++filt

run : simul
	LD_LIBRARY_PATH=/opt/systemcass/lib-linux ./simul.x 10000
clean : 
	rm -f *.o simul.x
