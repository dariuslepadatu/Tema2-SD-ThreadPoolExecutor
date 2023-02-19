build:
	gcc -g tema2.c -o tema2 -Wall
check: build
	valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./tema2 _test/input/test24.in _test/output/test24.out
run:
	./tema2
clean:
	rm -rf tema2