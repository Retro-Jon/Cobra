CC = i686-w64-mingw32-g++

COBRA_SRC_DIR = Engine/src/Cobra/
COBRA_SRC = $(wildcard $(COBRA_SRC_DIR)*.cpp) $(wildcard $(COBRA_SRC_DIR)*.c)

COBRA_LIBS = -lglfw3 -lopengl32 -lgdi32 Engine/bin/lua54.dll
COBRA_INCLUDES = -I"Engine/src/"

RUNTIME_SRC = Runtime/src/Runtime.cpp
RUNTIME_INCLUDES = -I"Engine/"
RUNTIME_LIBS = Runtime/test/Cobra.dll

main:
	$(CC) -g -std=c++17 $(COBRA_SRC) -shared $(COBRA_INCLUDES) -o Engine/bin/Cobra.dll $(COBRA_LIBS)
	copy Engine\bin\Cobra.dll Runtime\test\Cobra.dll
	$(CC) -g $(RUNTIME_SRC) $(RUNTIME_INCLUDES) -o Runtime/test/Runtime.exe $(RUNTIME_LIBS)
