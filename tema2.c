/* LEPADATU Darius - 312 CB */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define LINIE 100
#define VF(a) (((ASt)(a))->vf)
#define DIME(a) (((ASt)(a))->dime)
#define VIDAS(a) VF(a) == NULL
#define IC(a) (((AQ)(a))->ic)
#define SC(a) (((AQ)(a))->sc)

typedef struct celst { 
    int id;
    struct celst *urm;
} TCelSt, *ACelSt;  

typedef struct stiva {
    size_t dime;
    ACelSt vf;
} TStiva, *ASt;

typedef struct celc { 
    struct celc *urm;
    void *id, *priority, *time, *exec_time;
    ASt Thread;
} TCelC, *ACelC;

typedef struct coada {
    size_t dime;
    ACelC ic, sc;
} TCoada, *AQ;


// initializeaza stiva

void *InitS(size_t d) {
    ASt s = NULL;
    s = (ASt)malloc(sizeof(TStiva));
    if(!s) return NULL;
    s->dime = d;
    s->vf = NULL;
    return (void*)s;
}

// initializeaza coada

void *InitQ(size_t d) {
    AQ c = NULL;
    c = (AQ)malloc(sizeof(TCoada));
    c->dime = d;
    c->ic = NULL;
    c->sc = NULL;
    return (void*)c;
}

// Push adauga un int in stiva, iar pop il scoate si introduce in alta stiva.

int Push(ASt a, int x) {
    ACelSt aux = (ACelSt)malloc(sizeof(TCelSt));
    if(!aux) return 0;
    aux->id = x;
    aux->urm = a->vf;
    a->vf = aux;
    return 1;
}

int Pop(void *s, int *x) {
    if (VIDAS(s)) return 0;
    ACelSt aux = VF(s);
    *x = aux->id;
    VF(s) = aux->urm;
    free(aux);
    return 1;
}

// IntrQ adauga un element in coada, iar ExtrQ ii returneaza campurile prin efect lateral.

int IntrQ(AQ c, int id, int priority, int time, int exec_time, ASt Thread) {
    ACelC aux = (ACelC)malloc(sizeof(TCelC));
    if (!aux) return 0;
    aux->Thread = Thread;
    aux->id = id;
    aux->priority = priority;
    aux->time = time;
    aux->exec_time = exec_time;
    aux->urm = NULL;
    if (!IC(c) && !SC(c)) {
        IC(c) = aux;
        SC(c) = aux;
    } else {
        SC(c)->urm = aux;
        SC(c) = aux;
    }
    return 1;
}

int ExtrQ(void *c, int *id, int *priority,
int *time, int *exec_time, ASt Thread) {
    if (!IC(c) && !SC(c)) return 0;
    ACelC aux = IC(c);
    IC(c) = aux->urm;
    *id = aux->id;
    *priority = aux->priority;
    *time = aux->time;
    *exec_time = aux->exec_time;
    Thread = aux->Thread;
    if (!IC(c)) SC(c) = NULL;
    free(aux);
    return 1;
}

// RastoarnaS primeste o stiva ca parametru, ii intoarce elementele si le introduce
// intr-o stiva destinatie.

int RastoarnaS(void *s, void *d) {
    ACelSt aux = NULL, ld = VF(d), ls = VF(s);
    if(VIDAS(s)) return 0;
    while(ls) {
        aux = ls;
        ls = ls->urm;
        aux->urm = ld;
        ld = aux;
    }
    VF(s) = NULL;
    VF(d) = ld;
    return 1;
}

// ConcatQ alipeste doua cozi in una singura singura.

int ConcatQ(void *s, void *d) {
    if (!IC(s) && !SC(s)) return 0;
    if (!IC(d) && !SC(d)) {
        IC(d) = IC(s); 
        SC(d) = SC(s); 
        IC(s) = SC(s) = NULL;
    } else {
        SC(d)->urm = IC(s);
        SC(d) = SC(s);
        IC(s) = SC(s) = NULL;
    }
    return 1;
}

