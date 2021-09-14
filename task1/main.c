#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <math.h>
volatile int flag =0;


void Handler(int v){
    signal(SIGUSR1,Handler);
    printf("infinity solutions (D<0)!\n");
    flag = 1;

}
void Solver(float a,float b,float c){
    float D = 0;
    float x1,x2 = 0;
    pid_t pid;
    D = b*b - 4*a*c;
    if (D < 0){
        pid = getpid();
        kill(pid,SIGUSR1);

    }
    if ( D == 0){
        x1 = (-b+ sqrt(D))/(2*a);
        printf("root: %f",x1);
    }
    if (D > 0  ){
        x1 = (-b+ sqrt(D))/(2*a);
        x2 =(-b-sqrt(D))/(2*a);
        printf("root: %f",x1);
        printf("\n");
        printf("root: %f\n",x2);
    }

}

int main() {
    int counter;
    char answ;
    int flag1 = 0;
    float d=0;int k =0;
    float * Arr = malloc(sizeof (float )* 3);
    float a = 0;
    float b = 0;
    float c = 0;
    signal(SIGUSR1,Handler);
    printf(" Hello, enter 3 coefs of square equation\n");
    if(scanf("%f%f%f",&a,&b,&c) == 3)
    Solver(a,b,c);
    else printf("invalid coefs...\n");

    fflush(stdout);
    while (1){
        flag1 = 0;
        printf("more? \n");
        printf("Y/N\n");

        scanf("%s",&answ); // continue or not
        if ( answ == 'N' || answ == 'n'){
            printf("GL!");
            break;
        }

        if ( answ == 'Y' || answ == 'y'){
            printf("OOOOkay here You go, waiting for your 3 next coefs :) \n");
            k = 0;
            while ( k<3){
               if( scanf("%f",&d) == 1) { //waiting coefs
                   Arr[k] = d;
                   k++;
               } else{
                   printf("invalid coefs, try again...\n");
                   flag1 = 1; // if coefs okay, can procede to Solver func
                   break;
               }

            }
            if (!flag1){
                a = Arr[0];
                b = Arr[1];
                c = Arr[2];
                Solver(a,b,c);
            }

        }
    }
    return 0;
}
