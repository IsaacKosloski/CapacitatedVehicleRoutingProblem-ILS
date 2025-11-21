#!/bin/bash
cd ..
make
cd bin
./cvrp_solver ../Benchmarks/E/E-n101-k14.vrp E-n101-k14 E
./cvrp_solver ../Benchmarks/E/E-n101-k8.vrp E-n101-k8 E
./cvrp_solver ../Benchmarks/E/E-n22-k4.vrp E-n22-k4 E
./cvrp_solver ../Benchmarks/E/E-n23-k3.vrp E-n23-k3 E
./cvrp_solver ../Benchmarks/E/E-n30-k3.vrp E-n30-k3 E
./cvrp_solver ../Benchmarks/E/E-n33-k4.vrp E-n33-k4 E
./cvrp_solver ../Benchmarks/E/E-n51-k5.vrp E-n51-k5 E
./cvrp_solver ../Benchmarks/E/E-n76-k10.vrp E-n76-k10 E
./cvrp_solver ../Benchmarks/E/E-n76-k14.vrp E-n76-k14 E
./cvrp_solver ../Benchmarks/E/E-n76-k7.vrp E-n76-k7 E
./cvrp_solver ../Benchmarks/E/E-n76-k8.vrp E-n76-k8 E
