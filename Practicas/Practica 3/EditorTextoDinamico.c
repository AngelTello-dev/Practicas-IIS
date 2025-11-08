#include <stdio.h>
#include <stdlib.h> // malloc, realloc, free, exit
#include <string.h> // strlen, strcpy, strstr, etc.

void rellenar_buffer(char **buffer, const char *texto_fuente) {
    // asigna memoria (longitud del texto + 1 para '\0')
    *buffer = (char *)malloc(strlen(texto_fuente) + 1);

    if (*buffer == NULL) {
        printf("Error: No se pudo asignar memoria inicial.\n");
        exit(1); // termina el programa si falla malloc
    }
    
    // copia el texto fuente al búfer
    strcpy(*buffer, texto_fuente);
}

void agregar_texto(char **buffer, const char *texto_a_agregar) {
    int longitud_actual = strlen(*buffer);
    int longitud_agregar = strlen(texto_a_agregar);
    int nueva_longitud_total = longitud_actual + longitud_agregar + 1;

    // intenta expandir la memoria
    char *nuevo_buffer = (char *)realloc(*buffer, nueva_longitud_total);

    if (nuevo_buffer == NULL) {
        printf("Error: No se pudo re-asignar memoria (realloc).\n");
        free(*buffer); // se libera el búfer original si realloc falla
        exit(1);
    }

    // actualiza el puntero original a la nueva dirección de memoria
    *buffer = nuevo_buffer;

    // concatena el nuevo texto (copiando en la posición del antiguo '\0')
    strcpy(*buffer + longitud_actual, texto_a_agregar);
}


char *buscar_y_reemplazar(const char *texto_original, const char *texto_a_buscar, const char *texto_de_reemplazo) {
    char *resultado;
    int ocurrencias = 0;
    int longitud_busqueda = strlen(texto_a_buscar);
    int longitud_reemplazo = strlen(texto_de_reemplazo);
    const char *puntero_temporal = texto_original;

    // contar ocurrencias para calcular memoria necesaria
    while ((puntero_temporal = strstr(puntero_temporal, texto_a_buscar))) {
        ocurrencias++;
        puntero_temporal += longitud_busqueda;
    }

    // calcular tamaño final y asignar memoria
    int longitud_resultado = strlen(texto_original) + ocurrencias * (longitud_reemplazo - longitud_busqueda);
    resultado = (char *)malloc(longitud_resultado + 1);

    if (resultado == NULL) {
        printf("Error: No se pudo asignar memoria para el texto final.\n");
        exit(1);
    }
    resultado[0] = '\0'; // inicializar la cadena resultante

    // construir la nueva cadena con los reemplazos
    puntero_temporal = texto_original;
    char *posicion_actual_resultado = resultado;

    while (1) {
        const char *coincidencia = strstr(puntero_temporal, texto_a_buscar);
        
        if (coincidencia == NULL) {
            // no hay más coincidencias, copiar el resto y salir
            strcpy(posicion_actual_resultado, puntero_temporal);
            break;
        }

        // copiar el fragmento antes de la coincidencia
        int caracteres_antes = coincidencia - puntero_temporal;
        strncpy(posicion_actual_resultado, puntero_temporal, caracteres_antes);
        posicion_actual_resultado[caracteres_antes] = '\0'; 
		// strncpy no siempre añade el nulo
        posicion_actual_resultado += caracteres_antes;

        // copiar el texto de reemplazo
        strcpy(posicion_actual_resultado, texto_de_reemplazo);
        posicion_actual_resultado += longitud_reemplazo;

        // mover el apuntador original
        puntero_temporal = coincidencia + longitud_busqueda;
    }

    return resultado;
}

int main() {
    // --- datos de entrada ---
    char *buffer_de_texto = NULL; // puntero para memoria dinámica
    const char *texto_inicial = "Hola mundo, este es un mundo maravilloso y un editor de texto basico. El mundo es genial.";
    const char *texto_para_incrementar = " ... Si lo puedes imaginar lo puedes programar -> Nota (agregado con la funcion realloc)";
    const char *texto_para_buscar = "mundo";
    const char *texto_para_reemplazar = "planeta";

    // llenar el búfer inicial (malloc)
    rellenar_buffer(&buffer_de_texto, texto_inicial);

    printf("--- Editor de Texto Basico en C ---\n\n");
    printf("1. Texto original:\n\"%s\"\n\n", buffer_de_texto);

    // mostrar incremento (realloc)
    agregar_texto(&buffer_de_texto, texto_para_incrementar);
    printf("2. Texto con incremento (despues de realloc):\n\"%s\"\n\n", buffer_de_texto);

    printf("3. Texto a buscar:\n\"%s\"\n\n", texto_para_buscar);

    printf("4. Texto a reemplazar:\n\"%s\"\n\n", texto_para_reemplazar);

    // realizar el reemplazo (malloc)
    char *texto_final = buscar_y_reemplazar(buffer_de_texto, texto_para_buscar, texto_para_reemplazar);

    printf("5. Texto final despues del reemplazo:\n\"%s\"\n\n", texto_final);

    free(buffer_de_texto); // libera la memoria de malloc/realloc
    free(texto_final);     // libera la memoria del resultado

    return 0;
}