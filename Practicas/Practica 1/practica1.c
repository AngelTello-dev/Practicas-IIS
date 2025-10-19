#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Constantes
#define MAX_SIZE 10
#define MIN_VAL 1
#define MAX_VAL 100

// Prototipos de funciones
void llenarMatriz(int matriz[MAX_SIZE][MAX_SIZE], int tamaño);
void mostrarMatriz(int matriz[MAX_SIZE][MAX_SIZE], int tamaño, char* titulo);
int sumaTotal(int matriz[MAX_SIZE][MAX_SIZE], int tamaño);
void contarParesImpares(int matriz[MAX_SIZE][MAX_SIZE], int tamaño, int* pares, int* impares);
int contarDivisiblesSiete(int matriz[MAX_SIZE][MAX_SIZE], int tamaño);
void obtenerTranspuesta(int matriz[MAX_SIZE][MAX_SIZE], int transpuesta[MAX_SIZE][MAX_SIZE], int tamaño);

int main() {
    int matriz[MAX_SIZE][MAX_SIZE];
    int transpuesta[MAX_SIZE][MAX_SIZE];
    int tamaño;
    int pares, impares, divisiblesSiete;
    
    // Semilla para números aleatorios
    srand(time(NULL));
    
    printf("=== PRACTICA 1: ARREGLOS BIDIMENSIONALES ===\n\n");
    
    // Solicitar tamaño de la matriz
    do {
        printf("Ingrese el tamaño de la matriz (1-%d): ", MAX_SIZE);
        scanf("%d", &tamaño);
        
        if (tamaño < 1 || tamaño > MAX_SIZE) {
            printf("Error: El tamaño debe estar entre 1 y %d\n", MAX_SIZE);
        }
    } while (tamaño < 1 || tamaño > MAX_SIZE);
    
    // Llenar matriz con valores aleatorios
    llenarMatriz(matriz, tamaño);
    
    // Mostrar matriz original
    mostrarMatriz(matriz, tamaño, "MATRIZ ORIGINAL");
    
    // Calcular y mostrar resultados
    printf("\n=== RESULTADOS ===\n");
    printf("Suma total de elementos: %d\n", sumaTotal(matriz, tamaño));
    
    contarParesImpares(matriz, tamaño, &pares, &impares);
    printf("Elementos pares: %d\n", pares);
    printf("Elementos impares: %d\n", impares);
    
    divisiblesSiete = contarDivisiblesSiete(matriz, tamaño);
    printf("Elementos divisibles entre 7: %d\n", divisiblesSiete);
    
    // Obtener y mostrar matriz transpuesta
    obtenerTranspuesta(matriz, transpuesta, tamaño);
    mostrarMatriz(transpuesta, tamaño, "MATRIZ TRANSPUESTA");
    
    return 0;
}

// Función para llenar matriz con valores aleatorios
void llenarMatriz(int matriz[MAX_SIZE][MAX_SIZE], int tamaño) {
    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            matriz[i][j] = MIN_VAL + rand() % (MAX_VAL - MIN_VAL + 1);
        }
    }
}

// Función para mostrar matriz
void mostrarMatriz(int matriz[MAX_SIZE][MAX_SIZE], int tamaño, char* titulo) {
    printf("\n=== %s ===\n", titulo);
    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            printf("%4d", matriz[i][j]);
        }
        printf("\n");
    }
}

// Función para calcular suma total
int sumaTotal(int matriz[MAX_SIZE][MAX_SIZE], int tamaño) {
    int suma = 0;
    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            suma += matriz[i][j];
        }
    }
    return suma;
}

// Función para contar elementos pares e impares
void contarParesImpares(int matriz[MAX_SIZE][MAX_SIZE], int tamaño, int* pares, int* impares) {
    *pares = 0;
    *impares = 0;
    
    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            if (matriz[i][j] % 2 == 0) {
                (*pares)++;
            } else {
                (*impares)++;
            }
        }
    }
}

// Función para contar elementos divisibles entre 7
int contarDivisiblesSiete(int matriz[MAX_SIZE][MAX_SIZE], int tamaño) {
    int count = 0;
    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            if (matriz[i][j] % 7 == 0) {
                count++;
            }
        }
    }
    return count;
}

// Función para obtener matriz transpuesta
void obtenerTranspuesta(int matriz[MAX_SIZE][MAX_SIZE], int transpuesta[MAX_SIZE][MAX_SIZE], int tamaño) {
    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {
            transpuesta[j][i] = matriz[i][j];
        }
    }
}
