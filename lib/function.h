#ifndef FUNCTION_H
#define FUNCTION_H

void sendStr(int fd, char *str);
void recvStr(int fd, char *str);
void sendInt(int fd, int i);
int recvInt(int fd);

int valueInArr(int val, int arr[], int size);
int getIndex(int *arr, int num, int select);
int countAvailableSeats(int id[], int size);
#endif