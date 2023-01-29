typedef struct {
    char URL[51], titlu[51];
    char * continut_p; // ce se afla intre <p> ... </p> (exclusiv tagurile)
    int nr_octeti, nr_accesari, checksum;
    // campuri impuse de enunt
    char * continut_html; //  <html> .. </html> (inclusiv tagurile) -> ptr task4
    char * continut_fisier; // TOT ce se afla in fisierul siteX.html
    int indice; // task2,3,5 
    int text, background; // task 5 
} site;
typedef enum {
    // coduri culori conform ncurses
    black = 0, red = 1, green = 2, yellow = 3, blue = 4, white = 7
} color;
