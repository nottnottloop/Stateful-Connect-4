cmake_minimum_required (VERSION 3.12)
set (CMAKE_CXX_STANDARD 17)

project ("Connect 4")

set (SOURCES
	src/main.cpp
	src/renderwindow.cpp
	src/entity.cpp
	src/game.cpp
	src/introstate.cpp
	src/playingstate.cpp
	src/text.cpp
)

add_executable (Connect4 ${SOURCES})
#add_executable (Connect4 WIN32 ${SOURCES})
set_target_properties(Connect4 PROPERTIES OUTPUT_NAME "Connect 4")

target_include_directories (Connect4 PUBLIC
	#C:/dev/Libraries/SDL2-2.0.16vc/include/SDL2
	"C:/dev/Libraries/SDL2-2.0.16gcc/x86_64-w64-mingw32/include/SDL2"
	include
)

target_link_directories (Connect4 PUBLIC
	#C:/dev/Libraries/SDL2-2.0.16/lib/x64
	"C:/dev/Libraries/SDL2-2.0.16gcc/x86_64-w64-mingw32/lib"
)

target_link_libraries(Connect4 mingw32 SDL2main SDL2 SDL2_image SDL2_ttf SDL2_mixer)