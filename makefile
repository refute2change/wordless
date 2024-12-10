build: compile link clean

compile:
	g++ -c *.cpp -I".\Externals\include" -DSFML_STATIC

link:
	g++ *.o -o wordless -L".\Externals\lib" -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lvorbisfile -lvorbis -lvorbisenc -lopengl32 -lopenal32 -lfreetype -lwinmm -lgdi32 -mwindows -lflac -logg

clean:
	rm *.o

debugcompile:
	g++ -c -g *.cpp -I".\Externals\include" -DSFML_STATIC

debug: debugcompile link clean
	gdb -x debug.gdb wordless.exe