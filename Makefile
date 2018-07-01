cc = gcc

WIN_LINKER_FLAGS= \
	-lsoil2 \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-mwindows \
	-lopengl32 \

LIN_LINKER_FLAGS= \
	-lSOIL \
	-lSDL2 \
	-lGL \
	-lGLU \


BIN = bin
OUTPUT = devorio


all:
	echo building for $(OS)
	ifeq ($(OS),Windows_NT)
		make windows
	endif
	ifeq ($(OS),Linux)
		make linux
	endif

windows:
	$(cc) -o $(BIN)/$(OUTPUT).exe lib/libSOIL.a src/devorio.c  -Iinclude -Llib -Wl,--subsystem,windows -Dmain=SDL_main $(WIN_LINKER_FLAGS)

linux:
	$(cc) -o $(BIN)/$(OUTPUT) lib/libSOIL.a src/devorio.c  -Iinclude -Llib $(LIN_LINKER_FLAGS)

run:
	ifeq ($(OS),Windows_NT)
		$(BIN)/$(OUTPUT).exe
	endif
	ifeq ($(OS),Linux)
		$(BIN)/$(OUTPUT)
	endif
