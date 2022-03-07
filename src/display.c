#include "display.h"

int count_digit(uint8_t number)
{
        if (number < 10)
                return 1;
        else if (number < 100)
                return 2;
        return 3;
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
        mvprintw(CONFIG_START_ROW, CONFIG_START_COL, "Configuration (config.txt):\n");
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
        if (!entry_window) {
                entry_window = newwin(ENTRY_HEIGHT, ENTRY_WIDTH,
                                      ENTRY_START_ROW + 1, ENTRY_START_COL);
        }
        refresh();

        use_default_colors();
        start_color();
        init_pair(1, COLOR_GREEN, -1);

        mvwprintw(entry_window, 1, 1, " Entries │ Draw \n");
        mvwprintw(entry_window, 2, 1, "────────────────\n");
        for (int i = 0; i < LOTTERY_NUMBERS; ++i) {
                const int ROW_BORDER_OFFSET = 3;
                const int COL_ENTRIES_OFFSET = 9;
                const int COL_DRAWN_NUM_OFFSET = 12;
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
        wrefresh(entry_window);

        return 0;
}

int display_earnings(WINDOW *earn_window, struct config *p_conf, struct sim_data *p_data)
{
        if (!earn_window) {
                earn_window = newwin(EARNINGS_HEIGHT, EARNINGS_WIDTH,
                                     EARNINGS_START_ROW + 1, EARNINGS_START_COL);
        }
        refresh();

        const int COL_NUM = 18;
        double money_spent = p_data->tickets_bought * p_conf->cost;
        double money_won = 0;
        for (int i = 0; i < LOTTERY_NUMBERS; ++i) {
                money_won += p_data->win_count[i] * p_conf->payouts[i];
        }
        double balance = money_won - money_spent;

        mvwprintw(earn_window, 1, 1, " Earnings\n");
        mvwprintw(earn_window, 2, 1, "──────────────────────────────────────\n");
        mvwprintw(earn_window, 3, 1, " Tickets Bought:");
        mvwprintw(earn_window, 3, COL_NUM, "%u\n", p_data->tickets_bought);
        mvwprintw(earn_window, 4, 1, " Money Spent:");
        mvwprintw(earn_window, 4, COL_NUM, "%s%'.2f\n", p_conf->currency_symbol, money_spent);
        mvwprintw(earn_window, 5, 1, " Money Won:");
        mvwprintw(earn_window, 5, COL_NUM, "%s%'.2f\n", p_conf->currency_symbol, money_won);
        mvwprintw(earn_window, 6, 1, " Net Balance:");
        mvwprintw(earn_window, 6, COL_NUM, "%s%'.2f\n", p_conf->currency_symbol, balance);

        box(earn_window, 0, 0);
        wrefresh(earn_window);
        return 0;
}
