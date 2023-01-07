CC = i686-w64-mingw32-g++

COBRA_SRCS = Engine/src/Cobra/Window.cpp Engine/src/Cobra/Object.cpp Engine/src/Cobra/LuaFunctions.cpp Engine/src/Cobra/General.cpp

COBRA_LIBS = -lglfw3 -lopengl32 -lgdi32 Engine/bin/lua54.dll
COBRA_INCLUDES = -I"Engine/src/"

RUNTIME_SRCS = Runtime/src/Runtime.cpp
RUNTIME_INCLUDES = -I"Engine/"
RUNTIME_LIBS = Runtime/test/Cobra.dll Engine/bin/lua54.dll

main:
	$(CC) -std=c++17 $(COBRA_SRCS) -shared $(COBRA_INCLUDES) -o Engine/bin/Cobra.dll $(COBRA_LIBS) -O3
	copy Engine\bin\Cobra.dll Runtime\test\Cobra.dll
	$(CC) $(RUNTIME_SRCS) $(RUNTIME_INCLUDES) -o Runtime/test/Runtime.exe $(RUNTIME_LIBS) -O3
