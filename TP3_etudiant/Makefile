CC = soclib-cc

all: simul.x

giet:
	(cd soft && $(MAKE))

giet-clean:
	(cd soft && $(MAKE) clean)

simul.x: tp3_top.desc tp3_top.cpp giet
	soclib-cc -t systemcass  -p tp3_top.desc -o $@
run:

	./simul.x
clean: giet-clean
	rm -f simul.x *~ *.o term0
