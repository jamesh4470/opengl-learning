output: main.cpp shader_program.cpp
	g++ main.cpp shader_program.cpp glad/src/glad.c \
		-I glad/include \
		-I glfw-3.5.1/include \
		-Lglfw-3.5.1/build/src -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl \
		-o output

run:
	./output

clean:
	rm output
