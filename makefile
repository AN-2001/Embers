cc := g++
flags :=  -Wall -I. -I./glad         \
		  -I./math -DEMBERS_DEBUG -g

libs := -lglfw -lGL -lX11  \
		-lpthread -lXrandr \
		-lXi -ldl -lm

obj := main.o      \
	   glad/glad.o \
	   embers.o    \
       shader.o    \
	   program.o   \
	   math/vec3.o \
	   math/mat4.o \
	   image.o     \
	   chess.o     \

proj := embers
all: $(proj)

$(proj): errors.h config.h $(obj)
	$(cc) $(obj) $(flags) $(libs) -o $(proj)

%.o: %.cpp %.h config.h
	$(cc) -c $(flags) $< -o $@

%.o: %.cpp 
	$(cc) -c $(flags) $< -o $@

clean:
	rm $(obj) $(proj)
