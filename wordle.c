#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
// culori si constante
#define VERDE 2
#define GALBEN 3
#define NR_LITERE 26
#define NR_LIT_CUV 7
#define Y_MESAJ 10
#define X_MESAJ 25
#define LINII_SUB 7
#define COL_SUB 6
#define NR_ALEGERI 2
#define LUNG_ALEGERI 11

void center(int linie, char *titlu) {
    int lungime, indent, y, latime;
    getmaxyx(stdscr, y, latime);
    lungime = strlen(titlu);
    indent = (latime - lungime) / 2;
    mvaddstr(linie, indent, titlu);
    // sa nu imi mai dea -Wall warning
    y = y;
    refresh();
}

void init_tabela(WINDOW *tabela[LINII_SUB][COL_SUB]) {
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 5; j++) {
            tabela[i][j] = subwin(stdscr, 3, 3, i * 3 + 4, j * 3 + 4);
            box(tabela[i][j], 0, 0);
        }
    }
}

int navigare_meniu(WINDOW *meniu, char algeri_meniu[NR_ALEGERI][LUNG_ALEGERI]) {
    int i, c, highlight = 0;
    while (1) {
        for (i = 0; i < 2; i++) {
            if (i == highlight) wattron(meniu, A_REVERSE);
            // arat in ce pozitie sunt si ce am in meniu
            wmove(meniu, i + 1, 1);
            wprintw(meniu, algeri_meniu[i]);
            wattroff(meniu, A_REVERSE);
        }
        c = wgetch(meniu);
        // navigare cu sagetile
        // si iesire cu enter
        if (c == KEY_UP) {
            highlight--;
            // sa nu ies din meniu
            if (highlight == -1) highlight = 0;
        }
        if (c == KEY_DOWN) {
            highlight++;
            // sa nu ies din meniu
            if (highlight == 2) highlight = 1;
        }
        if (c == 10) break;
    }
    return highlight;
}

void generator_random(int ceiling, int floor, int *random) {
    // imi aleg un numar random
    srand(time(NULL));
    // vreau sa fie in inervalul [floor, ceiling]
    *random = (rand() % (ceiling - floor + 1)) + floor;
}

void contorizare_litere(char cuvant_ales[NR_LIT_CUV],
                        int contor_litere[NR_LITERE]) {
    int i;
    // vad ce litere am in cuvant
    // si de cate ori apare fiecare
    for (i = 0; i < 5; i++) {
        contor_litere[cuvant_ales[i] - 'a']++;
    }
}

void transfer_date(int contor_litere[NR_LITERE],
                   int contor_litere_copie[NR_LITERE]) {
    int i;
    for (i = 0; i < 26; i++) {
        contor_litere_copie[i] = contor_litere[i];
    }
}

void color_litere(WINDOW *tabela[LINII_SUB][COL_SUB],
                  char cuvant_ales[NR_LIT_CUV], char cuvant_input[NR_LIT_CUV],
                  int contor_litere[NR_LITERE], int i) {
    int k, verde[NR_LIT_CUV] = {0};
    // vom parcurge tot cuvantul input
    // si vom vedea care litere sunt corecte
    // daca sunt verzi atunci punem 1 in vectorul verde
    // dupa aceea vom parcurge inca o data
    // pentru galben
    for (k = 0; k < 5; k++) {
        if (cuvant_input[k] == cuvant_ales[k]) {
            verde[k] = 1;
            wbkgd(tabela[i][k], COLOR_PAIR(VERDE));
            contor_litere[cuvant_ales[k] - 'a']--;
            wrefresh(tabela[i][k]);
        }
    }
    // acum am epuizat toate literele plasate corect
    // si trebuie sa le coloram pe cele care nu sunt
    for (k = 0; k < 5; k++) {
        // verificam daca mai am aparitii ale literei
        if ((contor_litere[cuvant_input[k] - 'a'] > 0) && (verde[k] == 0)) {
            // verificam daca litera este in cuvant
            if (strchr(cuvant_ales, cuvant_input[k]) != NULL) {
                wbkgd(tabela[i][k], COLOR_PAIR(GALBEN));
                contor_litere[cuvant_input[k] - 'a']--;
                wrefresh(tabela[i][k]);
            }
        }
    }
}

void sterg_mesaj() {
    wmove(stdscr, 10, 25);
    clrtoeol();
    wmove(stdscr, 10, 25);
}

