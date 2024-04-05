#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>

/*
 Examen prácticas
*/


/* Imprime la lista de vectores */
void listar_vector(int v[], int n){
    int i;
    for (i = 0; i < n; i++) printf(" %2d", v[i]);
}

/* Genera numeros aleatorios */
void aleatorio(int v[], int n){
    int i, m=2*n+1;
    for (i = 0; i < n; i++) v[i] = (rand() % m) - n;
}

/* Genera vector ascendiente */
void ascendiente(int v[], int n){
    int i;
    for (i = 0; i < n; i++) v[i] = i;
}

/* Genera vector descendiente */
void descendiente(int v[], int n){
    int i,j;
    for (i = n-1, j = 0; i >=0; i--, j++){
        v[j] = i;
    }
}

/* Copia un vector */
void copiarVector(int v[], int w[], int n, int ini){
    int i;
    for (i = ini; i < n; ++i) w[i] = v[i];
}

void intercambiar(int v[],int pos1,int pos2){
    int aux;
    aux=v[pos1];
    v[pos1]=v[pos2];
    v[pos2]=aux;
}

/* Codigo en C del algoritmo. */
void auxOrd(int v[], int izq, int der){
    int pivote, i, j;
    if (izq<der){
        pivote = v[der-1];
        intercambiar(v,izq,der-1);
        i = izq+1;
        j = der;
        while (i<=j){
            while (v[j]>pivote) j--;
            while (i<=der && v[i]<pivote) i++;
            if (i<=j){
                intercambiar(v,i,j);
                i++;
                j--;
            }
        }
        intercambiar(v,izq,j);
        auxOrd(v,izq,j-1);
        auxOrd(v,j+1,der);
    }
}

void ordenar(int v[], int n){
    auxOrd(v,0,n-1);
}

/* Calcula si un vector está ordenado */
void estaOrdenado(int v[], int n) {
    int i;
    for (i = 0; i < (n-1); ++i){
        if (v[i] > v[i + 1]){
            printf("\nEl vector no está ordenado\n");
            return;
        }
    }
    printf("\nEl vector está ordenado\n");
}

/* Funcionalidad del test */
void ordenarVectorTest(void (*funct)(int[],int), int v[], int n){
    funct(v, n);
    listar_vector(v, n);
    estaOrdenado(v,n);
}

/* Realiza el test */
void test(void (*funct)(int[],int), int numPruebas){
    int i, n = 9;
    int v[n],w[n];
    for (i = 0; i < numPruebas; i++) {
        printf("\n---PRUEBA: %d---",i+1);
        printf("\nVECTOR SIN ORDENAR:\n");
        funct(v, n);
        copiarVector(v,w,n,0);
        listar_vector(v, n);
        estaOrdenado(v,n);
        printf("\nVECTOR ORDENADO\n");
        ordenarVectorTest(ordenar,v,n);
    }
}

/* Imprime el resultado del test */
void printTest(){
    printf("\n\n---TEST CON VECTORES ALEATORIOS---");
    test(aleatorio, 1);
    printf("\n\n---TEST CON VECTORES ASCENDENTES---");
    test(ascendiente, 1);
    printf("\n\n---TEST CON VECTORES DESCENDENTES---");
    test(descendiente, 1);
}

/* Se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
void inicializar_semilla() {
    srand(time(NULL));
}

/* Calcular el tiempo real del programa */
double microsegundos(){
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

/* Funcion para calcular el tiempo de ejecucion de los algoritmos 1 y 2 */
void tTiempos(void (*algoritmo)(int[],int), void (*vector)(int[],int) , 
	          double x, double y, double z){
    double t0, t1, tf0, tf1, tf2;
    int v[32000], n, i;
    int k =1000;
    for (n = 500; n <= 32000; n*=2) {
        vector(v, n);
        t0= microsegundos();
        algoritmo(v, n);
        t1 = microsegundos();
        tf0 = t1 - t0;
        if (tf0<500) {
            t0 = microsegundos();
            for (i = 0; i < k; i++) {
                vector(v,n);
                algoritmo(v, n);
            }
            t1 = microsegundos();
            tf1 = t0 - t1;
            t0 = microsegundos();
            for(i = 0; i < k; i++) vector(v,n);
            t1 = microsegundos();
            tf2 = t0 - t1;
            tf0 = (tf2 - tf1)/k;
            printf("(*) %5d %15lf %17.7lf %25.7lf %25.7lf\n",
                   n,tf0,tf0/(pow(n,x)*log(n)),tf0/(pow(n,y)*log(n)),
                   tf0/(pow(n,z)*log(n)));
        } else {
            printf("    %5d %15lf %17.7lf %25.7lf %25.7lf\n",
                   n,tf0,tf0/(pow(n,x)*log(n)),tf0/(pow(n,y)*log(n)),
                   tf0/(pow(n,z)*log(n)));
        }
    }
}

/* Imprime las tablas */
void printTable(void (*algoritmo)(int[],int), void (*tipo_vector)(int[],int), 
		    int max_prints, double cota1, double cota2, double cota3){
    int i;
    for (i=0;i<max_prints;i++) {
        printf("\n%8s %14s %11s %.2f %s %11s %.2f %s %11s %.1f %s\n",
               "n", "t(n)", "t(n)/n ^", cota1, "* log(n)", "t(n)/n ^",
               cota2, "* log(n)", "t(n)/n ^", cota3 , "* log(n)");
        tTiempos(algoritmo,tipo_vector,cota1,cota2,cota3);
    }
}

/* Ejecuta la impresión de varias tablas x veces */
void printTiempos(){
    int max_prints = 2;
    printf("\n---MEDICIÓN DE TIEMPOS---\n");
    printf("\n**Ordenación vector ascendiente**\n");
    printTable(ordenar,ascendiente,max_prints,1.6,1.86,2);
    printf("\n**Ordenación vector descendiente**\n");
    printTable(ordenar,descendiente,max_prints,1.6,1.86,2);
    printf("\n**Ordenación vector aleatorio**\n");
    printTable(ordenar,aleatorio,max_prints,0.5,0.97,1.5);

}

/* main */
int main() {
    inicializar_semilla();
    printTest();
    printTiempos();
    return 0;
}
