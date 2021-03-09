all:
	g++ main.cpp tinyfiledialogs/tinyfiledialogs.c -o main -lSDL2 -lSDL2_image
clean:
	rm main
win:
	x86_64-w64-mingw32-g++ main.cpp tinyfiledialogs/tinyfiledialogs.c -o main.exe -lSDL2 -lSDL2_image -Wl,-subsystem,windows -municode -lcomctl32
clean_win:
	rm main.exe
