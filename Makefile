CFLAGS=-I/usr/include/GL

.PHONY=clean

smoke: draw.o ogl2particle.o shader.o
	$(CC) -lGL -lGLU -lglut $^ -o $@

clean:
	rm -f smoke *.o
