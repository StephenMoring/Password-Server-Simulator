proj5.x : passserver.o proj5.o
	g++ -std=c++11 -lcrypt -o proj5.x passserver.o proj5.o

proj5.o : proj5.cpp passserver.o
	g++ -std=c++11 -c passserver.o proj5.cpp

passserver.o : passserver.cpp
	g++ -std=c++11 -c passserver.cpp

make clean :
	rm *.o
	rm proj5.x
