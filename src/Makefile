CXX = clang
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -g
CCFLAGS = -std=c11 -Wall -Wextra -Werror -g
TESTFLAGS = -lcheck -lgtest -lstdc++
LIBS = -lncurses -lstdc++
CXX_FILES_SNAKE = brick_game/snake/snake_model.cpp gui/cli/snake_cli.cpp brick_game/snake/main.cpp
C_FILES_TETRIS = brick_game/tetris/tetris.c gui/cli/cli.c brick_game/tetris/main.c
CXX_TEST_FILES = brick_game/snake/snake_model.cpp tests/snake_test.cpp
O_FILES = *.o
GCOVFLAGS = -fprofile-arcs -ftest-coverage

all: install dvi

dvi:
	doxygen

dist: install
	tar cvzf CPP_GAMES.tgz ../GAMES

build:
	cd .. && cmake -S . -B "src/build" --install-prefix=$(CURDIR)/../GAMES

install: build
	cd build && make install
	$(CXX) $(CCFLAGS) $(C_FILES_TETRIS) -o ../GAMES/Tetris_TERM $(LIBS)
	$(CXX) $(CXXFLAGS) $(CXX_FILES_SNAKE) -o ../GAMES/Snake_Term $(LIBS)

uninstall:
	rm -rf build
	rm -rf ../GAMES


s21_games.a:
	$(CXX) $(CXXFLAGS) -c $(CXX_TEST_FILES)
	ar rcs snake_model.a $(O_FILES)

test: s21_games.a
	$(CXX) $(CXXFLAGS) ./tests/snake_test.cpp snake_model.a -o test_s21_games $(TESTFLAGS)
	./test_s21_games

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf vgcore.*
	rm -rf gcov_report
	rm -rf report
	rm -rf test_s21_calc*
	rm -rf *.info
	rm -rf *.gcno
	rm -rf *.gcda
	rm -rf test_s21_games
	rm  -rf report
	rm -rf doxygen

gcov_report: clean
	g++ $(CXXFLAGS) --coverage $(CXX_TEST_FILES) -o gcov_report $(TESTFLAGS) $(GCOVFLAGS)
	./gcov_report
	lcov -t "gcovreport" -o gcovreport.info -c -d .  --ignore-errors gcov --ignore-errors mismatch
	lcov --remove gcovreport.info "/usr/*" "/usr/local/include/*" "/14/*" -o gcovreport_filtered.info --ignore-errors unused
	genhtml -o report gcovreport_filtered.info
	rm -rf gcov_report*
	rm -rf *.info
	rm -rf *.gcno
	rm -rf *.gcda
	rm -rf *.o

clang_format:
	clang-format -i -style=file $(CXX_FILES_SNAKE) $(C_FILES_TETRIS) $(CXX_TEST_FILES)
