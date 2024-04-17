#!/bin/bash
set -euo pipefail
g++ -O3 --std=c++20 main.cc
time ./a.out | tee output.txt
