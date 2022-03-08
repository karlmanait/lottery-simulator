#pragma once
#include <stdint.h>

#define BUFFER_SIZE 1024
#define UTF_8_MAXSIZE 4
#define LOTTERY_NUMBERS 6

struct config {
        char currency_symbol[UTF_8_MAXSIZE];
        double cost;
        uint32_t tickets_per_sec;
        uint8_t max_number;
        uint8_t entries[LOTTERY_NUMBERS];
        double payouts[LOTTERY_NUMBERS];
};

struct sim_data {
        uint32_t tickets_bought;
        uint8_t drawn_numbers[LOTTERY_NUMBERS];
        uint32_t wincount[LOTTERY_NUMBERS];
};

