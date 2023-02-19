Inainte de main:

Am definit mai multe functii: eliberare_stiva, eliberare_coada, InitS, InitQ,
Push, Pop, IntrQ, ExtrQ, RastoarnaS, ConcatQ, IntrOrdQ, IntrFinishedQ, RunningTasks, 
CautaIdCoada, CautaIdStiva, PrintareWaiting, PrintareRunning si PrintareFinished.
Am scris doua functii importante pentru dezalocarea memoriei, eliberare_stiva si
eliberare_coada, ce parcurg structurile de date si dau free acolo unde este necesar.
Functiile InitS si InitQ intializeaza o stiva si respectiv o coada, implementate
prin intermediul unor liste.
Push adauga un int in stiva, iar pop il scoate si introduce in alta stiva.
IntrQ adauga un element in coada, iar ExtrQ ii returneaza campurile prin efect lateral.
RastoarnaS primeste o stiva ca parametru, ii intoarce elementele si le introduce
intr-o stiva destinatie.
ConcatQ alipeste doua cozi in una singura.
IntrOrdQ inseareaza ordonat un element in coada in functie de prioritate,
timpul si id-ul acestuia in felul urmator: descrecator in functie de prioritate,
crescator fata de timp si id.
IntrFinishedQ functioneaza similar cu functia anterioara, numai ca aceasta
insearaza in functie de timpul executiei taskului.
RunningTasks verifica daca elementele din coada running si-au terminat executia,
caz in care sunt introduse in coada finished, altfel li se mareste timpul executiei
si sunt introduse inapoi in coada sursa.
CautaIdCoada parcurge elementele pana ce identifica id-ul vrut si returneaza 1
daca e gasit, iar daca nu e gasit returneaza 0.
CautaIdStiva functioneaza ca functia anterioara, doar ca este implementata pentru
o stiva.
PrintareWaiting, PrintareRunning si PrintareFinished afiseaza elementele cozilor.

In main:

Se citeste de la tastatura rand cu rand cate o comanda. Aceasta este fragmentata
si parcursa prin strtok, fiind identificate functiile ce trebuie folosite
pentru a manipula stiva de threaduri si cele trei cozi.