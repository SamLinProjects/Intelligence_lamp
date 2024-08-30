#ifndef CLOCK_H
#define CLOCK_H

void time_sync_notification_cb(struct timeval *tv);
static void initialize_sntp(void);
static bool obtain_time(void);
void setClock(void *pvParameters);
void getClock(void *pvParameters);
void diffClock(void *pvParameters);
void clock(void);

#endif