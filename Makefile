all:
	g++ Source/*.cpp -o hw3 -std=c++11 -O3 -lXi -lGLEW -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -L/usr/X11/lib -I/usr/X11/include
alperen:
	g++ Source/*.cpp -o hw3 -std=c++11 -O3 -lglfw -lglew -framework OpenGL
hakan:
	g++ Source/*.cpp -o hw3 -std=c++11 -O3 -lXi -lGLEW -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lglfw -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -L/usr/X11/lib -I/usr/X11/include


