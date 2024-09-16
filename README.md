# WORDLE
## Indicatii folosire
Jocul are nevoie de un fisier text care contine lista de cuvinte din care poate
alege la intamplare unul castigator. Eu am folosit doar cuvintele din cerinta (
27 la numar ). Fisierul este atasat dupa indicatiile de pe forum.

## Cat mi-a luat 
As presupune ca am stat cam 3-4 zile pe acest joc. 

## Decizii Aspect
Asa cum este si in jocul original, literele sunt afisate ca majuscule,
indiferent de natura lor initiala. 

Am decis sa centrez titlul jocului, indicatiile pentru pornire si un sfat prin
care jucatorul stie ca poate accesa meniul oricand

Am decis ca ecranul principal sa fie incojurat de un contur alb.

Meniul de control am decis sa fie navigat cu arrow keys, deoarece mi se parea
cel mai firesc. 

Am decis ca marimea casutelor sa fie 3x3 pentru a putea centra in (1,1).

Am decis arbitrar ca pozitia unde apar mesaje menite pentru jucator sa fie
(Y_MESAJ, X_MESAJ). 

## Explicatie 
Am decis sa folosesc regulile jocului original, care difera un pic de cele din
cerinta, in ceea ce priveste ce litere sunt colorate si cand.

Daca in comanda de executie nu apare lista de cuvinte, programul nu va porni si
se va afisa un mesaj care indica eroarea.

Inainte de a porni interfata grafica, trebuie ales cuvantul castigator. Stiind
numarul cuvintelor pot defini intervalul [floor, ceiling], care acum este  [0,
27]. Cu ajutorul functiei random, stiu numarul liniei din fisier pe care se
afla cuvantul castigator.

Parcurg fisierul pana la linia cu numarul ales si dupa aceea inchid fisierul.

Pentru a putea respecta regulile jocului, am nevoie sa contorizez literele
cuvantului ales prin functia *contorizare_litere*. In WORDLE, daca cuvantul
ales este 'avion' si eu ghicesc 'aaaaa' atunci doar primul a va fi colorat iar
celalte nu, deoarece a-ul are doar o aparitie. 

Incep interfata grafica prin initscr(), cu aditiile:
- start_color() : pentru a putea colora literele
- cbreak() : pentru a putea iesi rapid cu CTRL+C, folosit in debugging
- noecho() : pentru a nu lasa input-ul de la tastatura sa fie vazut daca nu
  vreau asa ceva ( foarte folositor pentru casutele de cuvinte )

O alta componenta importanta a jocului WORDLE sunt culorile, asa ca definesc
cele 2 : verde si galben. Ecranul este deja negru asa ca am decis sa nu mai
colorez in cazul in care litera nu apartine cuvantului care trebuie ghicit.

Prin functia *center* introduc titlul si instructiunile.

### Functia center
Datorita functiei din biblioteca ncurses getmaxyx pot sa ma asigur ca titurile
vor fi intotdeauna centrate, indiferent de marimea terminalului ( asta daca
terminalul este destul de mare incat sa intre toate literele textului, dar nu
are neaparat de ce sa fie atat de ingust ). 

Foarte importante sunt defapt numarul de coloane, in aceasta functie retinut de
variabila latime. Pozitia de la care incepe titlul este definita de rezultatul
dintre diferenta dintre numarul total de coloane din terminal si lungimea
sirului pe care vreau sa il introduc pe aceea linie. Diferenta e impartita la
jumatate, pozitia de inceput pentru sir fiind retinuta in varaibila indent. 

### Revenire in main
Definesc variabilele de tip WINDOW :
- sub : matricea in care apar literele
- meniu : meniul de control

Prin functia *init_tabela* initalizez fiecare element ( subwindow ) din matrice
si ii definesc un contor pentru a avea pe ecran tabela de joc.

### init_tabela
Deoarece in regulile jocului avem 6 incercari sa ghicim un cuvant de 5 litere
atunci si tabela ( numita sub deoarece am folosit comanda subwindow pentru ea )
, o sa aiba exact aceste dimensiuni. 

