#!/bin/bash

set -e

gcc -Wall -ggdb3 main_mt32.c mt32.c -o mt32.dbg
gcc -Wall -ggdb3 main_mt64.c mt64.c -o mt64.dbg
