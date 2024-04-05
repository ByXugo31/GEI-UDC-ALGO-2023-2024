#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>
/*
 Practica 3 Algoritmos
*/

////////FUNCIONES QUE GENERAN VECTORES////////

/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
void inicializar_semilla() {
    srand(time(NULL));
}
/* Genera numeros aleatorios */
void aleatorio(int v[], int n){
    int i, m=2*n+1;
    for (i = 0; i < n; i++) v[i] = (rand() % m) - n;
}

/* Genera vector ascendiente */
void ascendente(int v[], int n){
    int i;
    for (i = 0; i < n; i++) v[i] = i;
}

/* Genera vector descendiente */
void descendente(int v[], int n){
    int i,j;
    for (i = n-1, j = 0; i >=0; i--, j++){
        v[j] = i;
    }
}

////////FUNCIONES AUXILIARES CON VECTORES////////

void copiarVector(int src[], int dest[], int n){
    int i;
    for (i = 0; i < n; ++i) dest[i] = src[i];
}

/* Imprime la lista de vectores */
void listar_vector(int v[], int n){
    int i;
    for (i = 0; i < n; i++) printf(" %2d", v[i]);
}

bool equalVector(int v[],int w[],int tamn, int tamm){
    int i;
    if (tamn != tamm) return false;
    for (i = 0; i < tamn; ++i) {
        if (v[i] != w[i]) return false;
    }
    return true;
}

////////DEFINICIÓN DEL MONTÍCULO Y OPERACIONES BÁSICAS////////

#define TAM 256000
struct monticulo {
    int ultimo;
    int vector[TAM];
};
typedef struct monticulo * pmonticulo;

bool estaVacio(pmonticulo m){
    return m->ultimo == -1;
}

void inicializarMonticulo(pmonticulo m){
    m->ultimo = -1;
}

void hundir(pmonticulo m, int i){
    int HijoIzq, HijoDer, aux, j;
    do{
        HijoIzq = 2*i+1;
        HijoDer = 2*i+2;
        j = i;
        if(HijoIzq<=m->ultimo && m->vector[HijoIzq]<m->vector[i]) i=HijoIzq;
        if(HijoDer<=m->ultimo && m->vector[HijoDer]<m->vector[i]) i=HijoDer;
        aux = m->vector[j];
        m->vector[j] = m->vector[i];
        m->vector[i] = aux;
    }while(j!=i);
}

void crearMonticulo(int v [], int n, pmonticulo m){
    int i;
    copiarVector(v,m->vector,n);
    m->ultimo = n-1;
    for (i = (m->ultimo/2); i >= 0; i--) hundir(m,i);
}

int quitarMenor(pmonticulo m){
    int min;
    if(estaVacio(m)){
        perror("Montículo vacio");
        exit(0);
    }
    min = m->vector[0];
    m->vector[0] = m->vector[m->ultimo];
    m->ultimo--;
    if (m->ultimo >= 0) hundir(m, 0);
    return min;
}

////////ALGORITMO A ANALIZAR////////

void ordenarPorMonticulos(int v[],int n){
    int i;
    pmonticulo m = malloc(sizeof (struct monticulo));
    inicializarMonticulo(m);
    crearMonticulo(v,n,m);
    for(i=0;i<=n-1;i++){
        v[i]=quitarMenor(m);
    }
    free(m);
}

////////TESTS////////

/*Test de inicialización de un montículo*/
void inicialicarTest(pmonticulo m){
    if (m->ultimo==-1) printf("\nMontículo inicializado correctamente\n");
    else{
        perror("No se ha inicializado el montículo");
        exit(0);
    }
}

/*Test de comprobar si está vacío un montículo*/
void vacioTest(pmonticulo m){
    bool vacio = estaVacio(m);
    printf("Resultado esperado: true\n");
    printf("Resultado obtenido: %s\n", vacio ? "true":"false");
    printf("RESULTADO = %s\n", vacio == 1 ? "OK" : "NOT OK");
}

/*Test de creación de un montículo*/
void creationTest(pmonticulo m){
    int v[] = {0, 2, 4, 6, 8, 23, 78, 99, 124, 256};
    crearMonticulo(v, 10, m);
    printf("Resultado esperado (vector proporcionado): ");
    listar_vector(v, 10);
    printf("\nResultado obtenido (vector montículo):     ");
    listar_vector(m->vector, m->ultimo+1);
    printf("\nRESULTADO = %s\n",equalVector(v,m->vector,10,m->ultimo+1)
                                ?"OK":"NOT OK");
}