// elibereaza memoria stivei

void eliberare_stiva(void *s) {
    if (!s) return;
    ASt aux = (ASt)s;
    ACelSt auxvf = NULL;
    while (aux->vf) {
        auxvf = aux->vf->urm;
        free(aux->vf);
        aux->vf = auxvf;
    }
    free(s);
}

// elibereaza memoria cozii

void eliberare_coada(void *c) {
    AQ aux = (AQ)c;
    ACelC auxic = NULL;
    while (IC(c)) {
        auxic = aux->ic->urm;
        if (aux->ic->Thread) {
            eliberare_stiva(aux->ic->Thread);
        }
        free(aux->ic);
        aux->ic = auxic;
    }
    free(c);   
}
// IntrOrdQ inseareaza ordonat un element 

void IntrOrdQ(AQ c, int id, int priority,
int time, int exec_time, ASt Thread) {
    if (!IC(c) && !SC(c)) {
        IntrQ(c, id, priority, time, exec_time, Thread);
        return;
    }
    ASt Thread1 = NULL;
    int id1, priority1, time1, exec_time1, cont = 0;
    void *caux = InitQ(sizeof(ACelC));
    while (ExtrQ(c, &id1, &priority1, &time1, &exec_time1, Thread1)) {
        if (priority1 < priority && !cont) {
            IntrQ(caux, id, priority, time, exec_time, Thread);
            cont = 1;
        } else if (priority1 == priority && !cont) {
            if (time1 > time) {
                IntrQ(caux, id, priority, time, exec_time, Thread);
                cont = 1;
            } else if (time1 == time) {
                if (id1 > id) {
                    IntrQ(caux, id, priority, time, exec_time, Thread);
                    cont = 1;
                }
            }
        }
        IntrQ(caux, id1, priority1, time1, exec_time1, Thread1);
    }
    if(cont == 0) {
        IntrQ(caux, id, priority, time, exec_time1, Thread);
    }
    ConcatQ(caux, c);
    eliberare_coada(caux);    
}

void IntrFinishedQ(AQ c, int id, int priority,
int time, int exec_time, ASt Thread) {
    if (!IC(c) && !SC(c)) {
        IntrQ(c, id, priority, time, exec_time, Thread);
        return;
    }
    ASt Thread1 = NULL;
    int id1, priority1, time1, exec_time1, cont = 0;
    void *caux = InitQ(sizeof(ACelC));
    while (ExtrQ(c, &id1, &priority1, &time1, &exec_time1, Thread1)) {
        if (time1 > time && !cont) {
            IntrQ(caux, id, priority, time, exec_time, Thread);
            cont = 1;
        }
        IntrQ(caux, id1, priority1, time1, exec_time1, Thread1);
    }
    if(cont == 0) {
        IntrQ(caux, id, priority, time, exec_time1, Thread);
    }
    ConcatQ(caux, c);
    eliberare_coada(caux);    
}

// RunningTasks verifica daca elementele din coada running si-au terminat executia,
// caz in care sunt introduse in coada finished, altfel li se mareste timpul executiei
// si sunt introduse inapoi in coada sursa.

int RunningTasks(void *c_running, void *c_finished,
int exec_time1) {
    ASt Thread = NULL;
    int id, priority, time, exec_time, max = -1;
    void *caux = InitQ(sizeof(ACelC));
    void *caux2 = InitQ(sizeof(ACelC));
    while (ExtrQ(c_running, &id, &priority, &time, &exec_time, Thread)) {
        if (time <= exec_time1 + exec_time) {
            IntrFinishedQ(caux2, id, priority, time, time, Thread);
            if (time > max) {
                max = time;
            }
        } else {
            IntrQ(caux, id, priority, time, exec_time1 + exec_time, Thread);
        }
    }
    ConcatQ(caux2,c_finished);
    ConcatQ(caux, c_running);
    eliberare_coada(caux);
    eliberare_coada(caux2);
    if (max > 0) {
        return max;
    }
    return 0;
}

