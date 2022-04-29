#ifndef MESSAGE_H
#define MESSAGE_H

#include "user.h"
#include "movie.h"
#include "cinema.h"
#include "time.h"
#include "seat.h"
#include "order.h"

enum
{
    SUCCESS,
    FAILURE,
    USERNAME,
    BOOKING,
    ORDERS,
    MOVIE,
    CINEMA,
    TIME,
    SEAT,
    CONFIRM
};

#define FARE 50000

typedef struct message
{
    user user;
    movie movie;
    cinema cinema;
    time time;
    seat seat;
    order order;
} message;

#endif