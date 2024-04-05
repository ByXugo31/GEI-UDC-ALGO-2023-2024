#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

/*
 Practica 1 Algoritmos
*/

/* Codigo en C del primer algoritmo. */
int sumaSubMax1(int v[], int n){
    int estaSuma, sumaMax = 0;
    for(int i = 0; i < n; i++){
        estaSuma = 0;
        for (int j = i; j < n; j++) {
            estaSuma += v[j];
            if (estaSuma > sumaMax) {
                sumaMax = estaSuma;
            }

        }
    }
    return sumaMax;
}
/* Codigo en C del segundo algoritmo */
int sumaSubMax2(int v[], int n) {
    int estaSuma = 0;
    int sumaMax = 0;
    for (int j = 0; j < n; j++) {
        estaSuma = estaSuma + v[j];
        if (estaSuma > sumaMax){
            sumaMax = estaSuma;
        } else if(estaSuma < 0){
            estaSuma = 0;
        }

    }
    return sumaMax;
}

/* Imprime la lista de vectores */
void listar_vector(int v[], int n){
    for (int i = 0; i < n; i++){
        printf("%2d", v[i]);

    }
}
/* Genera numeros aleatorios */
void aleatorio(int v[], int n){
    int i, m=2*n+1;
    for (i = 0; i < n; i++) {
        v[i] = (rand() % m) - n;
    }
}

/* Funcion que realiza el test para el algoritmo 1 */
void testData1(int v[],int n,int result,char *nombre) {
    int suma1 = sumaSubMax1(v, n);
    if (suma1 == result) printf("serie de datos %s: OK\n", nombre);
    else printf("serie de datos %s: FALLO\n", nombre);
}
/* Funcion que realiza el test para el algoritmo 2 */
void testData2(int v[],int n,int result,char *nombre) {
    int suma2 = sumaSubMax2(v, n);
    if (suma2 == result) printf("serie de datos %s: OK\n", nombre);
    else printf("serie de datos %s: FALLO\n", nombre);
}
/* Paso de los vectores para ejecutar el test 1 */
void test1(){
    int d1[]={-9,2,-5,-4,6};
    int d2[]={4, 0, 9, 2, 5};
    int d3[]={-2, -1, -9, -7, -1};
    int d4[]={9, -2, 1, -7, -8 };
    int d5[]={15, -2, -5, -4, 16};
    int d6[]={7, -5, 6, 7, -7 };
    printf("****************TEST 1*****************\n");
    printf("En Algoritmo1\n");
    testData1(d1,sizeof(d1)/sizeof(int),6,"-9, 2, -5, -4, 6");
    testData1(d2,sizeof(d2)/sizeof(int),20,"4, 0, 9, 2, 5");
    testData1(d3,sizeof(d3)/sizeof(int),0,"-2, -1, -9, -7, -1");
    testData1(d4,sizeof(d4)/sizeof(int),9,"9, -2, 1, -7, -8 ");
    testData1(d5,sizeof(d5)/sizeof(int),20,"15, -2, -5, -4, 16");
    testData1(d6,sizeof(d6)/sizeof(int),15,"7, -5, 6, 7, -7 ");
    printf("En Algoritmo2\n");
    testData2(d1,sizeof(d1)/sizeof(int),6,"-9, 2, -5, -4, 6");
    testData2(d2,sizeof(d2)/sizeof(int),20,"4, 0, 9, 2, 5");
    testData2(d3,sizeof(d3)/sizeof(int),0,"-2, -1, -9, -7, -1");
    testData2(d4,sizeof(d4)/sizeof(int),9,"9, -2, 1, -7, -8 ");
    testData2(d5,sizeof(d5)/sizeof(int),20,"15, -2, -5, -4, 16");
    testData2(d6,sizeof(d6)/sizeof(int),15,"7, -5, 6, 7, -7 ");
}


/* test que se realiza a los algoritmo 1 y 2 con vectores aleatorios */
void test2(){
    int a, b;
    int v[9];
    printf("\ntest2\n");
    printf("%33s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
    for (int i = 0; i < 10; i++) {
        aleatorio(v, 9);
        listar_vector(v, 9);
        a = sumaSubMax1(v, 9);
        b = sumaSubMax2(v, 9);
        printf("%27d%15d\n", a, b);
    }
}
/* se establece la semilla de una nueva serie de enteros pseudo-aleatorios */
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
void tTiempos(int (*funct)(int[],int), double x, double y, double z){
    double t0, t1, tf;
    int v[32000], n;
    int k =1000;
    for (n = 500; n <= 32000; n= n*2) {
        aleatorio(v, n);
        t0= microsegundos();
        funct(v, n);
        t1 = microsegundos();
        tf = t1 - t0;
        if (tf<500) {
            t0 = microsegundos();
            for (int i = 0; i < k; i++) {
                funct(v, n);
            }
            t1 = microsegundos();
            tf = (t1 - t0)/k;
            printf("(*) %5d %15lf %15lf %15lf %15lf\n",
                   n, tf, tf/ pow(n, x), tf/ pow(n, y), tf/ pow(n, z));
        } else {
            printf("    %5d %15lf %15lf %15lf %15lf\n",
                   n, tf, tf/ pow(n, x), tf/ pow(n, y), tf/ pow(n, z));
        }
    }

}
/* Funcion que imprime las tablas de los tiempos de los algoritmos */
void printTiempos(){
    int i,max_prints = 3;
    printf("sumaSubMax 1\n");
    for (i=0;i<max_prints;i++) {
        printf("\n%8s%15s%19s%15s%16s\n",
               "n", "t(n)", "t(n)/n^1.8", "t(n)/n^2", "t(n)/n^2.2");
        tTiempos(sumaSubMax1,1.8,2,2.2);
    }
    printf("sumaSubMax 2\n");
    for (i=0;i<max_prints;i++) {
        printf("\n%8s%15s%19s%15s%16s\n",
               "n", "t(n)", "t(n)/n^0.8", "t(n)/n^1", "t(n)/n^1.2");
        tTiempos(sumaSubMax2,0.8,1,1.2);
    }
}

int main() {
    inicializar_semilla();
    test1();
    test2();
    printTiempos();
    return 0;
}
