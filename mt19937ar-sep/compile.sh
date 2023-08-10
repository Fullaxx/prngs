#!/bin/bash

set -e

gcc -Wall -ggdb3 mtTest.c mt19937ar.c -o mtTest.dbg
