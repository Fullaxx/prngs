#!/bin/bash

set -e

gcc -Wall -ggdb3 mt19937-64test.c mt19937-64.c -o mt19937-64test.dbg
