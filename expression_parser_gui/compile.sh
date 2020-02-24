gcc -c tinyexpr.c -ansi -Wall -Wshadow -O2 -o tinyexpr.o -lm &&
g++ -c pch.cpp -o pch.o &&
g++ -DIMGUI_USER_CONFIG=\"imconfig_allegro5.h\" -fpermissive main.cpp expr_parser.cpp imgui/imgui*.cpp pch.o tinyexpr.o -L /usr/lib -lallegro -lallegro_main -lallegro_primitives -o expression_parser