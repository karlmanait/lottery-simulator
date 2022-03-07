#include <string.h>
#include <stdlib.h>
#include "config.h"

int set_config_struct(struct config *p_conf, char *key, char *value)
{
        if (!strcmp(key, "Currency")) {
                value[strcspn(value, "\n")] = 0;
                strcpy(p_conf->currency_symbol, ++value);

        } else if (!strcmp(key, "Ticket Cost")) {
                p_conf->cost = strtod(value, (char **)NULL);

        } else if (!strcmp(key, "Tickets Per Second")) {
                p_conf->tickets_per_sec = strtol(value, (char **)NULL, 10);

        } else if (!strcmp(key, "Maximum Entry Number")) {
                p_conf->max_number = strtol(value, (char **)NULL, 10);

        } else if (!strcmp(key, "Entries")) {
                const char *NUMBER_DELIMITER = ",";
                char *num = strtok(value, NUMBER_DELIMITER);
                for (int i = 0; num; ++i) {
                        p_conf->entries[i] = strtol(num, (char **)NULL, 10);
                        num = strtok(NULL, NUMBER_DELIMITER);
                }

        } else if (!strcmp(key, "Payouts")) {
                const char *NUMBER_DELIMITER = ",";
                char *num = strtok(value, NUMBER_DELIMITER);
                for (int i = 0; num; ++i) {
                        p_conf->payouts[i] = strtol(num, (char **)NULL, 10);
                        num = strtok(NULL, NUMBER_DELIMITER);
                }

        } else {
                fprintf(stderr, "Found illegal key in config: \"%s\"\n", key);
                return 1;
        }

        return 0;
}

void init_config(struct config *p_conf)
{
        for (int i = 0; i < LOTTERY_NUMBERS; ++i) {
                p_conf->entries[i] = 0;
                p_conf->payouts[i] = 0;
        }
}

int read_config(struct config *p_conf, char *conf_string)
{
        const char *BORDER_ROW_OFFSET = "\n";
        const char *BORDER_COL_OFFSET = " ";
        const char *CONFIG_DELIMITER = ":";
        int result = 0;
        char buffer[BUFFER_SIZE] = "";
        FILE *file = fopen("config.txt", "r");
        init_config(p_conf);

        strcat(conf_string, BORDER_ROW_OFFSET);
        while (fgets(buffer, sizeof buffer, file) != NULL) {
                strcat(conf_string, BORDER_COL_OFFSET);
                strcat(conf_string, buffer);
                char *key = strtok(buffer, CONFIG_DELIMITER);
                char *value = strtok(NULL, CONFIG_DELIMITER);
                if (set_config_struct(p_conf, key, value)) {
                        result = 1;
                        goto out;
                }
        }
        if (!feof(file)) {
                fprintf(stderr, "Configuration EOF error\n");
                result = 1;
                goto out;
        }
out:
        fclose(file);
        return result;
}
