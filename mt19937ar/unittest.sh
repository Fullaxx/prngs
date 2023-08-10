#!/bin/bash

set -e

./mt19937ar.dbg | head -n 2 | tail -n 1 | \
grep -q "1067595299  955945823  477289528 4107218783 4228976476"
