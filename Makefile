all:
	g++ -g -o main.exe -I.\\include -L.\\lib src/*.cpp src/*.c -lglfw3 -lgdi32 -lopengl32
	./main.exe

build:
	g++ -g -o main.exe -I.\\include -L.\\lib src/*.cpp src/*.c -lglfw3 -lgdi32 -lopengl32
