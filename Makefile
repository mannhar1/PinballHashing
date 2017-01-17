all: Pinball.o driver.o
	g++ -Wall Pinball.cpp driver.cpp -o driver.out

Pinball.o: Pinball.cpp
	g++ -Wall -c Pinball.cpp



myDriver.o: myDriver.cpp
	g++ -Wall -c myDriver.cpp

driver.o: driver.cpp
	g++ -Wall -c driver.cpp


clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#

run:
	./driver.out

valgrind:
	valgrind driver.out
