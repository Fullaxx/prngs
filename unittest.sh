#!/bin/bash

set -e

./mt64.dbg | head -n 2 | tail -n 1 | \
grep -q "7266447313870364031  4946485549665804864 16945909448695747420 16394063075524226720  4873882236456199058"
