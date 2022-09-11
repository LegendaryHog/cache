#!usr/bin/python

from operator import truediv
import random, sys

MAX_KEY = 128
MIN_QUAN = 32
MAX_QUAN = 512

def generate_test(quantity_of_keys):
    keys_arr = []
    for i in range(quantity_of_keys):
        keys_arr[i] = random(1, MAX_KEY)
    return keys_arr

#for simplification data an key is the same
class _cache_elem_:
    key = 0
    counter = 0

class _cache_:
    capacity = 0
    size = 0
    cache = []
    
    def find(self, key):
        for i in range(self.size):
            if self.cache[i].key == key:
                return i
            return -1

    def swap_elem (self, ind1, ind2):
        tmp = self.cache[ind1]
        self.cache[ind1] = self.cache[ind2]
        self.cache[ind2] = tmp

    def full(self):
        if self.size == self.capacity:
            return True
        return False

    def push(self, push_elem):
        self.cache[self.size] = push_elem
        self.size += 1
    
    def pop(self):
        pop = self.cache[self.size - 1]
        self.size -= 1
        return pop
    
    def increment_pp_counter(self, ind):
        self.cache[ind].counter += 1

    def counter(self, ind):
        return self.cache[ind].counter

    def lookup_update(self, key):
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

def generate_answer (keys_arr, quantity_of_keys, cache_capacity, hits):
    cache = _cache_(cache_capacity, 0, [])

    for i in range(quantity_of_keys):
        hits += cache.lookup_update(keys_arr[i])
    return cache

def print_in_file (file_name, keys_arr, quantity_of_keys, cache):
    f = open(file_name, 'w')

    f.write(quantity_of_keys)
    f.write('\n')
    f.write(keys_arr)
    f.write('\n')
    f.write()


def main():
    quantity_of_keys = random(MIN_QUAN, MAX_QUAN)
    keys_arr = generate_test(quantity_of_keys)
    hits = 0
    cache = generate_answer(keys_arr, quantity_of_keys, int(quantity_of_keys/16), hits)

    file_name = 'test.txt'
    print_in_file(file_name, keys_arr, quantity_of_keys, cache)
    


