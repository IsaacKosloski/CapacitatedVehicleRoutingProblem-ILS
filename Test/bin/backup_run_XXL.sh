#!/bin/bash
cd ..
make
cd bin
./cvrp_solver ../Benchmarks/XXL/Antwerp1.vrp Antwerp1 XXL
./cvrp_solver ../Benchmarks/XXL/Antwerp2.vrp Antwerp2 XXL
./cvrp_solver ../Benchmarks/XXL/Brussels1.vrp Brussels1 XXL
./cvrp_solver ../Benchmarks/XXL/Brussels2.vrp Brussels2 XXL
./cvrp_solver ../Benchmarks/XXL/Flanders1.vrp Flanders1 XXL
./cvrp_solver ../Benchmarks/XXL/Flanders2.vrp Flanders2 XXL
./cvrp_solver ../Benchmarks/XXL/Ghent1.vrp Ghent1 XXL
./cvrp_solver ../Benchmarks/XXL/Ghent2.vrp Ghent2 XXL
./cvrp_solver ../Benchmarks/XXL/Leuven1.vrp Leuven1 XXL
./cvrp_solver ../Benchmarks/XXL/Leuven2.vrp Leuven2 XXL
