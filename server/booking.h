#ifndef BOOKING_H
#define BOOKING_H
#include "../lib/message.h"

void setFd(int fd);
void sendListMovies();
void sendListCinemas();
void sendListTimes();
void sendListSeats();
void confirmOrder(int seatNum);
#endif
