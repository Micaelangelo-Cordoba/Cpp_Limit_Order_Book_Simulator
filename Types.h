#pragma once
#ifndef TYPES_H
#define TYPES_H
#include <cstdint>



enum class Type : char { //will be used by simulated network packets to indicate what type of order they are
    ADD_BUY = 'B', 
    ADD_SELL = 'S',
    CANCEL = 'C',
    MKT_BUY = 'N',
    MKT_SELL = 'M'
};

struct NetworkPacket { //represents a fully parsed packet that needs to be processed by the order book
   
    uint64_t n_id;
    uint64_t n_price;
    uint64_t n_timestamp;
    uint32_t n_size;
    Type N_TYPE;
};



//create another scoped enumeration so we can inform the Engine why certain orders
//have beeen Denied for various reasons

enum class OrderStatus : char {
    ACCEPTED,
    DUPLICATE_ID, //for add_buy add_sell
    ALLOCATION_ERR,  //cannot allocate because pool is exhausted
    INVALID_ORDER,   //certain orders have wrong field values such as 0 size, 0 price.
    CANCELED_ORDER, //keep track of cancelled orders and even failure to cancel
    CANCEL_FAIL
        
};

//struct will be used to gather statistics that simply tracking events per second doesnt show:
struct TradingStats {

uint64_t accepted_orders {}; //counts all valid orders
uint64_t duplicated_ids {}; //Failed orders
uint64_t invalid_orders {}; //failed orders
uint64_t allocation_err {}; //failed orders

uint64_t cancelled_orders {}; //orders that have been cancelled before being matched!
uint64_t cancel_fails {}; //now accounting for cancel failures
uint64_t trade_executions {}; //How many matches have occured!
//must remember someone can buy 250 shares, and they would end up buying 100 orders from one seller, and 150 orders
//out of 250 orders from another seller, leading to two trade executions despite one function call.

uint64_t traded_shares {}; //how many total shares have been fully processed. (note orders contain several shares)!

uint64_t market_buy_requests {};
uint64_t market_sell_requests {};

};
struct MatchResult { //OrderStatus cannot necessarily represent tradeExecutions and TradedShares since
    //they dont increment by one. So we can use the matching functions to return one of these.

    uint64_t trade_exec {};
    uint64_t trad_shar {};

    uint64_t mark_buy_req {};
    uint64_t mark_sell_req {};

};

struct PriceLevel;

//Order Node enforces it to fit nicely with CPU cache lines which are ~64 bytes
//makes them fit contiguously in memory
struct alignas(32) Order {
uint64_t id; //identifier
uint64_t price; //int price
uint64_t timestamp; //nanoseconds since start enforces time-priority tie breakers
uint32_t size; //quantity of shares 

Order* next {nullptr}; //pointer to next Order in the time-queue
Order* prev {nullptr}; //pointer to previous order

PriceLevel* parent_level = nullptr;
};

struct PriceLevel {  //All orders are grouped by price .
    Order* head {nullptr};
    Order* tail {nullptr};
    //this manages the case where there are multiple order with the same price, 
    //but different timestamps!
    uint64_t price;
    uint32_t total_volume = 0;
  
    bool is_bid;
};



#endif