void daca_e_litera(WINDOW *sub[LINII_SUB][COL_SUB], int i, int j, char litera,
                   char cuvant_input[NR_LIT_CUV]) {
    // daca pun litera mare
    // o transform in litera mica
    if (isupper(litera)) litera = tolower(litera);
    cuvant_input[j] = litera;
    // este o decizie de design
    // sa afisez literele mari
    wprintw(sub[i][j], "%c", toupper(litera));
    wrefresh(sub[i][j]);
    refresh();
}

void backspace(WINDOW *sub[LINII_SUB][COL_SUB], int i, int *j, int *contor) {
    if (*contor != 5) {
        // daca am introdus cel putin un caracter
        if (*j != 0) {
            wmove(sub[i][*j - 1], 1, 1);
            wprintw(sub[i][*j - 1], " ");
            wrefresh(sub[i][*j - 1]);
            *j = *j - 2;
            *contor = *contor - 1;
        } else {
            // daca am introdus 0/1 caractere
            *j = -1;
            *contor = 0;
            wmove(sub[i][*j + 1], 1, 1);
            wprintw(sub[i][*j + 1], " ");
            wrefresh(sub[i][*j + 1]);
        }
    } else {
        *contor = *contor - 1;
        wmove(sub[i][*j], 1, 1);
        wprintw(sub[i][*j], " ");
        wrefresh(sub[i][*j]);
        *j = *j - 1;
    }
}

void orice_alt_caracter_in_casute(WINDOW *sub[LINII_SUB][COL_SUB], int i,
                                  int *j, int contor) {
    // daca am introdus deja 5 caractere
    // nu mai pot introduce
    if (contor == 5) {
        wmove(stdscr, Y_MESAJ, X_MESAJ);
        wprintw(stdscr, "Doar ENTER, BACKSPACE sau ':'-");
        refresh();
    } else {
        // daca nu am introdus 5 caractere
        // pot introduce orice alt caracter
        wmove(stdscr, Y_MESAJ, X_MESAJ);
        wprintw(stdscr, "Introduceti doar litere!");
        refresh();
        // sterg caracterul curent
        if (*j != 0) {
            wmove(sub[i][*j], 1, 1);
            wprintw(sub[i][*j], " ");
            wrefresh(sub[i][*j]);
            *j = *j - 1;
        } else {
            // daca sunt la inceputul randului
            wmove(sub[i][*j + 1], 1, 1);
            wprintw(sub[i][*j + 1], " ");
            wrefresh(sub[i][*j + 1]);
            *j = -1;
        }
    }
};

void enter(WINDOW *sub[LINII_SUB][COL_SUB], int i, int *j, int *contor,
           char cuvant_input[7], char cuvant_ales[NR_LIT_CUV], int *ghicit,
           int contor_litere[NR_LITERE]) {
    // daca am introdus 5 caractere
    // pot verifica cuvantul
    if (*contor == 5) {
        // daca am introdus cuvantul corect
        if (strcmp(cuvant_input, cuvant_ales) == 0) {
            // daca am ghicit cuvantul
            // nu mai pot introduce litere
            *ghicit = 1;
            // sterg mesajul precedent
            // coloram cuvantul cu verde
            color_litere(sub, cuvant_ales, cuvant_input, contor_litere, i);
        } else {
            // ne apucam de litere
            // pentru galben conditia fiind
            // daca litera este in cuvant
            // dar nu pe pozitia corecta
            *ghicit = 0;
            color_litere(sub, cuvant_ales, cuvant_input, contor_litere, i);
        }
    } else {
        // daca nu am introdus 5 caractere
        // nu pot verifica cuvantul
        int k;
        wmove(stdscr, Y_MESAJ, X_MESAJ);
        // mesaj pentru jucator
        wprintw(stdscr, "Introduceti cuvant cu 5 litere!");
        refresh();
        // sterg mesajul precedent
        // pentru a nu se suprapune
        for (k = 0; k < 5; k++) {
            wmove(sub[i][k], 1, 1);
            wprintw(sub[i][k], " ");
            wrefresh(sub[i][k]);
            refresh();
        }
        *j = -1;
        *contor = 0;
    }
}