// CautaIdCoada parcurge elementele pana ce identifica id-ul vrut si returneaza 1
// daca e gasit, iar daca nu e gasit returneaza 0.

int CautaIdCoada(AQ c, int ref, char *tip_coada, FILE *f) {
    ASt Thread = NULL;
    int id, priority, time, exec_time, found = 0;
    void *caux = InitQ(sizeof(ACelC));
    while (ExtrQ(c, &id, &priority, &time, &exec_time, Thread)) {
        IntrQ(caux, id, priority, time, exec_time, Thread);
        if (id == ref && found == 0) {
            if (strcmp(tip_coada, "running") == 0) {
                fprintf(f, "Task %d is running (remaining_time = %d).\n",
                id, exec_time);
            } else if (strcmp(tip_coada, "waiting") == 0) {
                fprintf(f, "Task %d is waiting (remaining_time = %d).\n",
                id, time);
            } else {
                fprintf(f, "Task %d is finished (executed_time = %d).\n",
                id, time);
            }
            found = 1;
        }
    }
    ConcatQ(caux, c);
    eliberare_coada(caux);
    return found;
}

// CautaIdStiva parcurge elementele pana ce identifica id-ul vrut si returneaza 1
// daca e gasit, iar daca nu e gasit returneaza 0.

int CautaIdStiva(ASt s, int ref) {
    void *saux = InitS(sizeof(ACelSt));
    int x;
    while (Pop(s, &x) == 1) {
        Push(saux, x);
        if (x  == ref) {
            RastoarnaS(saux, s);
            eliberare_stiva(saux);
            return 1;
        }
    }
    RastoarnaS(saux, s);
    eliberare_stiva(saux);
    return 0;
}

// PrintareWaiting, PrintareRunning si PrintareFinished afiseaza elementele cozilor.

void PrintareWaiting(void *c, FILE *f) {
    ASt Thread = NULL;
    int id, priority, time, exec_time, cont = 0;
    void *caux = InitQ(sizeof(ACelC));
    fprintf(f, "====== Waiting queue =======\n[");
    while (ExtrQ(c, &id, &priority, &time, &exec_time, Thread)) {
        IntrQ(caux, id, priority, time, exec_time, Thread);
        if (cont == 0) {
            fprintf(f, "(%d: priority = %d, remaining_time = %d)",
            id, priority, time);
            cont++;
        } else {
            fprintf(f, ",\n(%d: priority = %d, remaining_time = %d)",
            id, priority, time);
        }
    }
    fprintf(f, "]\n");
    ConcatQ(caux, c);
    eliberare_coada(caux);
}

void PrintareRunning(void *c, FILE *f) {
    ASt Thread = NULL;
    int id, priority, time, exec_time, cont = 0;
    void *caux = InitQ(sizeof(ACelC));
    fprintf(f, "====== Running in parallel =======\n[");
    while (ExtrQ(c, &id, &priority, &time, &exec_time, Thread)) {
        if (cont == 0) {
            fprintf(f, "(%d: priority = %d, remaining_time = %d, running_thread = %d)",
            id, priority, time - exec_time, 1);
            cont++;
        } else {
            fprintf(f, ",\n(%d: priority = %d, remaining_time = %d, running_thread = %d)",
            id, priority, time - exec_time, 1);
        }
        IntrQ(caux, id, priority, time, exec_time, Thread);
    }
    fprintf(f, "]\n");
    ConcatQ(caux, c);
    eliberare_coada(caux);
}

void PrintareFinished(void *c, FILE *f) {
    ASt Thread = NULL;
    int id, priority, time, exec_time, cont = 0;
    void *caux = InitQ(sizeof(ACelC));
    fprintf(f, "====== Finished queue =======\n[");
    while (ExtrQ(c, &id, &priority, &time, &exec_time, Thread)) {
        IntrQ(caux, id, priority, time, exec_time, Thread);
        if (cont == 0) {
            fprintf(f, "(%d: priority = %d, executed_time = %d)",
            id, priority, time);
            cont++;
        } else {
            fprintf(f, ",\n(%d: priority = %d, executed_time = %d)",
            id, priority, time);
        }
    }
    fprintf(f, "]\n");
    ConcatQ(caux, c);
    eliberare_coada(caux);
}

