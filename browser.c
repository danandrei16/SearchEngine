#include <ncurses.h>
#include "task2si3.c"

void retin_culori(char culori[50], char text[10], char background[10]);
void asoc_culori(char culoare[10], color * culoare_enum);
void destroy_window(WINDOW * window) {
    // functie care sterge o fereastra (ii sterge doar continutul ptr ca pot fi refolosite dupa tasta 'b')
    wclear(window);
    wrefresh(window);
}
void Google(site * siteuri, int nr) {
    initscr();
    curs_set(0);
    keypad(stdscr, TRUE); // accepta si enter, sageti, etc
    noecho(); // nu afiseaza ce se va citi cu getch
    start_color(); // culori
    attron(A_BOLD);
    mvprintw(LINES / 2, (COLS - 6) / 2, "Google");
    attroff(A_BOLD);
    WINDOW * legenda = newwin(10, COLS * 0.9, LINES - 5, 2); // legenda cu posibile taste
    // Daca se apasa o tasta inafara legendei, programul asteapta input pana la o tasta corespunzatoare
    refresh();
    mvwprintw(legenda, 1, 0, "\r'C' - Cautare\n'Q' - Iesire");
    wrefresh(legenda);
    int tasta = getch();
    int tasta2, tasta3, tasta4;
    echo(); // de acum se afiseaza ce se citeste
    while (tasta != 'q') {
        switch (tasta) {
        case 'c':
            // 'c' ->  apare fereastra de cautare
            mvwprintw(legenda, 0, 0, "\rTastati textul dorit si apasati ENTER. Apoi:\n'S' - Cautare simpla\n'A' - Cautare avansata\n'Q'- Iesire\n\n");
            wrefresh(legenda);
            int width = COLS / 2;
            int height = LINES / 5;
            int startx = (COLS - width) / 2, starty = (LINES - height) / 2;
            WINDOW * google = newwin(height, width, starty - 2, startx); // fereastra care contine titlul Google
            mvwprintw(google, 0, (width - 6) / 2, "Google");
            wrefresh(google);
            WINDOW * border_win = newwin(height, width, starty, startx); // border pentru search engine
            WINDOW * win = newwin(height - 2, width - 2, starty + 1, startx + 1); // fereastra unde se tasteaza efectiv
            // ferestre separate pentru a evite suprascrierea pe border
            refresh();
            int x = (int)('*');
            wborder(border_win, 0, 0, 0, 0, x, x, x, x); // border cu * in colturi *aesthetic*
            wrefresh(border_win); // afiseaza border
            wmove(win, 0, 0);
            char caut[101]; // pres ca caut text de max 100 char
            wgetstr(win, caut); // tastez input-ul
            int len = strlen(caut);
            caut[len] = ' ';
            caut[len + 1] = '\n'; // spatiu si endline la finalul cautarii ptr a include si ultimul cuv
            noecho();
            tasta2 = getch(); // se apasa o noua tasta
            echo();
            while (1) {
                switch (tasta2) {
                case 's':
                case 'a':
                    // cazuri de cautare simpla sau avansata
                    tasta = 'c';
                    WINDOW * bara = newwin(LINES / 8, COLS / 1.25, 2, COLS / 10); // aici se afiseaza input-ul user-ului (pe pagina cu rezultatele)
                    wmove(bara, 1, 1);
                    wprintw(bara, caut);
                    refresh();
                    box(bara, 0, 0);
                    wrefresh(bara);
                    WINDOW * rezultate = newwin(LINES / 2, COLS / 1.25, LINES / 3, COLS / 10); // rezultatele obtinute
                    refresh();
                    int i;
                    int siteuri_afisate[100]; // pres ca sunt max 100 // retine X din siteX.html (ptr cele afisate)
                    int j = 0;
                    wmove(rezultate, 0, 0);
                    int m, * v = malloc(nr * sizeof(int)); // m=nr siteuri cu prop ceruta, v va contine indicii in ordinea ceruta
                    if (tasta2 == 's')
                        task2(caut, siteuri, nr, & m, v); // cautare simpla
                    else
                    if (tasta2 == 'a')
                        task3(caut, siteuri, nr, & m, v); // cautare avansata
                    for (i = 0; i < m; i++) {
                        // afisez titlu si url ptr site-urile care corespund
                        wprintw(rezultate, "%s %s\n", siteuri[v[i]].titlu, siteuri[v[i]].URL);
                        // retin indicii site-urilor in ordinea corespunzatoare
                        siteuri_afisate[j] = v[i];
                        j++;
                    }
                    free(v);
                    wrefresh(rezultate);
                    refresh();
                    if (j > 0) { // avem macar un rezultat
                        // deci se poate astepta input ^ sau v ptr scroll intre rezultate
                        mvwprintw(legenda, 0, 0, "\r^ - Scroll up\nv - Scroll down\nEnter - Deschideti pagina selectata\n'B' - Pagina anterioara\n'Q' - Iesire");
                        wrefresh(legenda);
                        keypad(rezultate, TRUE);
                        init_pair(1, COLOR_BLUE, COLOR_BLACK);
                        int k = 0; // implicit va fi selectat PRIMUL rezultat
                        int q = siteuri_afisate[k];
                        wmove(rezultate, 0, 0);
                        wclrtoeol(rezultate);
                        wattron(rezultate, COLOR_PAIR(1));
                        wprintw(rezultate, "%s %s", siteuri[q].titlu, siteuri[q].URL);
                        wattroff(rezultate, COLOR_PAIR(1));
                        wrefresh(rezultate);
                        noecho();
                        int n, ok = 0;
                        wmove(rezultate, 0, 0);
                        tasta3 = getch(); // voi citi UP sau DOWN, sau ENTER Q B
                        // ptr caz-urila key up si key down voi sterge randul si il rescriu cu format-ul pair 1
                        // ptr a indica faptul ca e selectat
                        while (1) {
                            switch (tasta3) {
                            case KEY_UP:
                                n = siteuri_afisate[k];
                                wmove(rezultate, k, 0);
                                wclrtoeol(rezultate);
                                wprintw(rezultate, "%s %s", siteuri[n].titlu, siteuri[n].URL);
                                if (k == 0)
                                    k = j - 1;
                                else
                                    k--;
                                n = siteuri_afisate[k];
                                wmove(rezultate, k, 0);
                                wattron(rezultate, COLOR_PAIR(1));
                                wclrtoeol(rezultate);
                                wprintw(rezultate, "%s %s", siteuri[n].titlu, siteuri[n].URL);
                                wattroff(rezultate, COLOR_PAIR(1));
                                wrefresh(rezultate);
                                tasta3 = getch();
                                break;
                            case KEY_DOWN:
                                n = siteuri_afisate[k];
                                wmove(rezultate, k, 0);
                                wclrtoeol(rezultate);
                                wprintw(rezultate, "%s %s", siteuri[n].titlu, siteuri[n].URL);
                                if (k == j - 1)
                                    k = 0;
                                else
                                    k++;
                                wmove(rezultate, k, 0);
                                n = siteuri_afisate[k];
                                wattron(rezultate, COLOR_PAIR(1));
                                wclrtoeol(rezultate);
                                wprintw(rezultate, "%s %s", siteuri[n].titlu, siteuri[n].URL);
                                wattroff(rezultate, COLOR_PAIR(1));
                                wrefresh(rezultate);
                                tasta3 = getch();
                                break;
                            case 'b':
                            case 'q':
                            case 10:
                            case KEY_ENTER:
                                ok = 1;
                                break;
                            default: // niciuna din tastele asteptate
                                tasta3 = getch(); // citesc pana intalnesc o tasta regasita in legenda
                                break;
                            }
                            if (ok == 1)
                                break;
                        }
                        echo();
                        if (tasta3 == 'q') {
                            tasta = 'q';
                            break;
                        }
                        if (tasta3 == 'b') {
                            destroy_window(bara);
                            destroy_window(rezultate);
                            tasta = 'c'; // ma intorc in modul de cautare
                        }
                        if (tasta3 == 10 || tasta3 == KEY_ENTER) {
                            // s-a apasat enter (key enter ar indicat enter de pe numpad si 10 cel normal)
                            mvwprintw(legenda, 0, 0, "\r\n\n\n'B' - Pagina anterioara\n'Q' - Iesire\n");
                            wrefresh(legenda);
                            // deschid linkul selectat
                            echo();
                            destroy_window(bara);
                            destroy_window(rezultate);
                            n = siteuri_afisate[k]; // X din siteX.html (cel deschis)
                            WINDOW * titlu = newwin(LINES / 8, COLS, 2, 0); // titlu
                            int len_titlu = strlen(siteuri[n].titlu);
                            wmove(titlu, 1, (COLS - len_titlu) / 2);
                            wattron(titlu, A_BOLD);
                            wprintw(titlu, siteuri[n].titlu);
                            wattroff(titlu, A_BOLD);
                            wrefresh(titlu);
                            WINDOW * continut = newwin(LINES / 2, COLS * 0.9, LINES / 3, 1); // continut p
                            init_pair(2, siteuri[n].text, siteuri[n].background);
                            wattron(continut, COLOR_PAIR(2));
                            mvwprintw(continut, 0, 5, siteuri[n].continut_p);
                            wattroff(continut, COLOR_PAIR(2));
                            wrefresh(continut);
                            refresh();
                            noecho();
                            tasta4 = getch(); // urmatoarea tasta apasata
                            while (1) {
                                switch (tasta4) {
                                case 'b':
                                case 'q':
                                    break;
                                default:
                                    tasta4 = getch();
                                    break;
                                }
                                if (tasta4 == 'b') {
                                    destroy_window(titlu);
                                    destroy_window(continut);
                                    destroy_window(rezultate);
                                    destroy_window(bara);
                                    delwin(rezultate);
                                    delwin(bara);
                                    wrefresh(titlu);
                                    wrefresh(continut);
                                    break;
                                } else
                                if (tasta4 == 'q') {
                                    tasta = 'q';
                                    break;
                                }
                            }
                            echo();
                            if (tasta4 == 'b' || tasta4 == 'q')
                                break;
                        }
                    } else { // niciun rezultat dupa cautare
                        mvwprintw(legenda, 0, 0, "\r'B' - Pagina anterioara\n'Q' - Iesire\n\n\n");
                        wrefresh(legenda);
                        noecho();
                        int tasta3 = getch();
                        while (1) {
                            switch (tasta3) {
                            case 'q':
                                tasta = 'q';
                                break;
                            case 'b':
                                tasta = 'c'; // inapoi la cautare
                                break;
                            default:
                                tasta3 = getch();
                                break;
                            }
                            refresh();
                            wrefresh(bara);
                            refresh();
                            if (tasta3 == 'b' || tasta3 == 'q') {
                                destroy_window(bara);
                                wrefresh(bara);
                                break;
                            }
                        }
                        echo();
                    }
                    break;
                case 'q':
                    tasta = 'q';
                    break;
                default:
                    break;
                }
                if (tasta4 != 'b') {
                    break;
                } else
                    tasta4 = 'o'; // valoare aleatorie dar sa nu fie 'b'
            }
            break;
        default:
            noecho();
            tasta = getch();
            echo();
            break;
        }
        refresh();
    }
    endwin();
}
int main() {
    FILE * fisier_site;
    char linie[31], continut_fisier[201], * p;
    int i, nr_siteuri = 7, aux, nr = 0;
    char siteNames[7][31] = {"sites/a_s_e.html", "sites/alabalaportocala.html", "sites/alt_site.html", "sites/facebook.html", "sites/important.html", "sites/tutorial_franceza.html", "sites/upb.html"};
    color * culori_siteuri_text = malloc(nr_siteuri * sizeof(color)); // alocare dinamica - enum
    color * culori_siteuri_bg = malloc(nr_siteuri * sizeof(color));
    site * siteuri = malloc(nr_siteuri * sizeof(site)); // alocare dinamica - struct
    for (i = 0; i < nr_siteuri; i++) {
        // citesc din master.txt
        char culori[50] = ""; // voi retine mai tarziu "color:...;background-color:..."
        int ghilimele = 0; //ptr proprietati color, bg color
        strcpy(linie, siteNames[i]);
        fisier_site = fopen(linie, "r"); // deschide fisier siteX.html
        fgets(continut_fisier, 201, fisier_site); // citesc prima linie din fisierul siteX.html
        int dim = 101;
        siteuri[i].continut_fisier = malloc(sizeof(site) * dim * sizeof(char));
        strcpy(siteuri[i].continut_fisier, ""); // camp initial vid
        strcat(siteuri[i].continut_fisier, continut_fisier); // concatenez prima linie a fisierului
        p = strtok(continut_fisier, " ");
        strcpy(siteuri[i].URL, p); // retin URL
        p = strtok(NULL, " ");
        aux = atoi(p);
        siteuri[i].nr_octeti = aux; // retin nr de octeti
        p = strtok(NULL, " ");
        aux = atoi(p);
        siteuri[i].nr_accesari = aux; // retin nr de accesari
        p = strtok(NULL, " ");
        aux = atoi(p);
        siteuri[i].checksum = aux; // retin checksum
        while (fgets(continut_fisier, 101, fisier_site)) { // citeste din fisierul siteX.html
            dim *= 2;
            siteuri[i].continut_fisier = realloc(siteuri[i].continut_fisier, sizeof(site) * dim * sizeof(char));
            strcat(siteuri[i].continut_fisier, continut_fisier); // concatenez linie cu linie tot fisierul
        }
        int okTitle = 0, okP = 0, okHTML = 0; // bool -> 1 cand ma aflu in tag-ul respectiv, 0 cand nu
        siteuri[i].continut_html = malloc(sizeof(site) * dim * sizeof(char));
        strcpy(siteuri[i].continut_html, ""); // initial vid
        siteuri[i].continut_p = malloc(sizeof(site) * dim * sizeof(char));
        strcpy(siteuri[i].continut_p, ""); // initial vid
        strcpy(siteuri[i].titlu, ""); // cu dimensiune deja declarata
        char * copie_continut = malloc(strlen(siteuri[i].continut_fisier) + 1);
        strcpy(copie_continut, siteuri[i].continut_fisier);
        p = strtok(copie_continut, " ");
        while (p) {
            // parcurg intreg continutul fisierului siteX.html
            // verific daca gasesc tag-urile html, title, p
            char * html_start = strstr(p, "<html>"), * html_end = strstr(p, "</html>");
            if (html_start) {
                okHTML = 1; // intru in <html>
                int d = html_start - p;
                char * aux = malloc(strlen(p) + 1);
                strcpy(aux, p);
                strcpy(p, aux + d); // daca am "abc<html>..." voi avea doar "<html>..."
                free(aux);
            }
            if (okHTML) {
                strcat(siteuri[i].continut_html, p); // primeste inclusiv </html> daca se gaseste
                strcat(siteuri[i].continut_html, " ");
                if (html_end)
                    okHTML = 0; // ies din <html>
            }
            p = strtok(NULL, " ");
        }
        int len = strlen(siteuri[i].continut_html);
        siteuri[i].continut_html[len - 1] = '\0';
        strcpy(copie_continut, siteuri[i].continut_fisier);
        p = strtok(copie_continut, " ");
        while (p) {
            char * q = malloc(101);
            strcpy(q, p);
            char * title_start = strstr(q, "<title>"), * title_end = strstr(q, "</title>");
            if (title_start) {
                okTitle = 1; // intru in <title>
                strcpy(p, title_start + strlen("<title>")); // daca am "abc<title>Titlu" voi avea doar "Titlu"
                if (title_end) {
                    char * sterg = strchr(p, '<');
                    p[sterg - p] = '\0';
                    strcat(siteuri[i].titlu, p); // primeste cuvant fara tag
                    strcat(siteuri[i].titlu, " ");
                    okTitle = 0;
                }
            }
            if (okTitle) {
                if (title_end) {
                    char * sterg = strchr(p, '<');
                    p[sterg - p] = '\0';
                    okTitle = 0;
                }
                strcat(siteuri[i].titlu, p);
                strcat(siteuri[i].titlu, " ");
            }
            free(q);
            p = strtok(NULL, " ");
        }
        len = strlen(siteuri[i].titlu);
        siteuri[i].titlu[len - 1] = '\0';
        strcpy(copie_continut, siteuri[i].continut_fisier);
        p = strtok(copie_continut, " ");
        while (p) {
            char * p_start = strstr(p, "<p"), * p_end = strstr(p, "</p>"); // p poate avea optiuni
            if (p_start) {
                okP = 1;
                int d = p_start - p;
                char * aux = malloc(strlen(p) + 1);
                strcpy(aux, p);
                strcpy(p, aux + d); // daca am "abc<p>..." voi avea doar "<p>..."
                free(aux);
            }
            if (okP == 1) {
                char * end_p = strchr(p, '>'); // caut unde se inchide tag-ul <p style ...
                // poate am proprietati color/ bg color
                if (strchr(p, '"')) {
                    if (strchr(p, '"') == strrchr(p, '"')) {
                        if (ghilimele == 0) {
                            strcat(culori, p);
                            ghilimele = 1;
                        } else {
                            strcat(culori, p);
                            ghilimele = 0;
                        }
                    } else {
                        // fara spatii intre " "
                        strcat(culori, p);
                    }
                } else
                if (ghilimele == 1)
                    strcat(culori, p);
                if (end_p) {
                    strcpy(p, end_p + 1); // retin ce e dupa <p..>
                    okP = 2;
                }
            }
            if (okP == 2) {
                if (p_end) {
                    int d = p_end - p;
                    p[d] = '\0';
                }
                strcat(siteuri[i].continut_p, p);
                strcat(siteuri[i].continut_p, " ");
            }
            p = strtok(NULL, " ");
        }
        char text[10], background[10];
        retin_culori(culori, text, background);
        asoc_culori(text, & culori_siteuri_text[nr]);
        asoc_culori(background, & culori_siteuri_bg[nr]);
        siteuri[nr].text = culori_siteuri_text[nr];
        siteuri[nr].background = culori_siteuri_bg[nr];
        len = strlen(siteuri[i].continut_p);
        siteuri[i].continut_p[len - 1] = '\0';
        nr++;
        free(copie_continut); // eliberez memorie ptr copia fisierului
        fclose(fisier_site);
    }
    //AICI REZOLV TASK 5 
    Google(siteuri, nr);
    //AICI ELIBEREZ MEMORIE ptr struct
    for (i = 0; i < nr; i++) {
        free(siteuri[i].continut_fisier);
        free(siteuri[i].continut_html);
        free(siteuri[i].continut_p);
    }
    free(culori_siteuri_text);
    free(culori_siteuri_bg);
    free(siteuri);
    return 0;
}
void retin_culori(char culori[50], char text[10], char background[10]) {
    char color[50] = "", bg[50] = "";
    char * p = strstr(culori, "background-color:");
    if (p) {
        culori[p - culori + 13] = 'x'; // bg color -> bg coxor (vr sa mi gaseasca color simplu dupa)
        strcpy(bg, p + strlen("background-color") + 1);
        char * x = strchr(bg, ';');
        bg[x - bg] = '\0';
    }
    char * q = strstr(culori, "color:");
    if (q) {
        strcpy(color, q + strlen("color") + 1);
        char * x = strchr(color, ';');
        color[x - color] = '\0';
    }
    if (strcmp(color, "") == 0)
        strcpy(text, "black");
    else
        strcpy(text, color);
    if (strcmp(bg, "") == 0)
        strcpy(background, "white");
    else
        strcpy(background, bg);
}
void asoc_culori(char culoare[10], color * culoare_enum) {
    if (strcmp(culoare, "white") == 0)
        * culoare_enum = white;
    if (strcmp(culoare, "black") == 0)
        * culoare_enum = black;
    if (strcmp(culoare, "red") == 0)
        * culoare_enum = red;
    if (strcmp(culoare, "green") == 0)
        * culoare_enum = green;
    if (strcmp(culoare, "blue") == 0)
        * culoare_enum = blue;
    if (strcmp(culoare, "yellow") == 0)
        * culoare_enum = yellow;
}
