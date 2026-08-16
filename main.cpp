#include "./Generate_Orders/GenerateOrders.hpp"
#include "./Process_Orders/OrderPipeline.hpp"
#include "./Limit_Order_Book/Book.hpp"
#include "./Limit_Order_Book/Limit.hpp"
#include "./Limit_Order_Book/Order.hpp"
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <filesystem>

int main(int argc, char** argv) {
    const std::string initialOrdersPath = (argc > 1) ? argv[1] : "./Generate_Orders/initialOrders.txt";
    const std::string ordersPath = (argc > 2) ? argv[2] : "./orders.txt";
    const std::string timingCsvPath = (argc > 3) ? argv[3] : "./order_processing_times.csv";

    Book* book = new Book();

    OrderPipeline orderPipeline(book);
    orderPipeline.setOutputCsvPath(timingCsvPath);

    GenerateOrders generateOrders(book);
    generateOrders.setOutputDirectory(".");

    orderPipeline.processOrdersFromFile(initialOrdersPath);

    if (!std::filesystem::exists(ordersPath)) {
        std::cout << "Missing " << ordersPath << ", generating a sample workload." << std::endl;
        generateOrders.createOrders(100000);
    }


    // Start measuring time
    auto start = std::chrono::high_resolution_clock::now();

    orderPipeline.processOrdersFromFile(ordersPath);

    // Stop measuring time
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    std::cout << book->performanceSummary() << std::endl;
    std::cout << "Time taken to process orders: " << duration.count() << " milliseconds" << std::endl;

    delete book;
    return 0;
}
