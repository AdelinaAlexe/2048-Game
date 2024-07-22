Jocul 2048

In implementarea jocului 2048 am folosit urmatoarele functii pentru a modulariza
mai bine codul:

Functia "move_to_end":
Am mutat elementele nenule dintr-un vector spre marginea din dreapta, cate o 
pozitie pana la final, iar 
zerourile le-am mutat pe toate in stanga. Am pastrat, de asemenea, ordinea 
elementelor nenule, facand 
interschimbari doar intre un element nenul si un zero.
Am apelat functia move_to_end(i + 1) pentru a face mutarile de la dreapta 
spre stanga asfel incat sa nu
se piarda anumite valori la interchimbare si pentru a nu ramane zerouri intre 
elemente nenule sau in dreapta.

Functia "merge_and_move":
In aceasta functie am verificat daca exista 2 elemente cu aceeasi valoare pe 
pozitii consecutive, si daca da,
le-am adunat la unul dintre ele, iar pe celalalt l-am facut 0 ( practic, am 
unit 2 celule cu aceeasi valoare).
Daca un element nenul se afla pe pozitie langa un element nul, le-am 
interchimbat pentru a gasi o eventuala
celula cu care sa il unesc pe cel nenul.
Tot aici, dupa ce am lipit toate celulele cu aceeasi valoare, am efectuat 
mutarea celulelor nenule spre capat,
folosind functia "move_to_end".

Functia "random_gen":
Este o functie care genereaza valori aleatorii de 2 sau 4 pe pozitii aleatorii.
Am apelat srand( time(NULL) ) pentru a face generarea cat mai aleatorie, 
astfel incat sa genereze valori diferite
la fiecare rulare a programului.
Am generat indici din matrice, pe pozitia carora sa nu se afle vreun element 
nenul.
Am generat o valoare de 2 sau 4 in functie de paritatea numarului generat cu 
rand().

Functia "check_status":
Am verificat daca jocul a fost pierdut sau castigat.
Am parcurs matricea cu elementele jocului si am cautat un element cu valoarea
2048. Daca exista un astfel de element,
am returnat 1, ceea ce inseamna ca jocul a fost castigat.
Am numarat cate valori de 0 exista in matrice, dar am si verificat daca in 
matrice exista elemente nenule "vecine",
adica daca se mai poate efectua o mutare valida.
Daca in matrice nu exista nicio valoare de 0 ( tabla este plina ) si daca nu
exista elemente pentru care se mai poate
efectua o mutare valida, atunci am returnat 2, adica jocul a fost pierdut.

Functia "previous_steps":
Este o functie care retine intr-un tablou tridimensional un numar de maxim 
500 de matrici, si intr-un vector un numar
de maxim 500 de scoruri precedente.
Daca s-a depasit capacitatea maxima, am mutat elementele din cele 2 tablouri 
pentru a retine in continuare decat ultimele 
500 de elemente, adica ultimele 500 miscari ale jocului si scoruri.
Tot in aceasta functie am retinut matricea jocului de la momentul curent in 
tabloul tridimensional "prev" si scorul in 
vectorul "s".

Functia "take_step_back":
Este functia in care am scazut numarul de pasi, adica numarul de mutari si am 
revenit la mutarea precedenta, atribuind
matricii jocului curent valorile matricii din jocul precedent, care au fost 
retinute in tabloul "prev". Am facut acelasi
lucru si pentru scor.

Functia "check_if_invalid":
Este o functie care verifica daca o mutare este valida sau nu. In cazul in 
care in urma primirii unei taste si a efectuarii
miscarii corespunzatoare nu s-a modificat nimic in joc ( matricea de la 
momentul curent este identica cu cea de la momentul
precedent ), inseamna ca mutarea este invalida, iar pentru acest lucru am 
returnat 1, iar in caz contrar am returnat 0.

Functia "determine_best_direction":
In aceasta functie am simulat toate posibilele mutari in toate cele 4 directii
pentru a vedea in ce directie se elibereaza
un numar maxim de celule. Acea directie am retinut-o intr-o variabila si am 
returnat-o.

Implementarea mutarilor am pornit-o de la un tutorial de pe YouTube, din care 
m-am documentat pentru a intelege mai bine jocul 2048,
insa am facut modificarile necesare pentru a aplica mutarile corect pe problema
mea.


Pentru a face tranzitia dintre meniu si jocul propriu-zis am simulat 2 ecrane,
prin cele 2 functii "Menu" si "Game".

Functia "Menu":
Am creat o fereastra mai mica pentru meniul propriu-zis, in care am adaugat 
cele 3 optiuni : "New Game", "Resume" si "Quit".
Am parcurs cele 3 optiuni si am folosit o variabila "highlight" care indica 
optiunea de la momentul curent, moment in care
evidentiez optiunea curent printr-o inversare de culori.
Citesc caracterul primit si schimb valoarea variabilei in functie de acesta 
( pentru "KEY_DOWN" creste, pentru "KEY_UP" scade ).
In momentul in care caracterul primit este "ENTER", se opreste structura 
repetitiva ( bucla ).
Verific la ce optiune m-am oprit si retin indicele corespunzator pentru a 
initializa ulterior jocul in functie de optiunea aleasa.
Am pornit implementarea acestei functii de la un tutorial de pe YouTube, 
pe care l-am gasit cand ma documentam in legatura cu biblioteca ncurses.

