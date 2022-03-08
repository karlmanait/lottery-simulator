#include <string.h>

#include "display.h"

int count_digit(uint32_t number)
{
        int count = 0;
        do {
                number /= 10;
                count++;
        } while (number > 0);
        return count;
}

bool is_match(uint8_t drawn_number, struct config *p_conf)
{
        for (int i = 0; i < LOTTERY_NUMBERS; ++i) {
                if (drawn_number == p_conf->entries[i])
                        return true;
        }
        return false;
}

int display_config(WINDOW *conf_window, char *conf_string)
{
        mvprintw(CONFIG_START_ROW, CONFIG_START_COL, "Configuration (config.txt):");
        if (!conf_window) {
                conf_window = newwin(CONFIG_HEIGHT, CONFIG_WIDTH,
                                     CONFIG_START_ROW + 1, CONFIG_START_COL);
        }
        refresh();

        mvwprintw(conf_window, 0, 0, "%s", conf_string);
        box(conf_window, 0, 0);

        wrefresh(conf_window);

        return 0;
}

int display_entry(WINDOW *entry_window, struct config *p_conf, struct sim_data *p_data)
{
        const int ROW_BORDER_OFFSET = 3;
        const int ROW_DIVIDER = 2;
        const int COL_ENTRIES_OFFSET = 9;
        const int COL_DRAWN_NUM_OFFSET = 12;
        const int COL_DIVIDER = 10;

        if (!entry_window) {
                entry_window = newwin(ENTRY_HEIGHT, ENTRY_WIDTH,
                                      ENTRY_START_ROW, ENTRY_START_COL);
        }
        refresh();

        use_default_colors();
        start_color();
        init_pair(1, COLOR_GREEN, -1);

        mvwprintw(entry_window, 1, 1, " Entries │ Draw ");
        mvwprintw(entry_window, 2, 1, "─────────┼──────");
        for (int i = 0; i < LOTTERY_NUMBERS; ++i) {
                int row = i + ROW_BORDER_OFFSET;
                int col_entries = COL_ENTRIES_OFFSET - count_digit(p_conf->entries[i]);

                mvwprintw(entry_window, row, col_entries, "%u │", p_conf->entries[i]);
                if (is_match(p_data->drawn_numbers[i], p_conf)) {
                        wattron(entry_window, COLOR_PAIR(1));
                }
                mvwprintw(entry_window, row, COL_DRAWN_NUM_OFFSET, "%u", p_data->drawn_numbers[i]);
                wattroff(entry_window, COLOR_PAIR(1));
        }

        box(entry_window, 0, 0);
        mvwprintw(entry_window, 0, COL_DIVIDER, "┬");
        mvwprintw(entry_window, ENTRY_HEIGHT - 1, COL_DIVIDER, "┴");
        mvwprintw(entry_window, ROW_DIVIDER, 0, "├");
        mvwprintw(entry_window, ROW_DIVIDER, ENTRY_WIDTH - 1, "┤");

        wrefresh(entry_window);

        return 0;
}

