#ifndef ORDER_H
#define ORDER_H
typedef struct order
{
    char uname[100];

    int movie_id;
    char movie[20];

    int cinema_id;
    char cinema[20];

    int time_id;
    char time[20];

    int seat_num;
    int seat_id[10];

} order;
#endif