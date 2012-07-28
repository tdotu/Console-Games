/*
	 Copyright 2012 Simon Diepold all rights reserved

	 Mastermind is free software: you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation, either version 3 of the License, or
	 any later version.

	 Mastermind is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Universe Kernel.  If not, see <http://www.gnu.org/licenses/>.


	 Mastermind ist Freie Software: Sie koennen es unter den Bedingungen
	 der GNU General Public License, wie von der Free Software Foundation,
	 Version 3 der Lizenz oder jeder spaeteren
	 veroeffentlichten Version, weiterverbreiten und/oder modifizieren.

	 Mastermind wird in der Hoffnung, dass es nuetzlich sein wird, aber
	 Universe Kernel wird in der Hoffnung, dass es nuetzlich sein wird, aber
	 OHNE JEDE GEWAEHELEISTUNG, bereitgestellt; sogar ohne die implizite
	 Gew‰hrleistung der MARKTFAEHIGKEIT oder EIGNUNG FUER EINEN BESTIMMTEN ZWECK.
	 Siehe die GNU General Public License fuer weitere Details.

	 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
	 Programm erhalten haben. Wenn nicht, siehe <http://www.gnu.org/licenses/>.
 */


 /*
    authors:
    -Simon Diepold aka. Tdotu (Universe Team) simon.diepold@infinitycoding.de


 */


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <winuser.h>



#define richtig 1
#define platz 0

static char computer[8] = {0,0,0,0,0,0,0,0};
static char backup[8];
static char player[8] = {0,0,0,0,0,0,0,0};
static char result[8] = {0,0,0,0,0,0,0,0};
static char rounds[3]={7,5,5};
COORD position;
HANDLE cmdbuffer;
int versuch=0;
int level[3]={4,4,8};
int ziffern;
int schwierigket;

void set_posotion(short x, short y){
    position.X=x;
    position.Y=y;
    SetConsoleCursorPosition(cmdbuffer,position);
}

void random(char* arry){
    srand(time(NULL));
    while (arry[0]<=0 ||arry[0]>9){
        arry[0] = rand()%9+1;
    }
    while (arry[1] == arry[0] || arry[1] == 0|| arry[1]>9 || arry[1]<=0){
        srand(time(NULL));
        arry[1] = rand()%9+1;
    }
    while (arry[2] == 0 || arry[2] == arry[0] || arry[2] == arry[1]|| arry[2]>9 || arry[2]<=0){
        srand(time(NULL));
      arry[2] = rand()%9+1;
    }
    while (arry[3] ==0 || arry[3] == arry[0]|| arry[3] == arry[1]|| arry[3] == arry[2] || arry[3]>9 || arry[3]<=0 ){
        srand(time(NULL));
       arry[3] = rand()%9+1;
    }
}

void randomLV2(char* arry){
    srand(time(NULL));
    arry[0]=rand()%9+1;
    arry[1]=rand()%9+1;
    arry[2]=rand()%9+1;
    arry[3]=rand()%9+1;
}

void randomLV3(char* arry){
    srand(time(NULL));
    arry[0]=rand()%9+1;
    arry[1]=rand()%9+1;
    arry[2]=rand()%9+1;
    arry[3]=rand()%9+1;
    arry[4]=rand()%9+1;
    arry[5]=rand()%9+1;
    arry[6]=rand()%9+1;
    arry[7]=rand()%9+1;
}

void icmd (void){
    system("cls");
    SetConsoleCP(1252);
    position.X=0;
    position.Y=0;
    cmdbuffer=GetStdHandle(-11);
}

void dump_pc(void){
    printf("%d",backup[0]);
    printf("%d",backup[1]);
    printf("%d",backup[2]);
    printf("%d",backup[3]);
    if(ziffern==8){
        printf("%d",backup[4]);
        printf("%d",backup[5]);
        printf("%d",backup[6]);
        printf("%d\n",backup[7]);
    }else{
        printf("\n");
    }
}

