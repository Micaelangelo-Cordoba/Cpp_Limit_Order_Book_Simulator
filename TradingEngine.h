#pragma once
#ifndef TRADINGENGINE_H
#define TRADINGENGINE_H
#include "LimitOrderBook.h"
#include "Types.h"
#include "memory_pool.h"

class TradingEngine {

    private: 
        LimitOrderBook LOB {};
         TradingStats TS {}; /*MAIN IDEA: Tells information about the incoming prospective orders that 
                              Goes beyond just events/sec.

                              keeps track of how many orderes have been accepted.
                              how many orders have failed to be processed for various reasons
                              how many orders have been cancelled before being matched.
                              how many trade executions and shares fully processed?
                              */

    public:
        void packet_received(const NetworkPacket& packet); //will process these packets

        //create a number function specifically to process the return statements of the LOB add buy/sell functions
        //so tthat we can increment the trading stats values. 

        void process_stats(OrderStatus OS);

        //need function to return trading stats structs by reference to be used for output in main!
        TradingStats& get_stats() {return TS;}
     
};

inline void TradingEngine::process_stats(OrderStatus OS) { //put this in the switch statement
    //in packed_received

    using enum OrderStatus;

    switch (OS) {
        case (CANCELED_ORDER): {
            TS.cancelled_orders++;
            break;
        }
        case (DUPLICATE_ID): { //duplicate ids
            TS.duplicated_ids++;
            break;
        }
        case (INVALID_ORDER): { //error invalid info
            TS.invalid_orders++;
            break;
        }
        case (ALLOCATION_ERR): { //error allocated
            TS.allocation_err++;
            break;
        }
        case (ACCEPTED): { //successful order
            TS.accepted_orders++;
            break;
        }

    }
}

inline void TradingEngine::packet_received(const NetworkPacket& packet) {

    using enum Type;
    using enum OrderStatus;
    switch (packet.N_TYPE) { //enumerators used to determine what the LOB should do with the packets

        case (ADD_BUY): {
            OrderStatus OS = LOB.add_buy_limit_order(packet.n_id, packet.n_price, packet.n_size, packet.n_timestamp);
            process_stats(OS); //increment whats in the Trading Stats struct based on its return
            break;
        }
        case (ADD_SELL):  {
            OrderStatus OS = LOB.add_sell_limit_order(packet.n_id, packet.n_price, packet.n_size, packet.n_timestamp);
            process_stats(OS);
            break;
        }
        case (CANCEL): {
            OrderStatus OS = LOB.cancel_order(packet.n_id); //add member function
            process_stats(OS);
            break;
        }
        case (MKT_BUY): {
            LOB.execute_market_buy(packet.n_size);
            break;
        }
        case (MKT_SELL): {
            LOB.execute_market_sell(packet.n_size);
            break;
        }
        default: {
            std::cerr << "Error when processing packet\n";
            break;
        }
    }
}








#endif