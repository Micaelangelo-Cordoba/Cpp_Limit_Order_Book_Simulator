# Cpp_Limit_Order_Book_Simulator

# Overview

Engineered a low-latency limit order book simulator (LOB) written in modern C++.

This simulator processes millions of simulated orders, organizing bids and asks in price-time priority. 

Through the utilization of custom memory management and efficient data structures, the engine achieves deterministic performance and processes over **4.3 million orders per second** on consumer hardware. 

# Performance Metrics 🚀
**Throughput (Single-Threaded):** ~4.3 million orders per second.

**Average latency (Order Processing):** ~230 nanoseconds per order

**Hardware Profile:** Tested on a 2019 Intel-based MacBook Air.

# Benchmark Notes 

- Compiled with -O3
  
- Order_Distribution: 40% add_bids / 40% add_asks / 15% cancel_orders / 5% market_matches
  
- normal price distribution
  
# 💻🧠 Architecture

Achieved microsecond-level latency by bypassing out-of-the-box standard data structures such as std::priority_queue.

The LOB state is managed by the combination of a few data structures that allow for algorithmically efficient
critical-path operations:

1) **Price Priority: (std::map)** Orders are grouped into Price-Level buckets. They are maintained in sorted order using balanced search trees, allowing for insertion and removal of Price-Level structs takes O(log P) time, where P is the number of price levels. The highest-priority bids and asks are accessible in O(1) time.
   
2) **Time Priority: (Doubly Linked-List)** Price-Level structs manage a doubly linked-list of Order structs. This allows for a FIFO execution priority for Orders at the same price level, allowing O(1) insertion at the tail and O(1) unlinking when an order's pointer is known.

3) **Efficient Cancellation: (std::unordered_map)** A hash map links an Order's ID with a direct pointer to its Order struct. This allows the engine to directly jump to an order, unlink it from the linked-list and free its memory back into the memory pool in O(1) average time without traversing through the list. If cancellation empties a price level, removing that level from std::map introduces an additional O(log P) operation.




# Custom Free-List Memory Pool

Dynamic memory allocation ('new'/'delete') introduces latency spikes due to kernel system calls and heap fragmentation

This engine implements a custom contiguous memory arena (free list) using a preallocated std::vector.

At startup, millions of Order Structs are allocated side-by-side in RAM. The order structs are padded and aligned (alignas(32)) to fit perfectly within 64-byte cache lines. 

During live trading, Order structs are popped from the free list memory stack in O(1) constant time, minimizing the need for requesting the Operating System for heap memory.

# How To Run (Linux/MacOS)

1) **Clone The Repository**
   
    git clone https://github.com/Micaelangelo-Cordoba/Cpp_Limit_Order_Book_Simulator.git

2) **Move to Main Directory**
   
    cd Cpp_Limit_Order_Book_Simulator

3) **compile program in C++20 and -O3 flag**
   
    g++ -std=c++20 -O3 main.cpp -o matching_engine

4) **run executable**

   ./matching_engine







