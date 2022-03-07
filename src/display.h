#pragma once
#include <ncurses.h>

#include "common.h"

#define CONFIG_HEIGHT 8
#define CONFIG_WIDTH 80
#define CONFIG_START_ROW 0
#define CONFIG_START_COL 0

#define ENTRY_HEIGHT 10
#define ENTRY_WIDTH 18
#define ENTRY_START_ROW 10
#define ENTRY_START_COL 0

#define EARNINGS_HEIGHT 8
#define EARNINGS_WIDTH 40
#define EARNINGS_START_ROW 10
#define EARNINGS_START_COL 25

int display_config(WINDOW *conf_window, char *conf_string);
int display_entry(WINDOW *entry_window, struct config *p_conf, struct sim_data *p_data);
int display_earnings(WINDOW *earn_window, struct config *p_conf, struct sim_data *p_data);
