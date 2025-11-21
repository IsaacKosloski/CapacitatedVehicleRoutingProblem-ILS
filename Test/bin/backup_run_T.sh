#!/bin/bash
cd ..
make
cd bin
./cvrp_solver ../Benchmarks/T/T-n5-k5.vrp T-n5-k5 T