Cum am explicat in decizii aspect, am decis sa sa fac casuta de 3x3 pentru ca
centrul sa fie in (1,1) pentru a imi fi mie mai usor. 

Pentru a nu se suprapune casutele, atunci diferenta dintre marginea de sus a
doua dintre linii este 4, asa ca diferenta dintre linia de jos a unei linii si
linia de sus a urmatoarei linii este 1 rand. 

### Revenire in main
Definesc meniul, al carui marimi sunt alese pentru a fi convenabile pentr
majoritatea ecranelor, eu lucrand cu marimile terminalului default de Ubuntu pe
Windows. Pentru a naviga meniul cu ajutorul arrow keys-urilor am nevoie de
comanda keypad(meniu, TRUE), care detecteaza arrow keys si poate interpreta
acele semnale.

Am decis sa nu las utilizatorul sa intre imediat in joc, astfel ca jucatorul
este rugat sa apese:
- ENTER pentru a incepe jocul 
- : pentru a accesa meniul

Daca este apasat altceva, jucatorul primeste mesajul de pierdere a jocului (
definit in cerinta 2 ).

### Meniu
Nu am putut muta toate instructiunile necesare pentru meniul de control inafara
functiei main, deoarece nu puteam sa repornesc sau sa inchid programul in alta
functie. Astfel ca functia *navigare_meniu* returneaza alegerea pe care o face
utlilizatorul, iar prin variabila highlight aceasta este interpretata :
- daca utilizatorul a apasat pe Iesire : 
  - inchid interfata grafica prin endwin();
  - opresc programul general prin return 0
- daca utillizatorul a apasat pe Restart :
  - pornesc o alta instanta a programului prin functia system ( care necesita
    biblioteca stdlib.h )
  - cand se termina acea iteratie, revin in cea originala ( sau precedenta ) si
    urmez pasii de la Iesire

### navigare_meniu
Pentru a naviga meniul este important sa putem naviga meniul cat timp vrea
utilizatorul, am while(1). 

Optiunile meniului sunt afisate doar daca apelam meniul, astfel ca in functie
afisam optiunile cu atributul A_REVERSE care se comporta ca un highlight.

Cat timp nu apasam ENTER, navigarea se face cu KEY_UP si KEY_DOWN care schimba
valoarea varaibilei highlight ( care tine minte pe ce optiune a ajuns
utilizatorul si ii aplica A_REVERSE ). 

Daca se apasa ENTER, se va iesi din while-ul infinit si se va returna valoarea
variabilei highlight, care va fi 0 ( Iesire ) sau 1 ( Restart ).

### Revenire in main
Acum ca am aratat comportamentul programului cand este apasata tasta ':', nu
voi mai detalia cand apare in continuarea programului. Meniul poate fi apelat:
- inainte sa inceapa jocul
- in timpul jocului
- la final

Daca jucatorul vrea totusi sa inceapa jocul apasa pe ENTER si pozitia
cursor-ului se muta in prima casuta. 

Deoarece am definit sub drept matrice putem sa parcurgem matricea prin i ( 0-5 )
si j ( 0-4 ). Pentru a primi si afisa literele pe care utilizatorul le introduce
mutam cursorul in sub[i][j], iar afisajul este facut cu `wmove(sub[i][j], 1, 1)`
care ne pune in mijlocul casutei si `wpritnw(sub[i][j], "%c" , ch)`.

Verific daca este litera cu isalpha ( din biblioteca ctype.h ) iar dupa aceea
prin functia *daca_e_litera* fac afisajul cum am prezentat mai sus. Dupa aceea
incrementez contorul ( care tine minte cate litere am introdus ). 

### Functia daca_e_litera
Deoarece in lista de cuvinte, cuvintele sunt scrise cu litere mici, atunci daca
introduc o majuscula trebuie sa o transform intr-o litera mica. 

Pentru a putea verifica cuvantul introdus de utilzator, folosesc variabila
cuvant_input pentru a retine literele introduse pe rand.

Pentru afisaj litera este afisata ca majuscula `wprintw(sub[i][j], "%c",
toupper(litera));` 

