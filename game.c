/*Aldane Barrett

-------How to Play-------

-Execute code via terminal or IDE

-Select: 
l:Launch 
b:Drop a bomb based on process ID (Can only drop a bomb if plane is flying(Launched))
r:Refuel a plane based on process ID (Can only refuel if plane is flying(Launched))
q:Quit

Once launched, the plane losses fuel at a rate of 5 litre per second starting from 100 Litres
Refueling resets the fuel back to 100. You'll be asked which plane you would like to refuel based on it process ID
Once the fuel goes below 0, a warning message will show and the plane will crash
Droping a bomb is similar to refueling, you will have the option of dropping a bomb from a plane of your choice 
You can drop as many bombs before the fuel level goes below 0 and crashes. 
Press q to Quit.

*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#define MAX 12

char input[MAX];
char l, b, r, q;
int fuel_level = 100;
int current_fuel;
int j, k, i;
int plane;
int bomber_num, refuel_num;

/////////////////////////////////////////////
////////////*Signal Driver Code*/////////////
/////////////////////////////////////////////

void bomber(int sig)
{
    signal(SIGUSR1, bomber);
    printf("\n\t[+]Bomber %d to base, bombs away!\n", getpid());
}

void refuel(int sig)
{
    signal(SIGUSR2, refuel);
    fuel_level = 100;
    printf("\n\t[+]Plane %d has been refueled\n", getpid());
}

//////////////////////////////////////////////
////////////*Plane Driver Code*///////////////
//////////////////////////////////////////////

void printstar(int n)
{
    int i = 1;
    for (i = 1; i <= n; i++)
        printf("*");
}
void printspace(int n)
{
    int i = 1;
    for (i = 1; i <= n; i++)
        printf(" ");
}

void printplane(int level)
{
    int n = 2 * level - 1;
    int i, j = 0;
    for (i = 1; i <= level; i = i + 1)
    {
        if (i <= level / 2)
        {
            printspace(i);
            printstar(i);
        }
        else
        {
            printspace(level - i + 1);
            printstar(level - i + 1);
        }
        printf("\n");
    }
}
//////////////////////////////////////////////

int main(int argc, char const *argv[])
{
    signal(SIGUSR2, refuel); //initialize signals
    signal(SIGUSR1, bomber);
    int level = 10;

    while (1) //Infinite Loop
    {
        //////////////////////////////////////////////
        ///////////////////*Base*/////////////////////
        //////////////////////////////////////////////

        printf("\n[+]Enter a command: l-Launch, b-Bomb, r-Refuel, q-Quit:\n");
        fgets(input, MAX, stdin);

        if (input[0] == 'l')
        {

            plane = fork();
            if (plane < 0)
            {
                perror("Fork\n");
                exit(1);
            }
            if (plane == 0)
            {
                printf("\n");
                printplane(level); //Additional Code
                printf("\n[+] Plane %d was Launched!\n", getpid());
                for (int i; fuel_level >= i; i--)
                {
                    // current_fuel = fuel_level;
                    current_fuel = fuel_level -= 15;
                    sleep(3);
                    printf("\n[+]Bomber %d to base... %d %% of fuel left\n", getpid(), current_fuel);
                    if (current_fuel <= 0)
                    {
                        printf("\n[-] SoS! Plane %d is going to crash!\n", getpid());
                        sleep(1);
                        printf("\n\t[--Plane %d Crashed!--]\n", getpid());
                        exit(0);
                    }
                }
                break;
            }
        }
        else if (input[0] == 'r') //Refuel Command
        {
            printf("\n\t[-Refuel Command-]\n\t");
            printf("\n\t[-]Checking with base... \n\t");
            sleep(1);
            if (plane == 0) //
            {
                printf("\n\t[-]No plane to refuel!\n");
            }
            else
            {
                printf("[+]Which plane do you want to refuel?\n");
                scanf("%d", &refuel_num);

                if (refuel_num == plane)
                {
                    kill(plane, SIGUSR2);
                }
                else
                {
                    printf("\n\t[-]Plane %d doesn't exists!\n", refuel_num);
                }
            }
        }
        else if (input[0] == 'b')
        {
            printf("\n\t[-Bomber Called!-]\n\t");
            printf("\n\t[-]Checking with base... \n\t");
            sleep(1);
            if (plane == 0)
            {
                printf("\n[-]No plane if flying!\n");
            }
            else
            {
                printf("\n Which plane should drop a bomb?\n");
                scanf("%d", &bomber_num);

                if (bomber_num == plane)
                {
                    kill(plane, SIGUSR1);
                }
                else
                {
                    printf("\n\t[-]Plane %d doesn't exists!\n", bomber_num);
                }
            }
        }
        else if (input[0] == 'q')
        {
            printf("[-]Closing...\n");
            sleep(1);
            printf("[+]Closed!\n");
            exit(0);
        }
    }

    return 0;
}
