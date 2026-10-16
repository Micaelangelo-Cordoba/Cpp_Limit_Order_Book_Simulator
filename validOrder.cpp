#include <iostream>
#include <random>
#include <vector>
#include "profiler.h"
#include "Types.h"
#include "LimitOrderBook.h"
#include "TradingEngine.h"
#include <cassert>



//This is a test to ensure the add_buy_limit_order and add_sell_limit_order
//functions of the LimitOrderBook class return the proper OrderStatus enumerator.
//This will be used to determine whether the order is valid or not.

//Its not valid if the following Occurs:

//1) DUPLICATE_ID: The prospective order coming in has an id that is identical
//to that of a previosuly resting order. If I insert the duplicate id order into
//the order_map hash map, it would override the information of a previous order
//with the same id, and I wouldnt be able to cancel the older order.

//2) INVALID_ORDER: The prospective order contains information that doesnt make
//sense for the Limit Order Book, such as having a price of 0 or a size of 0.

//3) ALLOCATION_ERR: This is a special error that occurs when the free-list
//allocator has been exhausted and returns nullptr when using the .allocate() 
//function. we cannot dereference nullptrs, so we should return early using this
//enumerator value.

// If order succeeds return ACCEPTED
int main() {

LimitOrderBook LOBTest {3}; //should do ALLOCATION ERR when a 4th valid
//order comes in!


//for Duplicate_id

//id, price, size, time
using enum OrderStatus;

auto first = LOBTest.add_buy_limit_order(1, 0, 5, 4); //INVALID_ORDER-price
auto second = LOBTest.add_sell_limit_order(2, 0, 6, 5); //INVALID_ORDER-price
auto third = LOBTest.add_buy_limit_order(3, 5, 0, 8); //INVALID_ORDER-size
auto fourth = LOBTest.add_sell_limit_order(4,6, 0, 10); //INVALID_ORDER-size

auto fifth = LOBTest.add_buy_limit_order(5, 4, 4, 12); //ACCEPTED
auto sixth = LOBTest.add_sell_limit_order(6, 7,7, 25); //ACCEPTED

auto seventh = LOBTest.add_buy_limit_order(6, 8, 8, 50); //DUPLICATE_ID
auto eighth = LOBTest.add_sell_limit_order(6, 9, 9, 34); //DUPLICATE_ID

auto ninth = LOBTest.add_buy_limit_order(7, 10, 10, 100); //ACCEPTED (LAST)

auto tenth = LOBTest.add_buy_limit_order(8, 15, 15, 110); //ALLOCATION_ERR
auto eleventh = LOBTest.add_sell_limit_order(9, 45, 45, 1110); //ALLOCATION_ERR

//test all possible cases from each side:

assert(first == INVALID_ORDER);
assert(second == INVALID_ORDER);
assert(third == INVALID_ORDER);
assert(fourth == INVALID_ORDER);

assert(fifth == ACCEPTED);
assert(sixth == ACCEPTED);

assert(seventh == DUPLICATE_ID);
assert(eighth == DUPLICATE_ID);

assert(ninth == ACCEPTED);

assert(tenth == ALLOCATION_ERR);
assert(eleventh == ALLOCATION_ERR);


    return 0;
}