#define _POSIX_C_SOURCE 199309L

#include <locale.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "common.h"
#include "config.h"
#include "display.h"


void init_sim_data(struct sim_data *p_data)
{
        p_data->tickets_bought = 0;
        for (int i = 0; i < LOTTERY_NUMBERS; ++i) {
                p_data->wincount[i] = 0;
                p_data->drawn_numbers[i] = 0;
        }
}

void draw_numbers(struct sim_data *p_data, uint8_t limit)
{
        for (int i = 0; i < LOTTERY_NUMBERS; ++i) {
                p_data->drawn_numbers[i] = (rand() % limit) + 1;
        }
        p_data->tickets_bought++;
}

void count_match(struct config *p_conf, struct sim_data *p_data)
{
        int match = 0;
        for (int i = 0; i < LOTTERY_NUMBERS; ++i) {
                for (int j = 0; j < LOTTERY_NUMBERS; ++j) {
                        if (p_conf->entries[i] == p_data->drawn_numbers[j]) {
                                match++;
                                break;
                        }
                }
        }
        if (match)
                p_data->wincount[--match]++;
}

void *thread_get_input(void *arg)
{
        char *p_input = (char *)(arg);
        while (*p_input != 'q' && *p_input != 'Q') {
                *p_input = getch();
        }
        pthread_exit(NULL);
}

int main()
{
        setlocale(LC_ALL, "");
        srand(time(NULL));

        struct config conf;
        char conf_string[BUFFER_SIZE];
        if (read_config(&conf, conf_string)) {
                fprintf(stderr, "Configuration error\n");
                return 1;
        }

        initscr();
        noecho();

        struct sim_data data;
        init_sim_data(&data);

        struct timespec ts_sleep;
        bool min_tickets_per_sec = conf.tickets_per_sec <= 1;
        ts_sleep.tv_sec = min_tickets_per_sec ? 1.0 : 0;
        ts_sleep.tv_nsec = min_tickets_per_sec ? 0 : 1e9 / conf.tickets_per_sec;

        char input = 0;
        pthread_t tid;
        pthread_create(&tid, NULL, thread_get_input, (void *)&input);

        WINDOW *conf_window = NULL;
        WINDOW *entry_window = NULL;
        WINDOW *earn_window = NULL;
        WINDOW *wincount_window = NULL;

        if (display_config(conf_window, conf_string)) {
                fprintf(stderr, "Configuration display error\n");
                return 1;
        }
        mvprintw(LINES - 1, 0, "Press Q to quit ...");

        while (input != 'q' && input != 'Q') {
                draw_numbers(&data, conf.max_number);
                count_match(&conf, &data);

                if (display_entry(entry_window, &conf, &data)) {
                        fprintf(stderr, "Entries display error\n");
                        return 1;
                }

                if (display_earnings(earn_window, &conf, &data)) {
                        fprintf(stderr, "Earnings display error\n");
                        return 1;
                }

                if (display_wincount(wincount_window, &conf, &data)) {
                        fprintf(stderr, "Win Count display error\n");
                        return 1;
                }

                refresh();
                nanosleep(&ts_sleep, NULL);
        }
        mvprintw(LINES - 1, 0, "Press any key to continue ...");
        getch();
        endwin();
        return 0;
}


