#!/bin/bash

set -e

./mt19937-64test.dbg | sha256sum | grep -q 00d549465bf1edddd2dffdcc61703da18116f676ec19599238c1912cc9155320