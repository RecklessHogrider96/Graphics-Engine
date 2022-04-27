# set the C compiler, for C++ use g++
# use this setting on most sane machines
CC = gcc

# use this line on the engin linux machines
# CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -I/opt/local/include -O2 -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes -Wmissing-declarations
CPPFLAGS = $(CFLAGS)

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =../lib

# path to the bin directory
BINDIR =../bin

# libraries to include
LIBS = -lm -limageIO
LFLAGS = -L$(LIBDIR) -L/opt/local/lib

# put all of the relevant include files here
_DEPS = ppmIO.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of the executables here
EXECUTABLES = ppmtest

# put a list of all the object files here for all executables (with .o endings)
_OBJ = tiffmain.o ppmmain.o

# convert them to point to the right place
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# patterns for compiling source code
# $< is the file that caused the action to occur
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


all:
	for i in $(EXECUTABLES); do (make $$i) ; done


# put the specific executable rules here
ppmtest: $(ODIR)/ppmmain.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

lab1: $(ODIR)/lab1.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

createMask: $(ODIR)/createMask.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

mergeTwo: $(ODIR)/mergeTwo.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
	
imagetest: $(ODIR)/imagetest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

mandeltest: $(ODIR)/mandeltest.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

mandelbrotDriver: $(ODIR)/mandelbrotDriver.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

juliaDriver: $(ODIR)/juliaDriver.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
	
perlinNoiseDriver: $(ODIR)/perlinNoiseDriver.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

perlinNoise: $(ODIR)/perlinNoise.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test3a: $(ODIR)/test3a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test3b: $(ODIR)/test3b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test3c: $(ODIR)/test3c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test3d: $(ODIR)/test3d.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

testbench: $(ODIR)/testbench.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

drawScene: $(ODIR)/drawScene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

drawStuff: $(ODIR)/drawStuff.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test3ddotted: $(ODIR)/test3ddotted.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

polyspeed: $(ODIR)/polyspeed.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test4a: $(ODIR)/test4a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test4b: $(ODIR)/test4b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test4c: $(ODIR)/test4c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test4c-loop: $(ODIR)/test4c-loop.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5a: $(ODIR)/test5a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5b: $(ODIR)/test5b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5c: $(ODIR)/test5c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test5d: $(ODIR)/test5d.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

2DScene_R2D2: $(ODIR)/2DScene_R2D2.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

2DScene: $(ODIR)/2DScene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

3DScene: $(ODIR)/3DScene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6a: $(ODIR)/test6a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p6_cube: $(ODIR)/p6_cube.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test6b: $(ODIR)/test6b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p6_3D_scene: $(ODIR)/p6_3D_scene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p6_3D_scene2: $(ODIR)/p6_3D_scene2.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)
	
p6_3D_scene3: $(ODIR)/p6_3D_scene3.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p6_3D_scene4: $(ODIR)/p6_3D_scene4.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7a: $(ODIR)/test7a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7b: $(ODIR)/test7b.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test7c: $(ODIR)/test7c.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p7_cylinder_scene: $(ODIR)/p7_cylinder_scene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p7_flatcircle_scene: $(ODIR)/p7_flatcircle_scene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p7_cone_scene: $(ODIR)/p7_cone_scene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p7_tetrahedron_scene: $(ODIR)/p7_tetrahedron_scene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p7_3D_scene: $(ODIR)/p7_3D_scene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p7_teapot_scene: $(ODIR)/p7_teapot_scene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p7_3D_scene2: $(ODIR)/p7_3D_scene2.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p7_ellipsoid_scene: $(ODIR)/p7_ellipsoid_scene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p7_3D_scene3: $(ODIR)/p7_3D_scene3.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

test8a: $(ODIR)/test8a.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

cubism: $(ODIR)/cubism.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p8_3D_scene: $(ODIR)/p8_3D_scene.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

p8_3D_scene2: $(ODIR)/p8_3D_scene2.o
	$(CC) -o $(BINDIR)/$@ $^ $(LFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

