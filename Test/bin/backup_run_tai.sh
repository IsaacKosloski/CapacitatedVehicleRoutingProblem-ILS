#!/bin/bash
cd ..
make
cd bin
./cvrp_solver ../Benchmarks/tai/tai100a.vrp tai100a tai
./cvrp_solver ../Benchmarks/tai/tai100b.vrp tai100b tai
./cvrp_solver ../Benchmarks/tai/tai100c.vrp tai100c tai
./cvrp_solver ../Benchmarks/tai/tai100d.vrp tai100d tai
./cvrp_solver ../Benchmarks/tai/tai150a.vrp tai150a tai
./cvrp_solver ../Benchmarks/tai/tai150b.vrp tai150b tai
./cvrp_solver ../Benchmarks/tai/tai150c.vrp tai150c tai
./cvrp_solver ../Benchmarks/tai/tai150d.vrp tai150d tai
./cvrp_solver ../Benchmarks/tai/tai385.vrp tai385 tai
./cvrp_solver ../Benchmarks/tai/tai75a.vrp tai75a tai
./cvrp_solver ../Benchmarks/tai/tai75b.vrp tai75b tai
./cvrp_solver ../Benchmarks/tai/tai75c.vrp tai75c tai
./cvrp_solver ../Benchmarks/tai/tai75d.vrp tai75d tai
