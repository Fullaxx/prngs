#!/bin/bash

set -e

./mt19937ar-cok.dbg | sha256sum | grep -q be6d89fbf6ffdd1b67ce830abda146b53a55a55e8fb2f0038413acee5779f683
