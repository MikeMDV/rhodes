race : MarvenFinalDragRace.o rcar.o
	g++ -g rcar.o MarvenFinalDragRace.o -o race

rcar.o : rcar.cpp rcar.h
	g++ -g -c -std=c++0x rcar.cpp

MarvenFinalDragRace.o : MarvenFinalDragRace.cpp rcar.h
	g++ -g -c -std=c++0x MarvenFinalDragRace.cpp
