
mono : tp4_top.desc tp4_top.cpp
	soclib-cc  -t systemcass -j4 -P -p tp4_top.desc -o simulator.x

multi : tp4_top_multi.desc tp4_top_multi.cpp
	soclib-cc -t systemcass -j4 -m debug -P -p tp4_top_multi.desc -o simulator_multi.x

run2 :
	./simulator_multi.x -NPROCS 2 -IOCFILE images.raw

run4 :
	./simulator_multi.x -NPROCS 4 -IOCFILE images.raw

clean:
	rm -rf repos *.o *.x core tmp *.deps stats trace simul* tty*
	soclib-cc -x -X -P -p tp4_top_multi.desc  -o simulator_multi.x