/*Test de quitar menor de un montículo*/
void quitarMenorTest(pmonticulo m){
    int v[] = {2, 6, 4, 99, 8, 23, 78, 256, 124};
    int min = quitarMenor(m);
    printf("Resultado esperado (valor esperado):                0\n");
    printf("Resultado obtenido (valor que devuelve la función): %d\n", min);
    printf("RESULTADO = %s\n", min == 0 ? "OK" : "NOT OK");
    printf("Resultado esperado (vector esperado):  ");
    listar_vector(v, 9);
    printf("\nResultado obtenido (vector montículo): ");
    listar_vector(m->vector, m->ultimo+1);
    printf("\nRESULTADO = %s\n",equalVector(v,m->vector,9   ,m->ultimo+1)
                                ?"OK":"NOT OK");
}

/*Imprime el test de los montículos*/
void monticuloTestPrint(){
    pmonticulo m = malloc(sizeof(struct monticulo));
    inicializarMonticulo(m);
    printf("\n*******TEST OPERACIONES MONTÍCULOS*******\n");
    inicialicarTest(m);
    printf("\n--ESTÁ VACÍO TEST--\n");
    vacioTest(m);
    printf("\n--CREACIÓN TEST--\n");
    creationTest(m);
    printf("\n--QUITAR MENOR TEST--\n");
    quitarMenorTest(m);
    free(m);
}

/*Verifica si un vector está ordenado*/
bool estaOrdenado(int v[], int n) {
    int i;
    for (i = 0; i < (n-1); ++i){
        if (v[i] > v[i + 1]){
            printf("\nEl vector no está ordenado\n");
            return false;
        }
    }
    printf("\nEl vector está ordenado\n");
    return true;
}

/*Test que verifica si un algoritmo ordena o no*/
void ordenacionTest(void (*funct)(int[],int), int numPruebas){
    int i, n = 9;
    int v[n];
    for (i = 0; i < numPruebas; i++) {
        printf("\n-PRUEBA: %d-",i+1);
        printf("\nVECTOR SIN ORDENAR:\n");
        funct(v, n);
        listar_vector(v, n);
        estaOrdenado(v,n);
        printf("\nVECTOR ORDENADO\n");
        ordenarPorMonticulos(v,n);
        listar_vector(v,n);
        estaOrdenado(v,n);
    }
}

/*Imprime el test de ordenación*/
void ordenacionTestPrint(){
    printf("\n*******TEST ALGORITMO DE ORDENACION*******\n");
    printf("\n---TEST CON VECTORES ALEATORIOS---");
    ordenacionTest(aleatorio, 1);
    printf("\n\n---TEST CON VECTORES ASCENDENTES---");
    ordenacionTest(ascendente, 1);
    printf("\n\n---TEST CON EL VECTORES DESCENDENTES---");
    ordenacionTest(descendente, 1);
}

/*Imprime ambos test*/
void printTest(){
    monticuloTestPrint();
    ordenacionTestPrint();
}

////////CALCULO DE TABLAS DE TIEMPOS////////

