# LFU cache on C++

Its my cache by algorythm LFU on C++. I made it with complexity O(1). You can test it:

1. Generate test on python:
    ```
    cd test/
    python3 testgen.py [directory for output and name of file with test] [number of keys] [if you dont need generate answer print there any symbols]
    ```

2. Complexity tests.

    For complexity test you should generate big tests (1 000 000 - 20 000 000 keys) without answers (or wait more thar one hour :D ).
    For test complexity open directory `compl_test/` and open Makefile and print path to file with test next to `./test.out`. After this just print `make` in command string.

    Also you can just compile and run program without `make`. Print `make compile` after this print:
    ```
    ./test.out [names of file with geenrated tests]
    ```

3. Hits tests.

    For test hits you should generate small tests (less than 50 000) with answers. `testgen.py` will generate by test file with given name and file with answers in same directory (name of file has prefix `answ_`). After this open directory `hits_test` and open `Makefile`, print pair of files next to `./test.out`: file with test and file with answers. Print make. Or run by yourself.

4. How run?
    ```
    cd build
    make
    ./cache
    ```
    Good luck.