### Revenire in main
Daca nu este litera si inca nu am un cuvant intreg am 4 cazuri :
1. Am apasat enter inainte de a introduce un cuvant 
2. Am apasat backspace 
3. Am accesat meniul
4. Am apasat pe ceva 'nepermis' 

Instructiunile pentru ENTER sunt continute in functia *enter*

### Functia enter inainte de 5 caractere
Cum este descris si in cerinta, utilizatorul trebuie informat ca nu a introdus
inca un cuvant satisfacator. 

Se va sterge linia ( pun spatii peste literele
introduse anterior ) pe care ne aflam in momemtul curent si se va reseta
valoarea variabilei j ( coloanele ) si cea a contorului de litere. 

Mesajul se afisa la pozitia alesa arbitrat de mine, acela fiind "Introduceti
cuvant cu 5 litere!" 

### Revenire in main
Pe langa instructiunile din *enter*, variabila mesaj_time devine 1, semnaland
ca a fost afisat un mesaj pentru acel caracter. Mesajul va fi sters pentru
urmatorul caracter prin functia *sterg_mesaj* ( care foloseste functii ale
bibliotecii ncurses pentru a sterge ce se afla pe linia respectiva ).

Instructiunile pentru tasta BACKSPACE se afla in functia *backspace*

### Functia backspace
Pentru vrem sa stergem caracterul precedent avem 3 cazuri principale 
1. Avem cateva caractere dar nu 5
2. Suntem in prima casuta de pe acel rand
3. Vrem sa stergem caracterul din ultima casuta

Pentru 1 trebuie tinut minte ca j va fi incrementat la finalul iteratiei, asa
ca daca vrem sa ajugem pe caracterul precedent trebuie ca coloana pe care ne
aflam sa fie decrementata cu 2 pentru ca incerementare sa devina coloana - 1.

Caracterul precedent este in acelasi timp 'sters' ( suprascriu un spatiu peste
acel caracter ). In acelasi timp contorul literelor este decrementat cu 1.

Pentru 2 se urmeaza aceeasi pasi dar coloana devine astfel -1 pentru ca dupa
incrementare sa devina 0 si contorul este resetat la 0.

Pentru cazul 3 vrem sa revenim pe ultima casuta ( coloana acesteia fiind 4),
asa ca decrementare coloanei este cu 1. In rest aceeiasi pasi sunt urmati. 

### Revenire in main
Au mai ramas 2 optiuni : 3 si 4

Pentru meniul de control am explicat functionalitatea mai sus, asa ca nu ma voi
repeta.

Pentru cazul in care utilizatorul introduce orice alt caracter folosesc functia
*orice_alt_caracter_in_casute*. Si aici am un mesaj afisat, asa ca variabila
mesaj_time devine 1. 

### Functia orice_alt_caracter_in_casute
Conteaza foarte mult pozitia cursor-ului in aceasta functie.

Daca contorul nu este 5, atunci trebuie sa sterg acel caracter si sa raman in
aceeasi pozitie pentru ca utilizatorul sa poata introduce un caracter adecvat.
 
Procesul este asemanator cu cel de la functiile *backspace* si *enter*, dar cum
nu am introdus un caracter litera, nu vom scadea contorul. 

Utilizatorul primeste un avertisment "Introduceti doar litere!".

Daca contorul este 5, atunci nu vrem sa introducem litere, ci vrem doar ENTER,
BACKSPACE sau vrem sa accesam meniul de control. Asa ca utilizatorul primeste
acest avertisment modificat si inca o sansa sa introduca ceva adecvat. Daca
utilizatorul totusi nu vrea ( din motive care nu imi sunt clare ), programul va
trece la urmatoarea linie, fara a verifica cuvantul. 

### Revenire in main
Acum ca am explicat cazurile inainte de introducerea unui cuvant intreg, trec la
portiunea in care iesim din rand. Daca am ajuns ca variabila contor sa aiba
valoarea 5, atunci inseamna ca am introdus 5 litere si avem 4 optiuni :
1. ENTER pentru verificarea variabiliei cuvant_input
2. BACKSPACE pentru a sterge caracterul din ultima casuta ( am explicat
   procesul in explcatia pentru functia *backspace* asa ca nu ma voi repeta )
