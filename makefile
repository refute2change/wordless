build: staticcompile staticlink clean

compile:
	g++ -c *.cpp -I".\Externals\include"

staticcompile:
	g++ -c *.cpp -I".\Externals\include" -DSFML_STATIC

link:
	g++ *.o -o wordless -L".\Externals\lib" -lsfml-graphics -lsfml-window -lsfml-system
	
staticlink:
	g++ *.o -o wordless -L".\Externals\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows -lflac -logg

clean:
	del /Q *.o

debugcompile:
	g++ -c -g *.cpp -I".\Externals\include" -DSFML_STATIC

debug: debugcompile link clean
	gdb -x debug.gdb wordless.exe