/* Calcular el tiempo real del programa */
double microsegundos(){
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

/*Mide el tiempo de crear un montículo*/
double medirTiempoMonticulo(int v[],int n){
    double t0, t1;
    pmonticulo m = malloc(sizeof (struct monticulo));
    aleatorio(v,n);
    t0 = microsegundos();
    crearMonticulo(v,n,m);
    t1 = microsegundos();
    free(m);
    return t1-t0;
}

/*Mide el tiempo de ejecución*/
double medirTiempoAlgoritmo(int v[],int n,void (*algoritmo)(int[],int)){
    double t0, t1;
    t0= microsegundos();
    algoritmo(v, n);
    t1 = microsegundos();
    return t1-t0;
}

/*Mide el tiempo de crear un monticulo en k ejecuciones*/
double medirTiempoMonticuloTPequenos(int v[],int n,int k){
    double t0, t1;
    int i;
    pmonticulo m = malloc(sizeof (struct monticulo));

    t0 = microsegundos();
    for(i=0;i<k;i++){
        aleatorio(v,n);
        crearMonticulo(v,n,m);
    }
    t1 = microsegundos();
    free(m);

    return t1-t0;
}

/*Mide el tiempo de ejecucion del algoritmo en k ejecuciones*/
double medirTiempoAlgoritmoTPequenos(int v[],int n, int k,
                                     void (*algoritmo)(int[],int k),
                                     void (*vector)(int[],int)){
    double t0, t1;
    int i;
    t0 = microsegundos();
    for (i = 0; i < k; i++) {
        vector(v,n);
        algoritmo(v, n);
    }
    t1 = microsegundos();
    return t1-t0;
}

/*Mide el tiempo de ejecucion del algoritmo en k ejecuciones*/
double medirTiempoGenerarVector(int v[],int n,int k,void (*vector)(int[],int)){
    double t0, t1;
    int i;
    t0 = microsegundos();
    for(i = 0; i < k; i++) vector(v,n);
    t1 = microsegundos();
    return t1 - t0;
}

/* Funcion para calcular el tiempo de ejecucion de los algoritmos 1 y 2 */
void tTiempos(void (*algoritmo)(int[],int), void (*vector)(int[],int) ,
              double x, double y, double z, bool crearMont){
    double tf0, tf1, tf2;
    int v[TAM], n;
    int k =1000;
    for (n = 2000; n <= TAM/8; n*=2) {
        vector(v, n);
        if (crearMont) tf0 = medirTiempoMonticulo(v,n);
        else tf0 = medirTiempoAlgoritmo(v,n,algoritmo);
        if (tf0<500) {
            if (crearMont) tf1 = medirTiempoMonticuloTPequenos(v,n,k);
            else tf1 = medirTiempoAlgoritmoTPequenos(v,n,k,algoritmo,vector);
            tf2 = medirTiempoGenerarVector(v,n,k,vector);
            tf0 = (tf1 - tf2)/k;
            if (crearMont){
                printf("(*) %5d %17.5lf %15.5lf\n",
                       n, tf0, tf0/ n);
            } else{
                printf("(*) %5d %17.5lf %15.5lf %19.5lf %21.5lf\n",
                       n, tf0, tf0/ pow(n,x), tf0/ pow(n,y)*log(n), tf0/ pow(n,z));
            }

        } else {
            if (crearMont){
                printf("    %5d %17.5lf %15.5lf\n",
                       n, tf0, tf0/ n);
            } else{
                printf("    %5d %17.5lf %15.5lf %19.5lf %21.5lf\n",
                       n, tf0, tf0/ pow(n,x), tf0/ pow(n,y)*log(n), tf0/ pow(n,z));
            }
        }
    }
}

/* Imprime las tablas */
void printTable(void (*algoritmo)(int[],int), void (*vector)(int[],int),
                int max_prints, double cota1, double cota2, double cota3,
                bool monticulo){
    int i;
    for (i=0;i<max_prints;i++) {
        if(i==0 || max_prints == 1){
            max_prints++;
            continue;
        }
        if(monticulo){
            printf("\n%8s%17s%17s\n",
                   "n", "t(n)", "t(n)/n");
        } else {
            printf("\n%8s %15s %15s %.2f %11s %.3f %1s %11s %.1f\n",
                   "n", "t(n)", "t(n)/n^", cota1, "t(n)/n^", cota2, "*log(n)", "t(n)/n^", cota3);
        }
        tTiempos(algoritmo,vector,cota1,cota2,cota3,monticulo);
    }
}

/* Funcion que imprime las tablas de los tiempos del algoritmo*/
void printTiempos(){
    int max_prints = 3;
    printf("\n---MEDICIÓN DE TIEMPOS---\n");
    printf("\nCreacion de monticulo con vector aleatorio\n");
    printTable(ordenarPorMonticulos,aleatorio,max_prints,0,1,0,true);
    printf("\nOrdenación por montículos aleatorios\n");
    printTable(ordenarPorMonticulos,aleatorio,max_prints,0.9,1.21,1.4,false);
    printf("\nOrdenación por montículos ascendente\n");
    printTable(ordenarPorMonticulos,ascendente,max_prints,0.9,1.29,1.4,false);
    printf("\nOrdenación por montículos descendente\n");
    printTable(ordenarPorMonticulos,descendente,max_prints,0.9,1.363,1.4,false);
}

////////MAIN////////

int main() {
    inicializar_semilla();
    printTest();
    printTiempos();
    return 0;
}