#!/bin/bash

clang++ --std=c++17 pcre1.cpp -L/usr/local/lib -lpcre -o pcre1
time ./pcre1

clang++ --std=c++17 pcre2.cpp -L/usr/local/lib -lpcre2-8 -o pcre2
time ./pcre2

clang++ --std=c++17 hyperscan.cpp -L/usr/local/lib -lhs -o hyperscan
time ./hyperscan
