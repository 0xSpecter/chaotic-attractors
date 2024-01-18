CXX = clang++ -std=c++11
CFLAGS = -O -I./include -I./include/**  -Wall -Wno-unused 
FRAMEWORKS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

all: 
	@$(CXX) $(CFLAGS) ./src/*.cpp ./src/**/*.cpp -l glfw3 -L./lib $(FRAMEWORKS) -o main
	@./main
	@rm -f main

clean:
	@rm -f main imgui.ini