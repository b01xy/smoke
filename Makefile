CFLAGS=-I/usr/include/GL

smoke: draw.o ogl2particle.o shader.o
	$(CC) -lGL -lGLU -lglut $^ -o $@

