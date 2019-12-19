all: viewer


data.o: data.cpp 
	g++ -std=c++0x -g -c data.cpp
viewer: viewer.o modflow.o visit_writer.o data.o
	g++ -std=c++0x -g viewer.o modflow.o visit_writer.o data.o -o viewer -I ./

viewer.o: viewer.cpp
	g++ -std=c++0x -g -c viewer.cpp -I ./

modflow.o: modflow.cpp data.o
	g++ -std=c++0x -g -c modflow.cpp -I ./

visit_writer.o: visit_writer.c
	gcc -std=c++0x -g -c visit_writer.c -I ./

clean:
	rm *o viewer