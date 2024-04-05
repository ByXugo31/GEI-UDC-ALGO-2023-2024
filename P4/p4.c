#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>
/*
 Practica 4 Algoritmos
*/

////////DEFINICIÓN DE LA MATRIZ////////

#define TAM_MAX 1024
typedef int ** matriz;
matriz crearMatriz(int n) {
    int i;
    matriz aux;
    if ((aux = malloc(n*sizeof(int *))) == NULL)
        return NULL;
    for (i=0; i<n; i++)
        if ((aux[i] = malloc(n*sizeof(int))) == NULL)
            return NULL;
    return aux;
}

void iniMatriz(matriz m, int n) {
    int i, j;
    for (i=0; i<n; i++)
        for (j=i+1; j<n; j++)
            m[i][j] = rand() % TAM_MAX + 1;
    for (i=0; i<n; i++)
        for (j=0; j<=i; j++)
            if (i==j)
                m[i][j] = 0;
            else
                m[i][j] = m[j][i];
}

void liberarMatriz(matriz m, int n) {
    int i;
    for (i=0; i<n; i++)
        free(m[i]);
    free(m);
}

////////FUNCIONES AUXILIARES DE GENERACIÓN////////

/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
void inicializar_semilla() {
    srand(time(NULL));
}

////////FUNCIONES AUXILIARES CON VECTORES////////

/*Copia un vector en otro*/
void copiarVector(int src[], int dest[], int n){
    int i;
    for (i = 0; i < n; ++i) dest[i] = src[i];
}

/*Copia una matriz en otra*/
void copiarMatriz(matriz dest, int n, int src[][n]){
    int i;
    for (i = 0; i < n; ++i) copiarVector(src[i],dest[i],n);
}

/* Imprime un vector */
void listar_vector(int v[], int n){
    int i;
    printf("[");
    for (i = 0; i < n; i++) printf(" %2d", v[i]);
    printf("  ] \n");
}

/*Imprime una matriz*/
void listar_matriz (matriz m, int n){
    int i;
    for (i = 0; i < n; i++) listar_vector(m[i],n);
}

/*Verifica si 2 vectores son iguales*/
bool vectoresIguales(int v[],int w[],int tamn, int tamm){
    int i;
    if (tamn != tamm) return false;
    for (i = 0; i < tamn; ++i) {
        if (v[i] != w[i]) return false;
    }
    return true;
}

/*Verifica si 2 matrices son iguales*/
bool matricesIguales(matriz m, int n, int ref[][n]){
    int i;
    for (i=0;i<n;i++){
        if (vectoresIguales(m[i],ref[i],n,n))continue;
        else return false;
    }
    return true;
}


////////ALGORITMO A ANALIZAR////////

void dijkstra(matriz Matriz, matriz D, int dimension){
    int* noVisitados = malloc(sizeof(int) * dimension);
    int i1, i2, i3, nodo, nodoV, minimo;
    for (i1=0; i1<dimension; i1++){
        for (i2=0; i2<dimension; i2++){
            noVisitados[i2] = 1;
            D[i1][i2] = Matriz[i1][i2];
        }
        noVisitados[i1] = 0;

        for(i2=0; i2<(dimension-2);i2++){
            minimo = -1;
            nodo = i1;
            for(i3=0; i3<dimension; i3++){
                if (noVisitados[i3] && (D[i1][i3] < minimo || minimo == -1)){
                    minimo = D[i1][i3];
                    nodo = i3;
                }

            }
            noVisitados[nodo] = 0;
            for(nodoV=0; nodoV<dimension; nodoV++){
                if(noVisitados[nodoV]){
                    if (D[i1][nodoV]>(D[i1][nodo]+Matriz[nodo][nodoV])){
                        D[i1][nodoV]=(D[i1][nodo]+Matriz[nodo][nodoV]);
                    }
                }
            }
        }
    }
    free(noVisitados);
}


////////TESTS////////

void inicializarMatricesTest(matriz m1, matriz m2){
    int ref[][5] = {{0,1,8,4,7},
                    {1,0,2,6,5},
                    {8,2,0,9,5},
                    {4,6,9,0,3},
                    {7,5,5,3,0}};
    int ref2[][4] = {{0,1,4,7},
                     {1,0,2,8},
                     {4,2,0,3},
                     {7,8,3,0}};
    copiarMatriz(m1,5,ref);
    copiarMatriz(m2,4,ref2);
}

void printResultadoEsperado(int n, int ref[][n]){
    matriz aux = crearMatriz(n);
    copiarMatriz(aux,n,ref);
    printf("\nTABLA DISTANCIAS MINIMAS ESPERADA:\n");
    listar_matriz(aux,n);
    liberarMatriz(aux,n);
}

void Test(matriz m, matriz d, int n, int ref[][n]){
    printf("MATRIZ DE ADYACENCIA DEL GRAFO:\n");
    listar_matriz(m,n);
    dijkstra(m,d,n);
    printResultadoEsperado(n,ref);
    printf("\nTABLA DISTANCIAS MINIMAS OBTENIDA:\n");
    listar_matriz(d,n);
    printf("RESULTADO = %s\n", matricesIguales(d,n,ref) ? "OK" : "NOT OK");
    printf("\n");
}

