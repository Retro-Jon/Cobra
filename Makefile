CC = g++

ENGINE_SRCS = dev/src/Engine/Application.cpp dev/src/Engine/General.cpp dev/src/Engine/LuaFunctions.cpp dev/src/Engine/Component.cpp

ENGINE_LIBS = -lglfw3 -lopengl32 -lgdi32
ENGINE_INCLUDES = -I"dev/src/"

SANDBOX_SRCS = Sandbox/src/App.cpp
SANDBOX_INCLUDES = -I"dev/src"
SANDBOX_LIBS = Sandbox/test/Engine.dll

main:
	$(CC) -std=c++17 $(ENGINE_SRCS) -shared $(ENGINE_INCLUDES) -o dev/bin/Engine.dll dev/bin/lua54.dll $(ENGINE_LIBS)
	copy dev\bin\Engine.dll Sandbox\test\Engine.dll
	$(CC) $(SANDBOX_SRCS) $(SANDBOX_INCLUDES) -o Sandbox/test/test.exe $(SANDBOX_LIBS) $(ENGINE_LIBS)
