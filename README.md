# SAST2025
# Simulating Non-Functional Communication Requirements in Automotive Software

This repository contains the artifacts associated with the technical paper *"Simulating Non-Functional Communication Requirements in Automotive Software"*. The project focuses on modeling and simulating V2X (Vehicle-to-Everything) communication scenarios in the ns-3 network simulator to support the early evaluation of non-functional requirements in embedded automotive systems.

## Overview

The repository provides:

- **Generalized Simulation Pseudocode**: A structured high-level routine for simulating V2X networks using IEEE 802.11p and IEEE 802.11bd protocols. It covers the definition of PHY and MAC layer parameters (e.g., modulation, transmit power, midambles, EDCA), node mobility configuration, periodic CAM-like message generation, and instrumentation using FlowMonitor to collect metrics such as latency, jitter, packet loss, throughput, and delivery rate per distance.

- **Practical Example Script (`vehicular-comparison.cc`)**: A minimal demonstration using two moving vehicles exchanging UDP messages over varying distances. This example illustrates the instantiation of the simulation elements described in the pseudocode. While simple, it serves as a starting point for reproducible and extensible V2X experiments.

- **PDF**: Graphical visualization of simulation results for 2 vehicles communicating with 802.11p and 802.11b, in two situations: 50 meters away and 100 meters away
## Requirements

- ns-3.35 or later
- C++17 compatible compiler
- Linux environment recommended (tested on Ubuntu 22.04)

## Running the Example

To compile and run the simulation:

```bash
cd ns-3.35
./waf build
./waf --run scratch/vehicular-comparison
