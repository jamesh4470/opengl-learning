output:
	g++ main.cpp glad/src/glad.c \
		-I glad/include \
		-I glfw-3.4/include \
		-L -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl \
		-o output
