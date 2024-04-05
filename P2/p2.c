#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>

/*
 Practica 2 Algoritmos
*/

/* Codigo en C del primer algoritmo. */
void ord_ins(int v[], int n){
    int i,x,j;
    for (i = 0 ; i < n; i++){
        x = v[i];
        j = i - 1;
        while (j >= 0 && v[j] > x){
            v[j+1] = v[j];
            j = j - 1;
        }
        v[j + 1] = x;
    }
}

/* Codigo en C del segundo algoritmo */
void ord_shell(int v[], int n){
    int incremento = n;
    int i,tmp,j;
    bool seguir;
    do{
        incremento = incremento/2;
        for(i=incremento;i<n;i++){
            tmp=v[i];
            j=i;
            seguir=true;
            while(j-incremento>=0 && seguir){
                if(tmp < v[j-incremento]){
                    v[j]=v[j-incremento];
                    j=j-incremento;
                }else seguir=false;
            }
            v[j]=tmp;
        }
    }while(incremento!=1);
}

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
void bestCase(int v[], int n){
    int i;
    for (i = 0; i < n; i++) v[i] = i;
}

/* Genera vector descendiente */
void worstCase(int v[], int n){
    int i,j;
    for (i = n-1, j = 0; i >=0; i--, j++){
        v[j] = i;
    }
}

/* Copia un vector */
void copiarVector(int v[], int w[], int n){
    int i;
    for (i = 0; i < n; ++i) w[i] = v[i];
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
        copiarVector(v,w,n);
        listar_vector(v, n);
        estaOrdenado(v,n);
        printf("\nVECTOR ORDENADO POR INSERCIÓN\n");
        ordenarVectorTest(ord_ins,v,n);
        printf("\nVECTOR ORDENADO POR SHELL\n");
        ordenarVectorTest(ord_shell,w,n);
    }
}

/* Imprime el resultado del test */
void printTest(){
    printf("\n\n---TEST CON VECTORES ALEATORIOS---");
    test(aleatorio, 1);
    printf("\n\n---TEST CON EL MEJOR CASO---");
    test(bestCase, 1);
    printf("\n\n---TEST CON EL PEOR CASO---");
    test(worstCase, 1);
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
            printf("(*) %5d %15lf %15lf %15lf %15lf\n",
                   n, tf0, tf0/ pow(n, x), tf0/ pow(n, y), tf0/ pow(n, z));
        } else {
            printf("    %5d %15lf %15lf %15lf %15lf\n",
                   n, tf0, tf0/ pow(n, x), tf0/ pow(n, y), tf0/ pow(n, z));
        }

    }
}

/* Imprime las tablas */
void printTable(void (*algoritmo)(int[],int), void (*tipo_vector)(int[],int), 
		    int max_prints, double cota1, double cota2, double cota3){
    int i;
    for (i=0;i<max_prints;i++) {
        printf("\n%8s%15s%15s%.2f%12s%.2f%12s%.2f\n",
               "n", "t(n)", "t(n)/n^",cota1, "t(n)/n^",cota2, "t(n)/n^",cota3);
        tTiempos(algoritmo,tipo_vector,cota1,cota2,cota3);
    }
}

/* Ejecuta la impresión de varias tablas x veces */
void printTiempos(){
    int max_prints = 2;
    printf("\n---MEDICIÓN DE TIEMPOS---\n");
    printf("\nOrdenación por inserción mejor caso\n");
    printTable(ord_ins,bestCase,max_prints,0.7,1.02,1.2);
    printf("\nOrdenación por inserción peor caso\n");
    printTable(ord_ins,worstCase,max_prints,1.8,2,2.2);
    printf("\nOrdenación por inserción aleatorio\n");
    printTable(ord_ins,aleatorio,max_prints,1.8,2,2.2);
    printf("\nOrdenacion shell mejor caso\n");
    printTable(ord_shell,bestCase,max_prints,0.9,1.15,1.3);
    printf("\nOrdenacion shell peor caso\n");
    printTable(ord_shell,worstCase,max_prints,1,1.11,1.3);
    printf("\nOrdenacion shell aleatorio\n");
    printTable(ord_shell,aleatorio,max_prints,1,1.16,1.3);

}

/* main */
int main() {
    inicializar_semilla();
    printTest();
    printTiempos();
    return 0;
}
