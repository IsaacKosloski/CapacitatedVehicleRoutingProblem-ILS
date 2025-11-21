#!/bin/bash
cd ..
make
cd bin
./cvrp_solver ../Benchmarks/D/Loggi-n1001-k31.vrp Loggi-n1001-k31 D
./cvrp_solver ../Benchmarks/D/Loggi-n401-k23.vrp Loggi-n401-k23 D
./cvrp_solver ../Benchmarks/D/Loggi-n501-k24.vrp Loggi-n501-k24 D
./cvrp_solver ../Benchmarks/D/Loggi-n601-k19.vrp Loggi-n601-k19 D
./cvrp_solver ../Benchmarks/D/Loggi-n601-k42.vrp Loggi-n601-k42 D
./cvrp_solver ../Benchmarks/D/Loggi-n901-k42.vrp Loggi-n901-k42 D
./cvrp_solver ../Benchmarks/D/ORTEC-n242-k12.vrp ORTEC-n242-k12 D
./cvrp_solver ../Benchmarks/D/ORTEC-n323-k21.vrp ORTEC-n323-k21 D
./cvrp_solver ../Benchmarks/D/ORTEC-n405-k18.vrp ORTEC-n405-k18 D
./cvrp_solver ../Benchmarks/D/ORTEC-n455-k41.vrp ORTEC-n455-k41 D
./cvrp_solver ../Benchmarks/D/ORTEC-n510-k23.vrp ORTEC-n510-k23 D
./cvrp_solver ../Benchmarks/D/ORTEC-n701-k64.vrp ORTEC-n701-k64 D
