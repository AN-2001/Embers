cc := g++
flags :=  -Wall -Werror -I. -I./glad         \
		  -I./math  -I./core -I./io          \
		  -I./core/glad #-DEMBERS_DEBUG -g

libs := -lglfw -lGL -lX11  \
		-lpthread -lXrandr \
		-lXi -ldl -lm

obj := main.o           \
	   core/glad/glad.o \
	   embers.o         \
       core/shader.o    \
	   core/program.o   \
	   math/vec3.o      \
	   math/mat4.o      \
	   io/image.o       \
	   chess.o

proj := embers
all: $(proj)

$(proj): ./core/errors.h config.h $(obj)
	$(cc) $(obj) $(flags) $(libs) -o $(proj)

%.o: %.cpp %.h config.h
	$(cc) -c $(flags) $< -o $@

%.o: %.cpp 
	$(cc) -c $(flags) $< -o $@

clean:
	rm $(obj) $(proj)