int main(int argc, char **argv) {
    FILE *in_file = NULL, *out_file = NULL;
    in_file = fopen(argv[argc - 2], "rt");
    out_file = fopen(argv[argc - 1], "w");
    void *s, *c_waiting, *c_running, *c_finished;
    char *linie = NULL, *p = NULL;
    int Q = 0, C = 0, N = 0, i = 0, nr_task = 0, thread_id;
    int nr_tasks = 0, exec_time = 0, priority = 0, id = 1, sum = 0;
    linie = (char*)malloc(LINIE * sizeof(char));
    c_waiting = InitQ(sizeof(ACelC));
    c_running = InitQ(sizeof(ACelC));
    c_finished = InitQ(sizeof(ACelC));
    s = InitS(sizeof(ACelSt));
    fscanf(in_file, "%d\n", &Q);
    fscanf(in_file, "%d\n", &C);
    N = 2 * C;
    for (i = N - 1; i >= 0; i--) {
        Push(s, i);
    }
    while (fgets(linie, LINIE, in_file)) {
        if (linie[strlen(linie) - 1] == '\n') {
            linie[strlen(linie) - 1] = '\0';
        }
        p = strtok(linie, " ");
        if (strcmp(p, "add_tasks") == 0) {
            p = strtok(NULL, " ");
            nr_tasks = atoi(p);
            p = strtok(NULL, " ");
            exec_time = atoi(p);    
            p = strtok(NULL, " ");
            priority = atoi(p);
            for (i = 1; i <= nr_tasks; i++, id++) {
                IntrOrdQ(c_waiting, id, priority, exec_time, 0, NULL);
                fprintf(out_file, "Task created successfully : ID %d.\n", id);
                
            }
        } else if (strcmp(p, "get_task") == 0) {
            p = strtok(NULL, " ");
            nr_task = atoi(p);
            if (CautaIdCoada(c_waiting, nr_task, "waiting", out_file) == 0) {
                if (CautaIdCoada(c_running, nr_task, "running", out_file) == 0) {
                    if(CautaIdCoada(c_finished, nr_task, "finished", out_file) == 0) {
                        fprintf(out_file, "Task %d not found.\n", nr_task);
                    }
                }
            }
        } else if (strcmp(p, "get_thread") == 0) {
            p = strtok(NULL, " ");
            thread_id = atoi(p);
            if(CautaIdStiva(s, thread_id)) {
                fprintf(out_file, "Thread %d is idle.\n", thread_id);
            }
        } else if (strcmp(p, "print") == 0) {
            p = strtok(NULL, " ");
            if (strcmp(p, "waiting") == 0) {
                PrintareWaiting(c_waiting, out_file);
            } else if (strcmp(p, "running") == 0) {
                PrintareRunning(c_running, out_file);
            } else if (strcmp(p, "finished") == 0) {
                PrintareFinished(c_finished, out_file);
            }
        } else if (strcmp(p, "run") == 0) {
            id = 1;
            p = strtok(NULL, " ");
            exec_time = atoi(p);
            fprintf(out_file, "Running tasks for %d ms...\n", exec_time);
            ConcatQ(c_waiting, c_running);
            sum = sum + RunningTasks(c_running, c_finished, exec_time);
        } else if (strcmp(p, "finish") == 0) {
            fprintf(out_file, "Total time: %d", sum);
        }
        p = strtok(NULL, " ");
    }
    eliberare_stiva(s);
    eliberare_coada(c_waiting);
    eliberare_coada(c_running);
    eliberare_coada(c_finished);
    free(linie);
    fclose(in_file);
    fclose(out_file);
    return 0;
}