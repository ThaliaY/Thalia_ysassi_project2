main: thalia_ysassi_project2.o
	g++ thalia_ysassi_project2.o -o main

thalia.ysassi-project2.o: thalia_ysassi_project2.cpp
	g++ -c thalia_ysassi_project2.cpp

clean:
	rm *.o main
