#include <stdlib.h>
#include <ctype.h>
#include "sortare.h"

void task2(char cuv_cheie[101], site * siteuri, int n, int * m, int * v) {
    char cuv_cheie_copie[101], cuvinte_cheie[101][101]; // citesc cuv_cheie si le retin in cuvinte_cheie
    int i, j, nr_cuv_cheie = 0; // momentan 0 cuvinte
    strcpy(cuv_cheie_copie, cuv_cheie);
    char * p = strtok(cuv_cheie_copie, " ");
    while (p) {
        // retin cuvintele
        strcpy(cuvinte_cheie[nr_cuv_cheie], p);
        nr_cuv_cheie++;
        p = strtok(NULL, " ");
    }
    cuvinte_cheie[nr_cuv_cheie - 1][strlen(cuvinte_cheie[nr_cuv_cheie - 1]) - 1] = '\0'; // elimin ultimul \n
    site * siteuri_rezultate = malloc((n + 1) * sizeof(site)); // structura in care retin info despre site-urile rezultate
    int maxim = 0;
    for (i = 0; i < n; i++) {
        int len = strlen(siteuri[i].continut_p);
        if (len > maxim)
            maxim = len;
        siteuri_rezultate[i].continut_p = malloc(sizeof(site) * (len + 1));
        // aloc dinamic
    }
    // aloc dinamic un spatiu aditional pentru interschimbari, cu dimensiunea celui mai mare paragraf
    siteuri_rezultate[n].continut_p = malloc(sizeof(site) * (maxim + 1));
    int a = 0;
    for (i = 0; i < n; i++) {
        // parcurg TOATE site-urile si retin site-urile care trebuie sortate (cele care respecta cautarea) 
        for (j = 0; j < nr_cuv_cheie; j++) {
            char * str = strstr(siteuri[i].continut_p, cuvinte_cheie[j]);
            int prev_char = str - siteuri[i].continut_p;
            int next_char = strlen(cuvinte_cheie[j]);
            if (str && !(isalpha(siteuri[i].continut_p[prev_char - 1])) && !(isalpha(str[next_char]))) {
                // verific daca gasesc cuvantul SI daca nu se afla in alt cuvant (ex. ati, facultati)
                // totusi "ati" s-ar gasi si in cazul "ati!" de ex.
                strcpy(siteuri_rezultate[a].continut_p, siteuri[i].continut_p);
                strcpy(siteuri_rezultate[a].URL, siteuri[i].URL);
                siteuri_rezultate[a].nr_accesari = siteuri[i].nr_accesari;
                siteuri_rezultate[a].indice = i;
                a++;
                break;
            }
        }
    }
    * m = a;
    sortare(siteuri_rezultate, a, n, ord2); // sortez dupa criteriul cerut
    for (i = 0; i < a; i++)
        v[i] = siteuri_rezultate[i].indice; // retin indicii in ordinea sortarii respective
    // retinand indicii in ordinea potrivita ma va ajuta la task5
    for (i = 0; i <= n; i++)
        free(siteuri_rezultate[i].continut_p);
    free(siteuri_rezultate);
}
void task3(char cuv_cheie[101], site * siteuri, int n, int * m, int * v) {
    char cuv_cheie_copie[101], cuvinte_cheie[101][101]; // citesc cuv_cheie si le retin in cuvinte_cheie
    int i, j, nr_cuv_cheie = 0; // momentan 0
    strcpy(cuv_cheie_copie, cuv_cheie);
    char * p = strtok(cuv_cheie_copie, " ");
    int ghilimele = 0; // 0- nu ma aflu in" ", 1 - ma aflu
    while (p) {
        // retin cuvintele / frazele
        int l = strlen(p);
        // cazul "frazelor"; voi retine tot ce este intre ghilimele (inclusiv) pe un rand al matricei
        // cazul uzual si cel cu '-' se retin asa cum sunt
        if (p[0] == '"')
            ghilimele = 1;
        if (ghilimele == 0 || p[0] == '"')
            strcpy(cuvinte_cheie[nr_cuv_cheie], p);
        else
        if (ghilimele == 1 || p[l - 1] == '"') {
            strcat(cuvinte_cheie[nr_cuv_cheie], " ");
            strcat(cuvinte_cheie[nr_cuv_cheie], p);
        }
        if (p[l - 1] == '"')
            ghilimele = 0;
        if (ghilimele == 0)
            nr_cuv_cheie++;
        p = strtok(NULL, " ");
    }
    if (ghilimele == 1)
        nr_cuv_cheie++;
    cuvinte_cheie[nr_cuv_cheie - 1][strlen(cuvinte_cheie[nr_cuv_cheie - 1]) - 1] = '\0'; // elimin ultimul \n
    site * siteuri_rezultate = malloc((n + 1) * sizeof(site)); // structura in care retin info despre site-urile rezultate
    int len, maxim = 0;
    for (i = 0; i < n; i++) {
        len = strlen(siteuri[i].continut_p);
        if (len > maxim)
            maxim = len;
        siteuri_rezultate[i].continut_p = malloc(sizeof(site) * (len + 1));
    }
    siteuri_rezultate[n].continut_p = malloc(sizeof(site) * (maxim + 1)); // aloc un spatiu in plus ptr interschimbari
    int a = 0;
    for (i = 0; i < n; i++) {
        // parcurg TOATE site-urile
        int ok = 0, ok_minus = 1;
        // ok=1 cand se gaseste minim un cuvant uzual sau cu " "
        // ok_minus se face 0 atunci cand se gaseste un cuvant cu -
        // deci cerinta este indeplinita cand ambele sunt 1
        for (j = 0; j < nr_cuv_cheie; j++) {
            char cuv[101];
            char * str;
            int prev_char, next_char;
            strcpy(cuv, cuvinte_cheie[j]);
            switch (cuv[0]) {
            case '-':
                // cuvinte care nu trb incluse
                str = strstr(siteuri[i].continut_p, cuv + 1);
                prev_char = str - siteuri[i].continut_p;
                next_char = strlen(cuv);
                if (str && strchr(" \n", siteuri[i].continut_p[prev_char - 1]) && strchr(" \n", str[next_char]))
                    ok_minus = 0;
                break;
            case '"':
                len = strlen(cuv);
                cuv[len - 1] = '\0'; // elimin ultima "
                str = strstr(siteuri[i].continut_p, cuv + 1);
                prev_char = str - siteuri[i].continut_p;
                next_char = strlen(cuv);
                if (str && !(isalpha(siteuri[i].continut_p[prev_char - 1])) && !(isalpha(str[next_char - 1])))
                    ok = 1;
                break;
            default:
                str = strstr(siteuri[i].continut_p, cuv);
                prev_char = str - siteuri[i].continut_p;
                next_char = strlen(cuv);
                if (str && strchr(" \n", siteuri[i].continut_p[prev_char - 1]) && strchr(" \n", str[next_char]))
                    ok = 1;
                break;
            }
        }
        if (ok == 1 && ok_minus == 1) {
            // site-ul corespunde
            strcpy(siteuri_rezultate[a].continut_p, siteuri[i].continut_p);
            strcpy(siteuri_rezultate[a].URL, siteuri[i].URL);
            siteuri_rezultate[a].nr_accesari = siteuri[i].nr_accesari;
            siteuri_rezultate[a].indice = i;
            a++;
        }
    }
    * m = a;
    sortare(siteuri_rezultate, a, n, ord3); // sortez dupa functia comparator potrivit task3
    for (i = 0; i < a; i++)
        v[i] = siteuri_rezultate[i].indice; // retin indicii in ordinea potrivita pentru a afisa site-urile
    for (i = 0; i <= n; i++)
        free(siteuri_rezultate[i].continut_p);
    free(siteuri_rezultate);
}
