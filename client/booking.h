#ifndef BOOKING_H
#define BOOKING_H
#include "../lib/message.h"

void booking(int fd, char *uname);
void receiveMovie();
void selectMovie();
void receiveCinema();
void selectCinema();
void receiveTime();
void selectTime();
void receiveSeat();
void selectSeat();
void confirmOrder();
void printTicket();
#endif