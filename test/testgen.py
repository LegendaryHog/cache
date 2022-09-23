#!/usr/bin/env python3

from io import TextIOWrapper
from typing import List
import random, sys
from pathlib import Path
import os 

class arr_int:
    size: int = 0
    arr: List[int] = []
    def __init__(self, sz: int):
        self.size = sz
        for i in range (self.size):
            self.arr.append(0)


def generate_test(quantity_of_keys: int):
    keys_arr: arr_int = arr_int(quantity_of_keys)
    for i in range(keys_arr.size):
        keys_arr.arr[i] = random.randint(1, max(5, int(quantity_of_keys/16)))
    return keys_arr

#for simplification data an key is the same
class _cache_elem_:
    key: int = 0
    counter: int = 0
    def __init__(self, key: int, counter: int):
        self.key = key
        self.counter = counter

class _cache_:
    size: int = 0
    cache: List[_cache_elem_] = []

    def __init__(self, sz: int):
        self.size = sz

    def find(self, key: int) -> int:
        hit: int = -1 
        for i in range(len(self.cache)):
            if self.cache[i].key == key:
                hit = i
                return hit
        return hit
    
    def read_size(self) -> int:
        return self.size

    def full(self) -> bool:
        if self.size == len(self.cache):
            return True
        return False

    def increment_pp_counter(self, ind: int):
        self.cache[ind].counter += 1

    def counter(self, ind: int) -> int:
        return self.cache[ind].counter

    def lookup_update(self, key: int) -> bool:
        hit = self.find(key)

        if hit == -1: #if not found
            #if full pop from back
            if self.full() == True:
                self.cache.pop()
            #push in back
            self.cache.insert(self.size, _cache_elem_(key, 1))
            return False
        else: #if found
            self.increment_pp_counter(hit)
            if hit != 0:
                place = hit
                #find the laastest place for replace
                while place > 0 and self.counter(place - 1) < self.counter(hit):
                    place -= 1
                #insert in new place
                self.cache.insert(place, self.cache[hit])
                #delete duplicate 
                self.cache.pop(hit + 1)
            return True

def generate_answer (keys_arr: arr_int, cache_capacity: int):
    cache = _cache_(cache_capacity)
    hits: int = 0
    for i in range(keys_arr.size):
        if cache.lookup_update(keys_arr.arr[i]) == True:
            hits += 1
    return cache, hits

def print_in_file_test (f: TextIOWrapper, keys_arr: arr_int, capacity: int):
    f.write(str(capacity))
    f.write('\n\n')
    f.write(str(keys_arr.size))
    f.write('\n\n')
    for i in range(keys_arr.size):
        f.write(str(keys_arr.arr[i]))
        f.write(' ')
    f.write('\n\n')
    
def print_in_file_answ (f: TextIOWrapper, cache: _cache_, hits: int):
    f.write(str(hits))

def main():
    quantity_of_keys: int = max(10, int(sys.argv[2]))
    keys_arr: arr_int = generate_test(quantity_of_keys)
    
    path, file_name_test = os.path.split(sys.argv[1])
    if (len(path) != 0):
        path += '/'
    f_test: TextIOWrapper = open(path + file_name_test, 'w')

    print_in_file_test(f_test, keys_arr, max(int(quantity_of_keys/32), 4))
    f_test.close()
     
    if len(sys.argv) <= 4:
        file_name_answ: str = 'answ_' + file_name_test
        f_answ: TextIOWrapper = open(path + file_name_answ, 'w')
        cache, hits = generate_answer(keys_arr, max(int(quantity_of_keys/32), 4))
        print_in_file_answ(f_answ, cache, hits)
        f_answ.close()

main()
    