int display_earnings(WINDOW *earn_window, struct config *p_conf, struct sim_data *p_data)
{
        const int ROW_DIVIDER = 2;
        const int COL_OFFSET = EARNINGS_WIDTH - 2;
        const int COL_STRLEN_OFFSET = -2;
        char buffer[BUFFER_SIZE];
        int col;

        if (!earn_window) {
                earn_window = newwin(EARNINGS_HEIGHT, EARNINGS_WIDTH,
                                     EARNINGS_START_ROW, EARNINGS_START_COL);
        }
        refresh();

        double money_spent = p_data->tickets_bought * p_conf->cost;
        double money_won = 0;
        for (int i = 0; i < LOTTERY_NUMBERS; ++i) {
                money_won += p_data->wincount[i] * p_conf->payouts[i];
        }
        double balance = money_won - money_spent;

        mvwprintw(earn_window, 1, 1, " Earnings");
        mvwprintw(earn_window, 2, 1, "──────────────────────────────────────");

        col = COL_OFFSET - count_digit(p_data->tickets_bought);
        mvwprintw(earn_window, 3, 1, " Tickets Bought:");
        mvwprintw(earn_window, 3, col, "%u", p_data->tickets_bought);

        snprintf(buffer, sizeof buffer, "%s%'.2f", p_conf->currency_symbol, money_spent);
        col = COL_OFFSET - COL_STRLEN_OFFSET -  strlen(buffer);
        mvwprintw(earn_window, 4, 1, " Money Spent:");
        mvwprintw(earn_window, 4, col, "%s%'.2f", p_conf->currency_symbol, money_spent);

        snprintf(buffer, sizeof buffer, "%s%'.2f", p_conf->currency_symbol, money_won);
        col = COL_OFFSET - COL_STRLEN_OFFSET -  strlen(buffer);
        mvwprintw(earn_window, 5, 1, " Money Won:");
        mvwprintw(earn_window, 5, col, "%s%'.2f", p_conf->currency_symbol, money_won);

        snprintf(buffer, sizeof buffer, "%s%'.2f", p_conf->currency_symbol, balance);
        col = COL_OFFSET - COL_STRLEN_OFFSET -  strlen(buffer);
        mvwprintw(earn_window, 6, 1, " Net Balance:");
        mvwprintw(earn_window, 6, col, "%s%'.2f", p_conf->currency_symbol, balance);

        box(earn_window, 0, 0);
        mvwprintw(earn_window, ROW_DIVIDER, 0, "├");
        mvwprintw(earn_window, ROW_DIVIDER, EARNINGS_WIDTH - 1, "┤");

        wrefresh(earn_window);

        return 0;
}

int display_wincount(WINDOW *wincount_window, struct config *p_conf, struct sim_data *p_data)
{
        const int ROW_DIVIDER = 2;
        const int ROW_BORDER_OFFSET = 3;
        const int COL_DIVIDER = 20;
        const int COL_WINCOUNT_OFFSET = 19;
        const int COL_EARNINGS_OFFSET = WINCOUNT_WIDTH - 2;
        const int COL_STRLEN_OFFSET = -2;
        char buffer[BUFFER_SIZE];
        int col;

        if (!wincount_window) {
                wincount_window = newwin(WINCOUNT_HEIGHT, WINCOUNT_WIDTH,
                                         WINCOUNT_START_ROW + 1, WINCOUNT_START_COL);
        }
        refresh();

        mvwprintw(wincount_window, 1, 1, " Times Won         │ Earnings Per Matchings ");
        mvwprintw(wincount_window, 2, 1, "───────────────────┼────────────────────────");
        for (int i = 0; i < LOTTERY_NUMBERS; ++i) {
                int row = i + ROW_BORDER_OFFSET;
                double earnings = p_data->wincount[i] * p_conf->payouts[i];

                mvwprintw(wincount_window, row, 1, " %u Matches: ", i + 1);

                col = COL_WINCOUNT_OFFSET - count_digit(p_data->wincount[i]);
                mvwprintw(wincount_window, row, col, "%u │", p_data->wincount[i]);

                snprintf(buffer, sizeof buffer, "%s%'.2f", p_conf->currency_symbol, earnings);
                col = COL_EARNINGS_OFFSET - COL_STRLEN_OFFSET - strlen(buffer);
                mvwprintw(wincount_window, row, col, "%s%'.2f", p_conf->currency_symbol, earnings);
        }

        box(wincount_window, 0, 0);
        mvwprintw(wincount_window, ROW_DIVIDER, 0, "├");
        mvwprintw(wincount_window, ROW_DIVIDER, WINCOUNT_WIDTH - 1, "┤");
        mvwprintw(wincount_window, 0, COL_DIVIDER, "┬");
        mvwprintw(wincount_window, ENTRY_HEIGHT - 1, COL_DIVIDER, "┴");

        wrefresh(wincount_window);

        return 0;
}
