#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
struct Date{
    unsigned char d,m,y;
};

void dataprint(const unsigned char *packed){
    unsigned char  mask = 0xF8;
    printf("Here is date: %d\n",(packed[0]&mask)>>3);
}
void monthprint(const unsigned char *packed){
    unsigned char mask = 0x7;
    unsigned char mask1 = 0x80;
    printf("Here is month: %d\n",((packed[0]&mask)<<1)|(packed[1]&mask1)>>7); // took 3 bits of month from first byte, shfted them left, added last month-bit from second byte
}
void yearprint(const unsigned char *packed){
    int buf;
    unsigned char mask = 0x7F;
    buf = (packed[1]&mask);
    if (buf >=50){
        printf("Here is year: %d\n",buf+1900);
    }
    else printf("Here is year: %d\n",buf+2000);

}
void datachange(unsigned char *packed,int date){
    unsigned char d = date;
    unsigned char vanish = 0x7;
    packed[0] &=vanish;
    packed[0]|=(date<<3);
    printf("Date changed successfully! \n");
}
void monthchange(unsigned char *packed,int month){
    unsigned char m =month;
    unsigned char vanish0 = 0xF8;
    unsigned char vanish1 = 0x7F;
    char monthmask1 = 0xE;
    char monthmak2 = 0x1;

    packed[0] &= vanish0; // cleared first byte for month
    packed[1] &= vanish1; // cleared second byte for month
    packed[0]|= (monthmask1&m)>>1;
    packed[1]|=(monthmak2&m)<<7;
    printf("Month changed successfully");


}
void yearchange(unsigned char *packed, int year){
    unsigned char y = year;
    unsigned char vanish = 0x80;
    packed[1] &= vanish;
    packed[1]|= y;
    printf("Year changed successfully! \n");


}
unsigned char* Packer(struct Date D){ // Packing function
    char daymask = 0x1F;
    char monthmask1 = 0xE;
    char monthmak2 = 0x1;
    char yearmask = 0x7F;
    unsigned char first;
    unsigned char second;
    unsigned char * packed = malloc(sizeof (unsigned char ) * 2);
    first = D.d & daymask;
    first <<=3; // first 5 bits of first byte are packed with date;
    first|= (D.m & monthmask1)>>1; // packing first byte with 3 last bits of month, need to pack one more bit, will do it in second byte
    second = (D.m & monthmak2)<<7; // packed last bit of month,shifted to leftmost position in byte.
    second|=(D.y&yearmask); // packed last 7 bits of year in second byte.
    printf("%d\n",second);
    packed[0] = first;
    packed[1] = second;
    return packed;
}

void Unpacker(const unsigned char * packed){
    unsigned char daymask = 0x1F << 3 ; // changing mask to fit positions of bytes
    unsigned  char monthmask1 = 0x7;
    unsigned char monthmak2 = 0x80;
    unsigned char yearmask = 0x7F;
    unsigned char  buf;
    struct Date d;
    d.d = (packed[0] & daymask) >> 3; // we got date
    printf("%d - date\n",d.d);
    buf = (packed[0]&monthmask1)<<1; // got first part of month, shifted right for last bit from right
    d.m=(packed[1]&monthmak2)>>7;
    d.m|=buf;
    printf("%d - month\n",d.m);
    d.y = (packed[1] & yearmask);
    printf("%d - year ",d.y);

}

void printBit(const unsigned char *packed){
    unsigned char mask; // setting mask to leftmost position
    int k;
    int flag=0;
    for(int i =0;i<2;i++){ // printing 2 bytes
        mask = 0x80;
        k = 7;
        for (int j = 0; j < 8; ++j) {
            printf("%d",(mask&packed[i])>>k); // printing curr bit shifting right
            k--;
            mask>>=1;
        }
        if (!flag)
            printf(" - first byte\n");
        else printf(" - second byte\n");
        flag++;


    }
    printf("\n");
}

int main() {
    int day = 0;
    int month = 0;
    int year =0;
    int flag = 0;
    int choice = 0;
    int num;
    unsigned char *Packed = malloc(sizeof (unsigned char ) *2);
    struct Date D;
    printf("Hello! Date Packer rdy to work, enter date: DD MM YY.\nDay 1-31, month 1-12, year 0 - 99\n");
    while (!flag){
        if (scanf("%d%d%d",&day,&month,&year) != 3){
            printf("Wrong symbols.Expecting digits.Terminating...\n");
            exit(1);
        }
        else flag = 1;
    }
    if((day < 32 && day > 0) && (month > 0 && month < 13) && (year > 0 && year < 100) ){
        printf("Date accepted,packing ... ");
        D.d = day;
        D.m = month;
        D.y = year;
        Packed = Packer(D);

    }
    else {printf("Data limit error! Day 1-31, month 1-12, year 0 - 99.Terminating...\n");
        exit(2);
    }
    while (1){
        printf("Enter num 0-7. Here are some instructions:\n \n если введен 0, то программа должна распечать дату в формате ЧЧ ММ ГГ, используя сохраненный ранее компактный формат,\n"
               "если введена 1, то программа печатает только дату,\n"
               "если введена 2, то программа печатает только месяц,\n"
               "если введена 3, то программа печатает только год в предложенном выше формате,\n"
               "если введена 4, то программа  изменяет дату,\n"
               "если введена 5, то программа изменяет месяц,\n"
               "если введена 6, то программа изменяет год,\n"
               "если введена 7, то программа выдает бинарное представление компактного формата,\n"
               "если введен любой другой символ отличный от чисел 0 - 7, то программа выходит из цикла и завершает работу,\n"
               "если введены числа 4, 5 или 6, то программа считывает дополнительное число, соответствующее новому дню, месяцу или году\n \n");
        scanf("%d",&choice);
        switch (choice) {
            case 0: Unpacker(Packed);break;
            case 1:dataprint(Packed);break;
            case 2:monthprint(Packed);break;
            case 3:yearprint(Packed);break;
            case 4:
                scanf("%d",&num); datachange(Packed,num); break;
            case 5:
                scanf("%d",&num);monthchange(Packed,num);break;
            case 6:scanf("%d",&num);yearchange(Packed,num);break;
            case 7:printBit(Packed);break;
            default:{
                printf("Freeing memory.Shutting down.");
                free(Packed);
                sleep(1);
                return 0;
            }
        }
    }

}
