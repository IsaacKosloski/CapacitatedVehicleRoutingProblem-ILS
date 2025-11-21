#!/bin/bash
cd ..
make
cd bin
./cvrp_solver ../Benchmarks/Vrp-Set-XML100/*.vrp * XML
