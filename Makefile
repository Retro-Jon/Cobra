CC = g++

COBRA_SRCS = Engine/src/Cobra/Window.cpp Engine/src/Cobra/Input.cpp Engine/src/Cobra/Component.cpp

COBRA_LIBS = -lglfw3 -lopengl32 -lgdi32
COBRA_INCLUDES = -I"Engine/src/"

RUNTIME_SRCS = Runtime/src/Runtime.cpp
RUNTIME_INCLUDES = -I"Engine/"
RUNTIME_LIBS = Runtime/test/Cobra.dll

main:
	$(CC) -std=c++17 $(COBRA_SRCS) -shared $(COBRA_INCLUDES) -o Engine/bin/Cobra.dll Engine/bin/lua54.dll $(COBRA_LIBS)
	copy Engine\bin\Cobra.dll Runtime\test\Cobra.dll
	$(CC) $(RUNTIME_SRCS) $(RUNTIME_INCLUDES) -o Runtime/test/Runtime.exe $(RUNTIME_LIBS) $(COBRA_LIBS)
