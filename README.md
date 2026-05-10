# Cpp_Limit_Order_Book_Simulator

# Overview

Engineered a low-latency C++ limit order book simulator that parses millions of market orders. It processes several bid and ask orders and organizes them based on price and time priority. 

It has been engineered to process and match 5 million simulated market orders in ~1.2 seconds on consumer hardware (2019 MacBook air).

# Implementation 

Utilized std::vector to create a pre-allocated memory pool to be used to allocate memory for Order structs used to parse and organize orders. This prevents requesting memory from the heap (new operator) every time memory for an Order struct needs to be allocated, drastically improving performance since this requires context switching.

2) Bypassed std::priority_queue to instead utilize a std::map, doubly linked list, and std::unordered map to manage orders in a performant manner.
std::map helps break the orders down into PriceLevels, where similar orders of identical price are placed into 
