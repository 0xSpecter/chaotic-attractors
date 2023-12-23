CC = clang -std=c11
CXX = clang++ -std=c++11
CFLAGS = -g -O -I../include -I../include/** -L../lib -Wall -Wno-unused
	
all: 
	@$(CXX) $(CFLAGS) -c ./*.cpp  
	@$(CC) $(CFLAGS) -pedantic-errors -O3 -c glad.c -o glad.o
	@$(CXX) $(CFLAGS) main.o glad.o -l glfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -o main
	@./main
	@rm -f main.o glad.o main

c:
	@rm -f main.o glad.o main 