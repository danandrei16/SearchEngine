#include <stdio.h>
#include <string.h>
#include "header.h"

int ord3(site site1, site site2) {
    return (site1.nr_accesari < site2.nr_accesari); // = 1 daca sunt descrescatore dupa nr_accesari (task3)
}
int ord2(site site1, site site2) {
    if (strcmp(site1.continut_p, site2.continut_p) == 0) // trecem la criterul 2 ptr ca continutul e identic
        return (site1.nr_accesari < site2.nr_accesari); // = 1 daca sunt cresc dupa nr_accesari (task2)
    return (strcmp(site1.continut_p, site2.continut_p) > 0); // = 1 daca sunt cresc alfabetic (criteriul 1)
}
void sortare(site * siteuri_cuv_cheie, int a, int n, int( * ord)(site, site)) {
    // functie care interschimba paragraful, URL, nr accesari si indicele respectiv (atunci cand e cazul)
    // ordinea indicilor obtinuta va determina ordinea in care se afiseaza site-urile
    int i, j;
    for (i = 0; i < a - 1; i++)
        for (j = i + 1; j < a; j++)
            if (ord(siteuri_cuv_cheie[i], siteuri_cuv_cheie[j]) == 1) {
                strcpy(siteuri_cuv_cheie[n].continut_p, siteuri_cuv_cheie[i].continut_p);
                strcpy(siteuri_cuv_cheie[i].continut_p, siteuri_cuv_cheie[j].continut_p);
                strcpy(siteuri_cuv_cheie[j].continut_p, siteuri_cuv_cheie[n].continut_p);
                strcpy(siteuri_cuv_cheie[n].URL, siteuri_cuv_cheie[i].URL);
                strcpy(siteuri_cuv_cheie[i].URL, siteuri_cuv_cheie[j].URL);
                strcpy(siteuri_cuv_cheie[j].URL, siteuri_cuv_cheie[n].URL);
                siteuri_cuv_cheie[n].nr_accesari = siteuri_cuv_cheie[i].nr_accesari;
                siteuri_cuv_cheie[i].nr_accesari = siteuri_cuv_cheie[j].nr_accesari;
                siteuri_cuv_cheie[j].nr_accesari = siteuri_cuv_cheie[n].nr_accesari;
                siteuri_cuv_cheie[n].indice = siteuri_cuv_cheie[i].indice;
                siteuri_cuv_cheie[i].indice = siteuri_cuv_cheie[j].indice;
                siteuri_cuv_cheie[j].indice = siteuri_cuv_cheie[n].indice;
            }
}
