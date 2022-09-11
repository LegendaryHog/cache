#!usr/bin/python

from typing import List
import random
import sys

MAX_KEY = 128
MIN_QUAN = 32
MAX_QUAN = 512

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
        keys_arr.arr[i] = random.randint(1, MAX_KEY)
    return keys_arr

#for simplification data an key is the same
class _cache_elem_:
    key: int = 0
    counter: int = 0
    def __init__(self, key: int, counter: int):
        self.key = key
        self.counter = counter

class _cache_:
    capacity: int = 0
    size: int = 0
    cache: List[_cache_elem_] = []

    def __init__(self, cap: int):
        self.capacity = cap
        for i in range(self.capacity):
            self.cache.append(_cache_elem_(0, 0))
    
    def find(self, key: int) -> int:
        hit: int = -1 
        for i in range(self.size):
            if self.cache[i].key == key:
                hit = i
        return hit
    
    def read_cap(self) -> int:
        return self.capacity
    
    def read_size(self) -> int:
        return self.size

    def swap_elem (self, ind1: int, ind2: int):
        tmp = self.cache[ind1]
        self.cache[ind1] = self.cache[ind2]
        self.cache[ind2] = tmp

    def full(self) -> bool:
        if self.size == self.capacity:
            return True
        return False

    def push(self, push_elem: _cache_elem_):
        self.cache[self.size] = push_elem
        self.size += 1
    
    def pop(self) -> _cache_elem_:
        pop = self.cache[self.size - 1]
        self.size -= 1
        return pop
    
    def increment_pp_counter(self, ind: int):
        self.cache[ind].counter += 1

    def counter(self, ind: int) -> int:
        return self.cache[ind].counter

    def lookup_update(self, key: int) -> bool:
        hit = self.find(key)

        if hit == -1: #if not found
            #if full pop from back
            if self.full() == True:
                self.pop()
            #push in back
            self.push(_cache_elem_(key, 1))
            return False
        else: #if found
            self.increment_pp_counter(hit)
            #this block for keeping cache sorting for fast insert
            if hit != 0:
                if self.counter(hit) > self.counter(hit - 1):
                    self.swap_elem(hit, hit - 1)
            return True

def generate_answer (keys_arr: arr_int, cache_capacity: int):
    cache = _cache_(cache_capacity)
    hits: int = 0
    for i in range(keys_arr.size):
        if cache.lookup_update(keys_arr.arr[i]) == True:
            hits += 1
    return cache, hits

def print_in_file (file_name: str, keys_arr: arr_int, cache: _cache_, hits: int):
    f = open(file_name, 'w')

    f.write(str(keys_arr.size))
    f.write('\n\n')
    for i in range(keys_arr.size):
        f.write(str(keys_arr.arr[i]))
        f.write(' ')
    f.write('\n\n')
    f.write(str(cache.read_cap()))
    f.write('\n\n')
    f.write(str(hits))


def main():
    quantity_of_keys: int = int(sys.argv[2])
    keys_arr: arr_int = generate_test(quantity_of_keys)
    cache, hits = generate_answer(keys_arr, int(quantity_of_keys/16))

    file_name: str = sys.argv[1]
    print_in_file(file_name, keys_arr, cache, hits)

main()
    