int main(int argc, char *argv[]) {
    // am creat lista de cuvinte si
    // acum trebuie sa aleg unul random
    // din intervaul [1, 27] -> minimul din cerinta
    FILE *lista_cuvinte;
    lista_cuvinte = fopen(argv[1], "r");
    char cuvant_ales[NR_LIT_CUV],
        algeri_meniu[NR_ALEGERI][LUNG_ALEGERI] = {"1.Iesire", "2.Restart"}, ch,
        cuvant_input[NR_LIT_CUV];
    int ceiling = 27, floor = 1, random, i, j, highlight, contor = 1,
        ghicit = 0, contor_litere[NR_LITERE] = {0},
        copie_contor_litere[NR_LITERE] = {0}, mesaj_time = 0;
    generator_random(ceiling, floor, &random);
    if (lista_cuvinte == NULL) {
        printf("Nu am gasit lista de cuvinte\n");
        return 1;
    }
    // citesc cuvintele din fisier
    // si ma opresc cand ajung la linia random
    // incep cu contor de la 1
    while (fgets(cuvant_ales, sizeof(cuvant_ales), lista_cuvinte) != NULL) {
        if (contor == random) break;
        contor++;
    }
    // vreau sa scot \n de la final
    cuvant_ales[strlen(cuvant_ales) - 1] = '\0';
    fclose(lista_cuvinte);
    // vreau sa contorizez cate si ce litere am
    contorizare_litere(cuvant_ales, contor_litere);
    // incep sa creez interfata
    initscr();
    start_color();
    cbreak();
    noecho();
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    // titlu + instructiuni
    center(1, "WORDLE");
    center(2, "ENTER pentru a incepe, ':' pentru meniu");
    center(3, "':' poate fi apelat oricand");
    box(stdscr, 0, 0);
    WINDOW *sub[LINII_SUB][COL_SUB], *meniu;
    init_tabela(sub);
    // definire meniu
    meniu = subwin(stdscr, 4, 20, 5, 25);
    // navighare meniu - cu arrow keys
    keypad(meniu, TRUE);
    box(meniu, 0, 0);
    wmove(meniu, 0, 1);
    wprintw(meniu, "Meniu");
    // primul caracter => ENTER sau :
    ch = getch();
    // sectiune meniu
    if (ch == ':') {
        highlight = navigare_meniu(meniu, algeri_meniu);
        if (highlight == 0) {
            // Iesire
            endwin();
            return 0;
        } else {
            if (highlight == 1) {
                // Restart
                system("./wordle_idei lista_cuvinte_wordle.txt");
                endwin();
                return 0;
            }
        }
    }
    // sectiune joc
    if (ch == 10) {
        for (i = 0; i < 6; i++) {
            // pentru fiecare cuvant input
            // trebuie sa resetez contorul
            // pentru a nu avea probleme
            transfer_date(contor_litere, copie_contor_litere);
            // contor pentru a verifica daca am introdus 5 litere
            contor = 0;
            // il resetez pentru fiecare linie
            if (ghicit == 1) break;
            for (j = 0; j < 5; j++) {
                // verificare daca am avut mesaj pentru
                // ultimul caracter introdus
                if (mesaj_time != 1) {
                    // daca nu am apasat ENTER inainte de 5 litere
                    sterg_mesaj();
                }
                // sunt in mijloc subwindow-ului
                // sa arate frumos si sa fie mai usor de vazut
                wmove(sub[i][j], 1, 1);
                ch = wgetch(sub[i][j]);
                // verific daca am introdus o litera
                if (isalpha(ch)) {
                    daca_e_litera(sub, i, j, ch, cuvant_input);
                    contor++;
                } else {
                    if (ch == 10) {
                        enter(sub, i, &j, &contor, cuvant_input, cuvant_ales,
                              &ghicit, copie_contor_litere);
                        mesaj_time = 1;

                    }  // vreau sa sterg ultimul caracter
                    if (ch == 127 || ch == KEY_BACKSPACE) {
                        backspace(sub, i, &j, &contor);
                    }
                    if (ch == 58) {
                        // daca am apasat ':'
                        highlight = navigare_meniu(meniu, algeri_meniu);
                        if (highlight == 0) {
                            // Iesire
                            endwin();
                            return 0;
                        } else {
                            if (highlight == 1) {
                                // Restart
                                system(
                                    "./wordle_idei "
                                    "lista_cuvinte_wordle.txt");
                                endwin();
                                return 0;
                            }
                        }
                    }
                    // verificam daca am apasat
                    // orice alt caracter
                    if (ch != 10 && ch != 127 && ch != KEY_BACKSPACE &&
                        ch != 58 && isalpha(ch) == 0) {
                        orice_alt_caracter_in_casute(sub, i, &j, contor);
                        mesaj_time = 1;
                    }
                }
                if (contor == 5) {
                    // am ajuns la "final de rand" si vreau
                    // verifcare enter / backspace / meniu
                    wmove(stdscr, Y_MESAJ, X_MESAJ);
                    ch = wgetch(stdscr);
                    if (ch == 10) {
                        // daca am apasat ENTER
                        // verific daca am ghicit cuvantul
                        // daca da => culoare verde
                        // daca am ghicit litere => culoare galbena
                        // ghicit = variabila care imi spune daca am ghicit
                        enter(sub, i, &j, &contor, cuvant_input, cuvant_ales,
                              &ghicit, copie_contor_litere);
                    }
                    if (ch == KEY_BACKSPACE || ch == 127) {
                        // daca am apasat backspace
                        backspace(sub, i, &j, &contor);
                    }
                    if (ch == 58) {
                        // daca am apasat ':'
                        highlight = navigare_meniu(meniu, algeri_meniu);
                        if (highlight == 0) {
                            // Iesire
                            endwin();
                            return 0;
                        } else {
                            if (highlight == 1) {
                                // Restart
                                system(
                                    "./wordle_idei "
                                    "lista_cuvinte_wordle.txt");
                                endwin();
                                return 0;
                            }
                        }
                    }
                    // verificam daca am apasat
                    // orice alt caracter
                    // in cazul in care am introdus ceva nepermis
                    // la verificare cuvant
                    // revin si ii mai dau o sansa
                    // sa puna ceva permis
                    if (ch != 10 && ch != 127 && ch != KEY_BACKSPACE &&
                        ch != 58) {
                        orice_alt_caracter_in_casute(sub, i, &j, contor);
                        // mai dam o sansa
                        ch = wgetch(stdscr);
                        if (ch == 10) {
                            enter(sub, i, &j, &contor, cuvant_input,
                                  cuvant_ales, &ghicit, copie_contor_litere);
                        }
                        if (ch == KEY_BACKSPACE || ch == 127) {
                            backspace(sub, i, &j, &contor);
                        }
                        if (ch == 58) {
                            highlight = navigare_meniu(meniu, algeri_meniu);
                            if (highlight == 0) {
                                // Iesire
                                endwin();
                                return 0;
                            } else {
                                if (highlight == 1) {
                                    // Restart
                                    system(
                                        "./wordle_idei "
                                        "lista_cuvinte_wordle.txt");
                                    endwin();
                                    return 0;
                                }
                            }
                        }
                    }
                }
                if (mesaj_time != 1) {
                    // daca nu am apasat ENTER inainte de 5 litere
                    sterg_mesaj();
                }
                mesaj_time = 0;
                refresh();
            }
        }
    }
    wmove(stdscr, Y_MESAJ, X_MESAJ);
    if (ghicit == 1) {
        wprintw(stdscr, "Bravo ai ghicit cuvantul! Vrei sa mai joci? (Y/N)");
        refresh();
        ch = wgetch(stdscr);
        if (ch == 'Y' || ch == 'y') {
            system("./wordle_idei lista_cuvinte_wordle.txt");
            endwin();
            return 0;
        } else {
            if (ch == ':') {
                highlight = navigare_meniu(meniu, algeri_meniu);
                if (highlight == 0) {
                    // Iesire
                    endwin();
                    return 0;
                } else {
                    if (highlight == 1) {
                        // Restart
                        system("./wordle_idei lista_cuvinte_wordle.txt");
                        endwin();
                        return 0;
                    }
                }
            } else {
                endwin();
                return 0;
            }
        }
    } else {
        wprintw(stdscr, "Nu ai ghicit cuvantul, acesta era: %s", cuvant_ales);
        refresh();
        ch = wgetch(stdscr);
        if (ch == ':') {
            int highlight = navigare_meniu(meniu, algeri_meniu);
            if (highlight == 0) {
                // Iesire
                endwin();
                return 0;
            } else {
                if (highlight == 1) {
                    // Restart
                    system("./wordle_idei lista_cuvinte_wordle.txt");
                    endwin();
                    return 0;
                }
            }
        } else {
            // Iesire
            endwin();
            return 0;
        }
    }
}