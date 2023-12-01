
.PHONY: compile clean run

compile: program.x

run: program.x
	./$< < input.txt > output.txt && cat output.txt

clean:
	rm -f *.x

program.x: source.cpp Makefile
	g++ -g -O2 $< -o $@