3. Pot accesa meniul de control 
4. Pot apasa pe alt caracter ( am explicat procesul in functia
   *orice_alt_caracter_in_casute* )

Ma voi muta in locatia din ecranul standard de mesaje pentru ca consider ca
este mai usor de urmarit. 

Instructiunile pentru ENTER sunt continute in functia *enter*

### Functia enter cand am 5 caractere
Aici o foarte importanta variabila este ghicit, care indica daca am ghicit in
totalitate cuvantul ales random. Este necesara pentru a afisa mesajul de
castig. 

Valorile variabilei ghicit vor fi influentate de rezultatul
`strcmp(cuvant_input, cuvant_ales)` care daca este 0 inseamna ca am ghicit
cuvantul. Altfel ghicit ramane 0.

Pentru colorarea literelor ma folosesc de functia *color_litere*

### Functia color_litere
Functia are nevoie de:
- cuvantul introdus de utilizatorul 
- cuvantul ales random de program 
- contorul literelor pentru cuvantul ales random ( o copie a celui original
  pentr a se putea utiliza pentru toate liniile )

Vom face o prima parcurgere a cuvantului_ales. Daca literele de pe o anumita
pozitie a cuvantului_ales si a cuvantului_input sunt egale:
- este colorata casuta respectiva 
- contorul pentru litera respectiva scade
- avem un vector care tine minte care litere au fost colorare cu verde

Facem o prima parcurgere prin care coloram cu verde deoarece daca o litera din
cuvantul ales cu mai multe aparitii trebuie prioritizat cazul in care
utilizatorul pune alt cuvant dar literele sunt intr-o pozitie corecta 

| 1     | 2     | 3     | 4     | 5     |
|-------|-------|-------|-------|-------|
| a     | b     | a     | t     | e     |
| verde | verde | verde | verde | negru |
| a     | b     | a     | t     | a     |

In acest exemplu putem vedea ca al treilea a din "abata" nu mai este colorat
deoarece cuvantul_ales "abate" are doar 2 aparitii ale literei a

| 1     | 2     | 3     | 4     | 5     |
|-------|-------|-------|-------|-------|
| a     | b     | a     | t     | e     |
| verde | negru | verde | negru | negru |
| a     | a     | a     | a     | a     |

Un alt caz poate fi cel in care din nou avem mai multe aparitii ale unei litere
in cuvant_input decat in cuvant_ales, dar nu ar fi corect sa fac galben al
doilea a din "aaaaa". Corect ar fi sa am ca in tabel, astfel ca trebuie sa vad
mai intai care litere trebuie colorate cu verde si sa tin cont de care sunt
colorate cu verde pentru a nu le colora de doua ori.

A doua parcurgere aduce schimbari casutelor doar in cazul in care contorul este
mai mare ca 0. Daca gasim vreo instanta a literei de pe pozitia k din
cuvantul_input in cuvantul_ales coloram cu galben. 

### Revenire in main
La finalul fiecarei iteratii este important sa rezetez variabila mesaj_time
pentru ca urmatorul caracter sa nu ramane cu mesajul pentru caracterul
precedent. 

Dupa ce este parcursa tabela cursorul este mutat pe pozitia de mesaj, definita
prin Y_MESAJ si X_MESAJ. Variabila ghicit decide ce mesaj este afisat :
- ghicit = 1 - mesajul este "Bravo ai ghicit cuvantul! Vrei sa mai joci? (Y/N)"
- ghicit = 0 - mesajul este "Nu ai ghicit cuvantul, acesta era: cuvant_ales"
  
Daca utilizatorul a castigat si vrea sa mai joace se poate apasa tasta Y sau y,
altfel utilizatorul poate accesa meniul sau poate apasa pe orice alta tasta
pentru a iesi din joc. 

Daca utilizatorul nu a castigat, atunci poate ori sa apeleze meniul de control
sau sa apese pe orice alta tasta pentru a iesi din joc.
