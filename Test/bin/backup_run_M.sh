#!/bin/bash
cd ..
make
cd bin
./cvrp_solver ../Benchmarks/M/M-n101-k10.vrp M-n101-k10 M
./cvrp_solver ../Benchmarks/M/M-n121-k7.vrp M-n121-k7 M
./cvrp_solver ../Benchmarks/M/M-n151-k12.vrp M-n151-k12 M
./cvrp_solver ../Benchmarks/M/M-n200-k16.vrp M-n200-k16 M
./cvrp_solver ../Benchmarks/M/M-n200-k17.vrp M-n200-k17 M
