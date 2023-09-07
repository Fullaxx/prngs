#!/bin/bash

set -e

./mt32.dbg | sha256sum | grep -q be6d89fbf6ffdd1b67ce830abda146b53a55a55e8fb2f0038413acee5779f683
./mt64.dbg | sha256sum | grep -q 00d549465bf1edddd2dffdcc61703da18116f676ec19599238c1912cc9155320
