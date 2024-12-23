build: compile link clean

compile:
	g++ -c *.cpp -I".\Externals\include" -DSFML_STATIC

link:
	g++ *.o -o wordless -L".\Externals\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows

clean:
	rm *.o

debugcompile:
	g++ -c -g *.cpp -I".\Externals\include" -DSFML_STATIC

debug: debugcompile link clean
	gdb -x debug.gdb wordless.exe