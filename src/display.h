#pragma once
#include <ncurses.h>

#include "common.h"

#define CONFIG_HEIGHT 8
#define CONFIG_WIDTH 60
#define CONFIG_START_ROW 0
#define CONFIG_START_COL 0

#define ENTRY_HEIGHT 10
#define ENTRY_WIDTH 18
#define ENTRY_START_ROW 10
#define ENTRY_START_COL 0

#define EARNINGS_HEIGHT 8
#define EARNINGS_WIDTH 38
#define EARNINGS_START_ROW 10
#define EARNINGS_START_COL 22

#define WINCOUNT_HEIGHT 10
#define WINCOUNT_WIDTH 46
#define WINCOUNT_START_ROW 20
#define WINCOUNT_START_COL 0

int display_config(WINDOW *conf_window, char *conf_string);
int display_entry(WINDOW *entry_window, struct config *p_conf, struct sim_data *p_data);
int display_earnings(WINDOW *earn_window, struct config *p_conf, struct sim_data *p_data);
int display_wincount(WINDOW *wincount_window, struct config *p_conf, struct sim_data *p_data);
