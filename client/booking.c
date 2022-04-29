#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "../lib/message.h"
#include "../lib/function.h"

int socketfd;
message ticket;

void printTicket()
{
    printf("\n--------------------------------------\n");
    printf("\tBooked Ticket Details\n");
    printf("--------------------------------------\n\n");
    printf("Username: %s\n", ticket.order.uname);
    printf("\nMovie Name: %s\n", ticket.order.movie);
    printf("Cinema: %s\n", ticket.order.cinema);
    printf("Time: %s\n", ticket.order.time);
    printf("\nBooked Seat Number: ");
    for (int i = 0; i < ticket.order.seat_num; i++)
    {
        printf(" %d", ticket.order.seat_id[i]);
    }
    printf("\n"); 
    printf("--------------------------------------\n");
}

void confirmOrder()
{
    sendInt(socketfd, CONFIRM);
    recvInt(socketfd);
    printTicket();
}

void selectSeat()
{
    int valid, count = 0, num, select, available_seats;
    char status;

    for (int i = 0; i < ticket.seat.row * ticket.seat.col; i++)
    {
        status = ticket.seat.status[i] == 0 ? '-' : 'x';
        printf("%2d: %c\t", ticket.seat.id[i], status);
        count++;
        if (count == ticket.seat.col)
        {
            count = 0;
            printf("\n");
        }
    }
    available_seats = countAvailableSeats(ticket.seat.status, ticket.seat.row * ticket.seat.col);
    do
    {
        printf("\nPlease enter number of seat you want to book: ");
        scanf("%d", &num);
        if (num <= 0 || num > available_seats)
        {
            printf("Number of seats is invalid!\n");
        }
    } while (num <= 0 || num > available_seats);

    for (int i = 0; i < num; i++)
    {
        valid = 0;
        do
        {
            printf("\nPlease enter the ID of seat no.%d: ", i + 1);
            scanf("%d", &select);

            if ((valueInArr(select, ticket.seat.id, ticket.seat.row * ticket.seat.col) == 0) && (valueInArr(select, ticket.order.seat_id, i) == 1) &&
                (ticket.seat.status[select] == 0))
            {
                ticket.order.seat_id[i] = select;
                valid = 1;
                ticket.seat.status[i] = 1;
            }
            else
            {
                printf("ID is invalid or seat is already taken!\n");
            }
        } while (valid == 0);
    }
    printf("\n");

    ticket.order.seat_num = num;
    sendInt(socketfd, ticket.order.seat_num);
    for (int i = 0; i < ticket.order.seat_num; i++)
    {
        sendInt(socketfd, ticket.order.seat_id[i]);
    }

    confirmOrder(ticket.order.seat_id);
}

void receiveSeat()
{
    sendInt(socketfd, SEAT);
    ticket.seat.row = recvInt(socketfd);
    ticket.seat.col = recvInt(socketfd);

    for (int i = 0; i < ticket.seat.row * ticket.seat.col; i++)
    {
        ticket.seat.id[i] = recvInt(socketfd);
        ticket.seat.status[i] = recvInt(socketfd);
    }

    selectSeat();
}

void selectTime()
{
    int select, valid = 0, indx;

    for (int i = 0; i < ticket.time.num; i++)
    {
        printf("%d: %s\n", ticket.time.id[i], ticket.time.name[i]);
    }
    do
    {
        printf("\nPlease enter the ID of time: ");
        scanf("%d", &select);
        if (valueInArr(select, ticket.time.id, ticket.time.num) == 1)
        {
            printf("ID is invalid !\n");
        }
        else
            valid = 1;
    } while (valid == 0);
    printf("\n");

    indx = getIndex(ticket.time.id, ticket.time.num, select);
    ticket.order.time_id = select;
    strcpy(ticket.order.time, ticket.time.name[indx]);

    sendInt(socketfd, ticket.order.time_id);
    sendStr(socketfd, ticket.order.time);

    receiveSeat();
}

void receiveTime()
{
    sendInt(socketfd, TIME);
    ticket.time.num = recvInt(socketfd);

    for (int i = 0; i < ticket.time.num; i++)
    {
        ticket.time.id[i] = recvInt(socketfd);
        recvStr(socketfd, ticket.time.name[i]);
    }

    selectTime();
}

void selectCinema()
{
    int select, valid = 0, indx;

    for (int i = 0; i < ticket.cinema.num; i++)
    {
        printf("%d: %s\n", ticket.cinema.id[i], ticket.cinema.name[i]);
    }
    do
    {
        printf("\nPlease enter the ID of cinema: ");
        scanf("%d", &select);
        if (valueInArr(select, ticket.cinema.id, ticket.cinema.num) == 1)
        {
            printf("ID is invalid !\n");
        }
        else
            valid = 1;
    } while (valid == 0);
    printf("\n");

    indx = getIndex(ticket.cinema.id, ticket.cinema.num, select);
    ticket.order.cinema_id = select;
    strcpy(ticket.order.cinema, ticket.cinema.name[indx]);

    sendInt(socketfd, ticket.order.cinema_id);
    sendStr(socketfd, ticket.order.cinema);

    receiveTime();
}

void receiveCinema()
{
    sendInt(socketfd, CINEMA);
    ticket.cinema.num = recvInt(socketfd);

    for (int i = 0; i < ticket.cinema.num; i++)
    {
        ticket.cinema.id[i] = recvInt(socketfd);
        recvStr(socketfd, ticket.cinema.name[i]);
    }

    selectCinema();
}

void selectMovie()
{
    int select, valid = 0, indx;

    for (int i = 0; i < ticket.movie.num; i++)
    {
        printf("%d: %s\n", ticket.movie.id[i], ticket.movie.name[i]);
    }
    do
    {
        printf("\nPlease enter the ID of movie: ");
        scanf("%d", &select);
        if (valueInArr(select, ticket.movie.id, ticket.movie.num) == 1)
        {
            printf("ID is invalid !\n");
        }
        else
            valid = 1;
    } while (valid == 0);
    printf("\n");

    indx = getIndex(ticket.movie.id, ticket.movie.num, select);
    ticket.order.movie_id = select;
    strcpy(ticket.order.movie, ticket.movie.name[indx]);

    sendInt(socketfd, ticket.order.movie_id);
    sendStr(socketfd, ticket.order.movie);

    receiveCinema();
}

void receiveMovie()
{
    sendInt(socketfd, MOVIE);
    ticket.movie.num = recvInt(socketfd);

    for (int i = 0; i < ticket.movie.num; i++)
    {
        ticket.movie.id[i] = recvInt(socketfd);
        recvStr(socketfd, ticket.movie.name[i]);
    }
    selectMovie();
}

void booking(int fd, char *uname)
{
    int choice;
    socketfd = fd;
    strcpy(ticket.order.uname, uname);

    choice = 1;
    printf("\n");
    if (choice == 1)
    {
        sendInt(socketfd, BOOKING);
        receiveMovie();
    }
}