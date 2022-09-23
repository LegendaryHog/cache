# LFU cache on C++

Its my cache by algorithm LFU on C++. I made it with complexity O(1). You can test it:

**1. Test geneartion.**

```
cd test/
chmod "+x" testgen.py
./testgen.py [OUT_FILE] [NUMBER_OF_KEYS] [CACHE_CAPACITY] [OPT]
```
[OPT] - if any symbols written - dont make file with prefix for name `answ_*`, else - make and write number of hits in this file

**2. Test answers**
```
chmod "+x" test_answ
./test_answ -c [CACHE_EXE] [TESTFILE1] [TESTFILE2] ...
```

**3. Test complexity.**
```
chmod "+x" test_compl
./test_compl -c [CACHE_EXE] [BIGTESTFILE1] [BIGTESTFILE2] ...
```

**4. How to run?**


Compiling:
```
cmake -S . -B build/
cd build/
make
```
Run:
```
./cache
```
