output:
	g++ main.cpp glad/src/glad.c \
		-I glad/include \
		-I glfw-3.4/include \
		-Lglfw-3.4/build/src -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl \
		-o output
