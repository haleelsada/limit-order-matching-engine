# Limit Order Book

This is a personal C++ project where I built a simplified exchange matching engine from scratch.
The goal was to understand how a limit order book works, how orders are matched, and how different data structures affect latency.

## Overview

The project simulates the core part of an exchange that receives buy and sell orders and keeps track of the active market.

In practical terms, it can:

- accept new orders
- match orders against the opposite side of the book
- store resting orders at specific price levels
- cancel and modify existing orders
- handle stop and stop-limit orders
- measure timing and internal book activity while orders are processed

## Main Features

### Order types

The engine supports several common order types:

- `Market` orders
- `Limit` orders
- `Stop` orders
- `Stop Limit` orders

It also supports add, cancel, and modify operations for the order types that are stored in the book.

### Price-time priority

Orders at the same price are stored in FIFO order.
That means the first order added at a price level is the first one that gets filled.

### Separate buy and sell books

Buy and sell orders are stored on opposite sides of the book.
This makes it easier to find the best bid and best offer quickly.

### AVL trees for price levels

Each side of the book uses an AVL tree to store price levels in sorted order.
This keeps operations on new price levels efficient even as the book grows.

### Direct lookup by order ID

Orders are also stored in hash maps, which makes cancel and modify operations fast when the order ID is known.

### Stop order support

Stop orders and stop-limit orders are stored separately until their trigger price is reached.
Once triggered, they are converted into active market or limit behavior.

### Performance tracking

I also added some lightweight tracking to make it easier to see what the book is doing while it runs:

- per-order processing time
- number of executed orders
- AVL rebalance count
- order and price-level allocation counters
- CSV output for timing data

## Performance Considerations

This project emphasizes both correctness and the operational behavior of a matching engine under load.

The current version already gives a base for discussing:

- cache locality
- allocator overhead
- branch-heavy matching logic
- single-threaded execution without lock contention
- data layout choices

The structure is still intentionally simple, but it leaves room for future optimization work.

## How It Is Built

The main pieces are:

- `Limit_Order_Book/`
  - core book, order, and price-level logic
- `Process_Orders/`
  - file-based order processing and timing collection
- `Generate_Orders/`
  - synthetic order flow generation
- `test/`
  - unit and integration tests

## Repository Layout

```text
Limit-Order-Book/
├── Generate_Orders/
│   ├── GenerateOrders.cpp
│   ├── GenerateOrders.hpp
│   └── initialOrders.txt
├── Limit_Order_Book/
│   ├── Book.cpp
│   ├── Book.hpp
│   ├── Limit.cpp
│   ├── Limit.hpp
│   ├── Order.cpp
│   └── Order.hpp
├── Process_Orders/
│   ├── OrderPipeline.cpp
│   └── OrderPipeline.hpp
├── test/
│   ├── CMakeLists.txt
│   ├── ExampleOrdersTests.cpp
│   └── LimitOrderBookTests.cpp
├── main.cpp
├── CMakeLists.txt
└── README.md
```

## Build

Requirements:

- CMake 3.29+
- a C++20 compiler

Build from the project root:

```bash
cmake -S . -B build -DBUILD_TESTS=OFF
cmake --build build -j
```

If you have GoogleTest installed and want to build tests too:

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build
```

## Run

The executable uses the bundled initial order file by default and will generate a sample workload if `orders.txt` is missing.

```bash
./build/LimitOrderBook
```

You can also pass your own paths:

```bash
./build/LimitOrderBook <initial_orders_file> <orders_file> <timing_csv_file>
```

Example:

```bash
./build/LimitOrderBook ./Generate_Orders/initialOrders.txt ./orders.txt ./order_processing_times.csv
```

## Output

When the program runs, it can produce:

- a processed order run
- a timing CSV file
- a summary line with execution, rebalance, and allocation counters

## Tests

The test suite covers basic order book behavior such as:

- adding orders
- cancelling orders
- modifying orders
- maintaining the tree structure
- validating order book edges

## Motivation

learn about:
- how order books are structured
- how matching systems keep latency low
- how data structures affect performance
- how to generate realistic market-like workloads
