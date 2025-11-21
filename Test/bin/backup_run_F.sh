#!/bin/bash
cd ..
make
cd bin
./cvrp_solver ../Benchmarks/F/F-n135-k7.vrp F-n135-k7 F
./cvrp_solver ../Benchmarks/F/F-n45-k4.vrp F-n45-k4 F
./cvrp_solver ../Benchmarks/F/F-n72-k4.vrp F-n72-k4 F
