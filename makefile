all:
	g++ *.cpp -g -o x -Wall
valgrind: 
	valgrind --track-origins=yes --leak-check=full ./x
clean:
	rm x
