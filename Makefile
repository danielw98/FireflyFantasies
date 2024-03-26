build:
	-mkdir obj 2> /dev/null
	g++ -c -o obj/Board.o src/Board.cpp
	g++ -c -o obj/FontRenderer.o src/FontRenderer.cpp
	g++ -c -o obj/GlobalFunctions.o src/GlobalFunctions.cpp
	g++ -c -o obj/TextureManager.o src/TextureManager.cpp
	g++ -c -o obj/Firefly.o src/Firefly.cpp
	g++ -c -o obj/Game.o src/Game.cpp
	g++ -c -o obj/GUIManager.o src/GUIManager.cpp
	g++ -c -o obj/main.o main.cpp
	g++ -o FireflyFantasies obj/Board.o obj/FontRenderer.o obj/GlobalFunctions.o obj/TextureManager.o obj/Firefly.o obj/Game.o obj/GUIManager.o obj/main.o -lSDL2 -lSDL2_image -lSDL2_ttf -g -Wall -Wextra -pedantic

clean:
	-rm -r obj 2> /dev/null
	-rm FireflyFantasies 2> /dev/null
