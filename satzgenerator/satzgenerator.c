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
    -Simon Diepold aka. Tdotu simon.diepold@infinitycoding.de


 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *gets(char *puffer);
char *fgets(char *puffer, int n, FILE *datei);

int A = 0;
int B = 0;
int C = 0;
int D = 0;
int end = 0;

FILE *src;


char *get_rand_string(char* buff, int n, char area){
    int area_beginn = 0;
    int area_end = 0;
    switch(area){

    case 'A':
    area_beginn = A;
    area_end = B-1;
    break;

    case 'B':
    area_beginn = B;
    area_end = C-1;
    break;

    case 'C':
    area_beginn = C;
    area_end = D-1;
    break;

    case 'D':
    area_beginn = D;
    area_end = end-1;
    break;

    default:
    return 0;
    break; //just to keeep conventions

    }

    int line = (rand()%(area_end-area_beginn))+area_beginn;
    fseek(src, 0L, SEEK_SET);
    int i;
    for(i = 0; i <= line; i++){
        if(i == line){
            int x;
            for(x = 0; x < n; x++){
                buff[x] = 0;
            }
        }
        fgets(buff, n, src);
    }

    return buff;
}

int main(void){
    system("title Satzgenerator");
    src = fopen("config.txt", "r");
    char buffer[256];

    if(src == NULL){
        printf("Die Datei config.txt konnte nicht gefunden werden!\nKopieren sie diese Datei bitte in ihr aktuelles Arbeitsverzeichniss oder\nerstellen sie diese bitte neu.\n");
        system("pause"); //Windows Only
        return 1;
    }

    int i,n;
    for(i = 0; i < 256; i++){
        buffer[i] = 0;
    }

    n = 0;
    while (fgets(buffer, 256, src) != NULL){
        switch(buffer[0]){

            case 'A':
            if(buffer[1] == ':'){
                    A = n+1;
            }
            break;

            case 'B':
            if(buffer[1] == ':'){
                    B = n+1;
            }
            break;

            case 'C':
            if(buffer[1] == ':'){
                    C = n+1;
            }
            break;

            case 'D':
            if(buffer[1] == ':'){
                    D = n+1;
            }
            break;

        }

        for(i = 0; i < 256; i++){
            buffer[i] = 0;
        }

        n++;
    }

    end = n;
    fseek(src, 0L, SEEK_SET);

    srand(time(NULL));
    while(1){


        char input[256];
        for(i = 0; i < 256; i++){
            input[i] = 0;
        }

        fgets(input, 256, stdin);

        if( strncmp(input, "ende", 4) == 0){
            return 0;
        }

        i = 0;
        char c;
        while(input[i] && input[i] !='\n'){
            if(islower(input[i])){
                c = input[i]-0x20;
            }else{
                c = input[i];
            }

            if(c < 0x41 || c > 0x44){
                printf("inkorrekte Eingabe\n");
                break;
            }
            get_rand_string(buffer, 256, c);
            int x;
            for(x = 0; buffer[x] != '\n'; x++){}
            buffer[x] = 0;
            printf("%s ",buffer);
            i++;
        }
        printf("\n");
    }
}
