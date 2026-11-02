.PHONY: all run test test-asan clean

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include
GTEST = -isystem googletest/googletest/include -I googletest/googletest -pthread googletest/googletest/src/gtest-all.cc
TESTS = tests/run_tests.cpp $(wildcard tests/tests_*.cpp)

all: test

lab: src/main.cpp
	$(CXX) $(CXXFLAGS) $< -o lab

run: lab
	./lab

test: $(TESTS)
	$(CXX) $(CXXFLAGS) $(GTEST) $^ -o run_tests
	./run_tests

test-asan: $(TESTS)
	clang++ $(CXXFLAGS) -fsanitize=address,undefined -g -O1 -fno-omit-frame-pointer $(GTEST) $^ -o run_tests_asan
	./run_tests_asan

clean:
	rm -f lab run_tests run_tests_asan