void dump_player(void){
    printf("%d",player[0]);
    printf("%d",player[1]);
    printf("%d",player[2]);
    printf("%d",player[3]);
    if(ziffern==8){
        printf("%d",player[4]);
        printf("%d",player[5]);
        printf("%d",player[6]);
        printf("%d\n",player[7]);
    }else{
        printf("\n");
    }
}

void playerinput(char* arry,int lengh){
    int i = 0;
    char temp;
    while(i<8){
        arry[i]=0; i++;
    }
    i = 0;
        while(1){
            temp=getch();
            if(temp>0x30 && temp<0x3a && i<lengh){
                arry[i]=temp;
                printf("%c",temp);
                i++;
            }else{
                if(temp=='\b'){
                        if(i!=0){
                            set_posotion((i-1),(versuch+1));
                            printf(" ");
                            set_posotion((i-1),(versuch+1));
                            i--;
                        }
                }
                else if(temp==0x0d && i==lengh){
                    break;
                }
            }
        }
    i=0;
    while(i<4){
        arry[i]-=0x30; i++;
    }
}

int check (int lengh){
    int i=0;
    int x=0;
    while(i<lengh){
        if(player[i]==computer[i]){
            result[x]=1;
            x++;
            player[i]=0;
            computer[i]=0;
        }
        i++;
    }
    return x;
}

int compare(int lengh){
    int i=0;
    int y=0;
    int x=0;
    while(i<lengh){
        if(player[i]==0){
            i++;
        }else{
            x=0;
            while(x<lengh){
                    if(player[i]==computer[x]){
                        y++;
                        computer[x]=0;
                        player[i]=0;
                        break;
                    }else{
                        x++;
                    }
            }
        }
        i++;
    }
    return y;
}

void xprintf(void){
    int i=0;
    while(i<ziffern){
        printf("X");
        i++;
    }
    printf("\n");
}




int main (void){
    icmd();
    start:;
    schwierigket=0;
    //standart frontpage
    system("title Mastermind");
    printf("Mastermind Programmiert von Simon Diepold\n");
    printf("drcke eine belibige Taste...");
    getchar();
    system("cls");
    printf("Whlen sie eine schwirigkeits Stufe:\n"
           "Einfach(1)        [4 Stellen, keine gleichen Zahlen,8 Versuche]\n"
           "Herausfordernd(2) [4 Stellen, 6 Versuche]\n"
           "Schwer(3)         [8 Stellen, 6 Versuche]\n"
           "Beenden(4)        [beendet das Programm]\n"
           );

    while(1){
        schwierigket=getchar();
        if(schwierigket==0x31 ||schwierigket==0x32 || schwierigket==0x33 || schwierigket==0x34){
            if(schwierigket==0x34){
                goto end;
            }
            schwierigket-=0x30;
            ziffern=level[(schwierigket-1)];
            break;
        }
    }
    system("cls");

    //zufallsgenerator
    if(schwierigket==1){
        random(backup);
    }
    else if (schwierigket==2){
        randomLV2(backup);
    }
    else if (schwierigket==3){
        randomLV3(backup);
    }

    xprintf();
    versuch = 0;
    beginn:;
    int i=0;
    int x=0;
    while(i<8){
        result[i]=0; i++;
    }
    i=0;
    while(i<8){
        computer[i]=backup[i];
        i++;
    }
    i=0;
    playerinput(player,ziffern);
    x=check(ziffern);

    if (x==ziffern){
        printf("\ndu hast gewonnen! Gratulation!");
        getch();
        system("cls");
        goto start;
    }else{
    x+=compare(ziffern);
    set_posotion((ziffern+1),(versuch+1));

    i=0;
    while(i<x){
        result[i]+=0x30;
        printf("%c",result[i]);
        i++;
    }
    printf(" \n");

    if(versuch==rounds[schwierigket-1]){
            printf("\nleider verloren. Die Lsung ist:");
            dump_pc();
            getch();
            system("cls");
            goto start;
        }
    versuch++;
    goto beginn;
    }
    end:;
    return 0;
}