Functia "Game":
Este functia in care am implementat toate functionalitatile jocului 
propriu-zis.
Pentru asta, am creat mai multe ferestre :
* "new_game" de dimensiunea intregului ecran
* "header" de dimeniunea primei linii, unde am afisat "titlul" "2048 GAME"
* "control_panel" de dimensiunea a jumatate de ecran ( fara prima linie ),
unde am afisat elementele panoului de control
* "actual_game" de dimensiunea a jumatate de ecran ( fara prima linie ), 
unde am afisat tabla de joc
* "cell" o fereastra de dimensiunea unei celule de joc, pentru a o putea 
colora corespunzator

Inainte de inceperea buclei in care actualizez permanent si implementez 
jocul propriu-zis, am declarat si initializat niste
variabile care retin timpul la momentul curent, pentru afisarea datei si 
a orei, dar si pentru a putea calcula ulterior
timpul de la ultima apasare a unei taste, necesar in implementarea miscarii 
automate.

De asemenea, in cazul in care se incepe un joc nou ( scorul este egal cu 0 ), 
am generat 2 valori aleatorii pe pozitii aleatorii.

In bucla, am afisat constant elementele panoului de control, actualizand 
scorul si highscore-ul jocului.
Am citit caracterul primit de la tastatura.
In cazul fiecarei mutari, am reinitializat timpul de la ultima apasare a 
unei taste, am adaugat matricea jocului in tablul miscarilor
precedente, am efectuat mutarea si unirea celulelor in functie de directia 
primita, apeland corespunzator functia "merge_and_move"
( am retinut in vectorul "a" liniile sau coloanele pe care aplic mutarea, 
punand elementele in ordinea corecta pentru a se efectua 
mutarea corespunzator ). Am retinut mutarile in matricea jocului. Am verificat
cu functia "check_if_invalid" daca mutarea efectuata
a fost corecta, iar daca da, am generat o alta valoare aleatorie pe pozitie 
aleatorie, si am afisat sageata corespunzatoare ultimei mutari valide efectuate
In cazul in care mutarea nu a fost valida, am afisat un mesaj care precieaza 
acest lucru pe linia urmatoare, pe care l-am "sters" suprascriindu-l cu un 
alt mesaj care contine numai spatii.

In cazul in care au trecut mai mult de 10 secunde de la ultima apasare a unei 
taste, am efectuat miscarea automata, in directia cea buna,
adica directia care elibereaza cele mai multe celule, pe care am aflat-o apeland
functia "determine_best_direction".

Am colorat celulele jocului, in functie de valoarea respectiva, folosind o 
noua fereastra pentru fiecare celula.

Am verificat daca jocul a fost castigat sau pierdut cu functia "check_status" 
si am afisat un mesaj corespunzator.

Am oprit jocul la apasarea unei taste 'q'. 
Am retinut intr-o variabila unul dintre caracterele 'w' 
( daca jocul a fost castigat ), 'l' ( daca jocul a fost pierdut ) si 'q' daca
jocul a fost parasit prin apasarea tastei 'q'.

In final, am afisat permanent daca si ora curenta, retinand intr-un sir de 
caractere data si ora curenta primite prin functia strftime.

Functia "main" :

Am initializat ecranul si am creat noi culori, nuante diferite de roz si mov, 
pe care le-am folosit pentru a colora diferit celulele 
jocului. Am initialiat in fucntie de aceste culori si niste perechi de culori.

Am alocat dinamic tabloul tridimensional in care retin fiecare matrice de joc,
de la fiecare mutare.
Am alocat dinamic si vectorul in care retin scorurile de la fiecare moment.

Am creat o bucla care ruleaza atat timp cat nu se alege optiunea 'QUIT', care 
face tranzitia dintre meniu si jocul propriu-zis in functie
de ce optiune a fost aleasa. Daca a fost aleasa optiunea "New Game", am 
initializat matricea jocului si scorul cu 0 si am apelat functia
"Game" dupa acesti parametrii. Daca a fost aleasa optiunea "Resume", se 
verifica daca jocul a fost pierdut sau a fost parasit prin apasarea
tastei 'q' si se apeleaza functia "Game" dupa parametrii corespunzatori.

Daca optiunea aleasa a fost "QUIT", am inchis ecranul jocului. 
In final, am eliberat memoria celor 2 tablouri. 

Pentru puncte bonus :
* am colorat fundalul intregului joc
* am impartit ecranul in ferestre care isi modifica dimeniunea in functie 
de dimensiunea terminalului, acest lucru aplicandu-se inclusiv celulelor 
tablei de joc.
* am centrat mesajele in mijlocul ferestrelor tot in functie de marimea 
terminalului
* am creat noi culori, printr-un amestec de culori RGB, ale caror valori 
le-am setat in functie de culoarea preferata, folosind functia init_color() 
din ncurses.
* am implementat si functia de UNDO, care prin apasarea tastei 'u' sau 'U' 
trece la pasul anterior. Se pot efectua maxim 500 de pasi in spate.
* am afisat ultima tasta apasata la fiecare moment, iar in cazul in care o 
mutare este invalida, am afisat un mesaj care precizeaza acest lucru si 
inseamna la apasarea unei taste diferite decat ultima folosita pentru o 
mutare valida.
* am calculat si un highscore, pe care l-am afisat in fereastra panoului 
de comenzi
* am afisat si legenda comenzilor posibile.

