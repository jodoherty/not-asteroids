cmake_minimum_required(VERSION 2.8)
project(asteroids)

set(SOURCES
	src/main.cpp
	src/Game.cpp
	src/SDLAudioService.cpp
	src/Asteroid.cpp
	src/Bullet.cpp
	src/Ship.cpp
	src/BitmapFont.c
)

set(HEADERS
	src/Common.h
	src/Resources.h
	src/Game.h
	src/Geometry.h
	src/Vector2d.h
	src/AudioService.h
	src/SDLAudioService.h
	src/Sprite.h
	src/MirroredSprite.h
	src/Asteroid.h
	src/Bullet.h
	src/Ship.h
	src/BitmapFont.h
)

if(MSVC)
	set(PLATFORM_C_FLAGS "/W3 /MD /O2 /EHsc /DUNICODE /DWIN32 /D_CRT_SECURE_NO_WARNINGS")
	set(PLATFORM_C_FLAGS_DEBUG "/W3 /MDd /Zi /Od /EHsc /DUNICODE /DWIN32 /D_CRT_SECURE_NO_WARNINGS")
	set(PLATFORM_CXX_FLAGS "${PLATFORM_C_FLAGS}")
	set(PLATFORM_CXX_FLAGS_DEBUG "${PLATFORM_C_FLAGS_DEBUG}")
else()
	set(PLATFORM_CXX_FLAGS " -std=c++11 -O2 ")
	set(PLATFORM_CXX_FLAGS "${PLATFORM_CXX_FLAGS} -g ")
endif()

set(CMAKE_C_FLAGS "${PLATFORM_C_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${PLATFORM_C_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${PLATFORM_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS "${PLATFORM_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${PLATFORM_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${PLATFORM_CXX_FLAGS_DEBUG}")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

include_directories(
	${SDL_INCLUDE_DIR}
	${OPENGL_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
)

link_libraries(
	${OPENGL_LIBRARY}
	${SDL_LIBRARY}
	${SDLMIXER_LIBRARY}
	${GLEW_LIBRARY}
)
add_executable(asteroids WIN32 MACOSX_BUNDLE ${SOURCES})

