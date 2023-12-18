
.PHONY: compile clean run

compile: task1.x task2.x

run: compile
	./task1.x < input.txt
	./task2.x < input.txt

clean:
	rm -f *.x

%.x: %.cpp Makefile ../helpers/helpers.hpp
	g++ -g -O2 $< -o $@

