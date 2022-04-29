#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLS 10
#define ROWS 10


void initializeSeats(char *filename){

    FILE *fptr;
    fptr = fopen(filename,"w");
    if(fptr == NULL){
        printf("Error in file opening for seat initialization. \n");
    }

    for(int i=0;i<ROWS;i++){
        for(int j=0;j<COLS;j++){
            fprintf(fptr,"0");
        }
        fprintf(fptr,"\n");
    }

    fclose(fptr);
}


int isSeatAvailable(char *filename, int row, int col){
    
    FILE *fptr;
    fptr = fopen(filename,"r+");
    if(fptr == NULL){
        printf("Error in file opening for book a seat. \n");
        return -1;
    }

    fseek(fptr,(((row*(ROWS+1)) + col)-1-(ROWS+1)),SEEK_SET);

    char seat;
    seat=fgetc(fptr);

    if(seat == '0'){
        fclose(fptr);
        return 1;
    }
    else if(seat == '1'){
        fclose(fptr);
        return -1;
    }

    fclose(fptr);
    return 1;
}


int bookTicket(char *filename, int row, int col){  

    if(row >ROWS || col > COLS || col < 1 || row < 1){
        printf("Please enter row and col between 1 and 25 \n");
        return -1;
    }

    FILE *fptr;
    fptr = fopen(filename,"r+");
    if(fptr == NULL){
        printf("Error in file opening for book a seat. \n");
        return -1;
    }

    int isAvailable = isSeatAvailable(filename,row,col);
    if(isAvailable == 1){
        printf("Seat is available. \n");

        fseek(fptr,(((row*(ROWS+1)) + col)-1-(ROWS+1)),SEEK_SET);
        fputs("1", fptr);
        fclose(fptr);
        return 1;
    }
    else{
        printf("Seat is not available. \n");
        fclose(fptr);
        return -1;
    }

    return 0;
}


void printMenu(char *filename){
    printf("================================================== \n");
    printf("              Screen is this way                   \n");
    printf("================================================== \n");

    FILE *fptr;
    fptr = fopen(filename,"r");
    if(fptr == NULL){
        printf("Error in file opening for print menu. \n");
        return;
    }

    printf("\t");
    for(int i=0;i<COLS;i++){
        printf("%d ",i+1);
    }
    printf("\n\n");


    char ch = ' ';
    int i = 1;
    int counter = 0;

    putchar(ch);
    while( (ch=fgetc(fptr)) != EOF){
        if(ch=='\n'){
            continue;
        }

        if((counter) % ROWS == 0){
            printf("\n");
            printf("%d\t",i);
            i++;
            counter=0;
        }
        putchar(ch);
        printf(" ");
        counter++;     

    }
    printf("\n==================================================\n");

    fclose(fptr);


}


int main()
{
    initializeSeats("theater1.txt");
    printMenu("theater1.txt");

    

    int row,col;

    printf("Enter row for booking a ticket: ");
    scanf("%d",&row);

    printf("Enter col for booking a ticket: ");
    scanf("%d",&col);

    
    int result=0;
    result = bookTicket("theater1.txt",row,col);
    
    if(result == 1){
        printf("Success in booking \n");
    }
    else if (result == -1){
        printf("Booking Unsucssfull... \n");
        return 0;
    }
    printMenu("theater1.txt");

    return 0;
}