void printTest(){
    matriz m,m2,d,d2;
    int n1 = 5, n2 = 4;
    m = crearMatriz(n1);
    m2 = crearMatriz(n2);
    d = crearMatriz(n1);
    d2 = crearMatriz(n2);
    int ref1[][5] = {{0,1,3,4,6},
                     {1,0,2,5,5},
                     {3,2,0,7,5},
                     {4,5,7,0,3},
                     {6,5,5,3,0}};

    int ref2[][4] = {{0,1,3,6},
                     {1, 0, 2, 5},
                     {3, 2, 0, 3},
                     {6, 5, 3, 0}};

    inicializarMatricesTest(m,m2);
    printf("------------TEST------------\n\n");
    printf("******GRAFO 1******\n");
    Test(m,d,n1,ref1);
    printf("******GRAFO 2******\n");
    Test(m2,d2,n2,ref2);
    liberarMatriz(m,n1);
    liberarMatriz(m2,n2);
    liberarMatriz(d,n1);
    liberarMatriz(d2,n2);
}

////////CALCULO DE TABLAS DE TIEMPOS////////

/* Calcular el tiempo real del programa */
double microsegundos(){
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

/*Mide el tiempo de ejecución*/
double medirTiempoAlgoritmo(matriz m,int n,
                            void (*algoritmo)(matriz,matriz,int)){
    double t0, t1;
    matriz d = crearMatriz(n);
    t0= microsegundos();
    algoritmo(m,d,n);
    t1 = microsegundos();
    liberarMatriz(d,n);
    return t1-t0;
}

/*Mide el tiempo de ejecucion del algoritmo en k ejecuciones*/
double medirTiempoAlgoritmoTPequenos(matriz m,int n, int k,
                                     void (*algoritmo)(matriz,matriz,int)){
    double t0, t1;
    int i;
    matriz d = crearMatriz(n);
    t0 = microsegundos();
    for (i = 0; i < k; i++) {
        iniMatriz(m,n);
        algoritmo(m,d,n);
    }
    t1 = microsegundos();
    liberarMatriz(d,n);
    return t1-t0;
}

/*Mide el tiempo de ejecucion del algoritmo en k ejecuciones*/
double medirTiempoGenerarMatriz(matriz m,int n,int k){
    double t0, t1;
    int i;

    t0 = microsegundos();
    for(i = 0; i < k; i++) iniMatriz(m,n);
    t1 = microsegundos();
    return t1 - t0;
}

/* Funcion para calcular el tiempo de ejecucion del algoritmo */
void tTiempos(double x, double y, double z,
              void (*algoritmo)(matriz,matriz,int)){
    double tf0, tf1, tf2;
    int n;
    int k =1000;
    matriz m;
    for (n=16; n <= TAM_MAX; n*=2) {
        m = crearMatriz(n);
        iniMatriz(m, n);
        tf0 = medirTiempoAlgoritmo(m,n,algoritmo);
        if (tf0<500) {
            tf1 = medirTiempoAlgoritmoTPequenos(m,n,k,algoritmo);
            tf2 = medirTiempoGenerarMatriz(m,n,k);
            tf0 = (tf1 - tf2)/k;
            printf("(*) | %5d | %17.5lf | %15.5lf | %19.5lf | %21.5lf |\n",
                   n, tf0, tf0/ pow(n,x), tf0/ pow(n,y), tf0/ pow(n,z));

        } else {
            printf("    | %5d | %17.5lf | %15.5lf | %19.5lf | %21.5lf |\n",
                   n, tf0, tf0/ pow(n,x), tf0/ pow(n,y), tf0/ pow(n,z));
        }
        liberarMatriz(m,n);
    }
}

/* Imprime las tablas */
void printTable( int max_prints, double cota1, double cota2, double cota3,
                 void (*algoritmo)(matriz,matriz,int)){
    int i;
    for (i=0;i<max_prints;i++) {
                if (i==0){
            printf("\n\n****TABLA: %d (MEDIDAS ANÓMALAS, DESCARTADA)****",i);
        }
        else printf("\n\n****TABLA: %d****",i);
        printf("\n%5s%6s |%18s |%12s%.2f |%16s%.2f |%18s%.2f |\n",
               "|","n", "t(n)","t(n)/n^",
               cota1,"t(n)/n^",cota2,"t(n)/n^",cota3);
        tTiempos(cota1,cota2,cota3,algoritmo);
    }
}

/* Funcion que imprime las tablas de los tiempos del algoritmo*/
void printTiempos(){
    int max_prints = 4;
    printf("\n------------MEDICION DE TIEMPOS------------\n");
    printTable(max_prints,2.6,2.74,2.9,dijkstra);
}

////////MAIN////////

int main() {
    inicializar_semilla();
    printTest();
    printTiempos();
    return 0;
}

