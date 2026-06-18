#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int verifica_data(int dia, int mes, int ano) {
    if(mes == 2) { //analisa dias de fevereiro com ano bissexto
        if(ano%4 == 0 && (ano%100 != 0 || ano%400 == 0)) {
            if(dia >= 1 && dia <= 29) {
                return 1;
            } else {
                return 0;
            }
        } else {
            if(dia >= 1 && dia <= 28) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    if(mes == 4 || mes == 6 || mes == 9 || mes == 11) { //analisa meses com 30 dias
        if(dia >= 1 && dia <= 30) {
            return 1;
        } else {
            return 0;
        }
    }
    if(dia >= 1 && dia <= 31) { //analisa meses com 31 dias
        return 1;
    } else {
        return 0;
    }
}