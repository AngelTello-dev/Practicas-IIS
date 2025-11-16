#include <stdio.h>
#include <stdlib.h> // malloc, realloc, free, exit
#include <string.h> // strlen, strcpy, strstr, strcspn, etc.

/*
PROYECTO FINAL - EDITOR DE TEXTOS PERSONALIZADOS CON PRESISTENCIA DE DATOS (TXT).
Autor: Tello Montes De Oca Angel Antonio

Este programa permite crear plantillas de texto con marcadores [NOMBRE] y [CARGO],
capturar datos de múltiples personas, y generar documentos personalizados para cada una.
*/

// Estructura para almacenar los datos de una persona
typedef struct {
    char *nombre;   // Nombre de la persona (memoria dinámica)
    char *cargo;    // Cargo o puesto de la persona (memoria dinámica)
} Persona;

/*
FUNCIÓN: rellenar_buffer
OBJETIVO: Asignar memoria y copiar texto a un buffer
PARÁMETROS:
  - buffer: Puntero doble donde se guardará el texto
  - texto_fuente: Texto original que se va a copiar
*/
void rellenar_buffer(char **buffer, const char *texto_fuente) {
    // Asigno memoria suficiente para el texto + el carácter nulo
    *buffer = (char *)malloc(strlen(texto_fuente) + 1);

    if (*buffer == NULL) {
        printf("Error: No se pudo asignar memoria inicial (malloc).\n");
        exit(1); // Si falla, cierro el programa
    }
    
    // Copio el texto fuente al buffer
    strcpy(*buffer, texto_fuente);
}

/*
FUNCIÓN: agregar_texto
OBJETIVO: Añadir más texto al final de un buffer existente
PARÁMETROS:
  - buffer: Puntero doble al texto existente
  - texto_a_agregar: Nuevo texto que se añadirá al final
*/
void agregar_texto(char **buffer, const char *texto_a_agregar) {
    // Si el buffer está vacío, simplemente lo relleno con el nuevo texto
    if (*buffer == NULL) {
        rellenar_buffer(buffer, texto_a_agregar);
        return;
    }
    
    // Calculo las longitudes necesarias
    int longitud_actual = strlen(*buffer);
    int longitud_agregar = strlen(texto_a_agregar);
    int nueva_longitud_total = longitud_actual + longitud_agregar + 1;

    // Expando la memoria existente usando realloc
    char *nuevo_buffer = (char *)realloc(*buffer, nueva_longitud_total);

    if (nuevo_buffer == NULL) {
        printf("Error: No se pudo re-asignar memoria (realloc).\n");
        exit(1);
    }

    // Actualizo el puntero y concateno el nuevo texto
    *buffer = nuevo_buffer;
    strcpy(*buffer + longitud_actual, texto_a_agregar);
}

/*
FUNCIÓN: buscar_y_reemplazar
OBJETIVO: Buscar todas las ocurrencias de un texto y reemplazarlas
PARÁMETROS:
  - texto_original: Texto donde se buscará
  - texto_a_buscar: Texto que se quiere reemplazar
  - texto_de_reemplazo: Texto que se usará como reemplazo
RETORNO: Nuevo string con los reemplazos realizados
*/
char *buscar_y_reemplazar(const char *texto_original, const char *texto_a_buscar, const char *texto_de_reemplazo) {
    char *resultado;
    int ocurrencias = 0;
    int longitud_busqueda = strlen(texto_a_buscar);
    int longitud_reemplazo = strlen(texto_de_reemplazo);
    const char *puntero_temporal = texto_original;

    // Cuento cuántas veces aparece el texto a buscar
    while ((puntero_temporal = strstr(puntero_temporal, texto_a_buscar))) {
        ocurrencias++;
        puntero_temporal += longitud_busqueda;
    }

    // Calculo el tamaño final y asigno memoria
    int longitud_resultado = strlen(texto_original) + ocurrencias * (longitud_reemplazo - longitud_busqueda);
    resultado = (char *)malloc(longitud_resultado + 1);

    if (resultado == NULL) {
        printf("Error: No se pudo asignar memoria para el texto final.\n");
        exit(1);
    }
    resultado[0] = '\0'; // Inicializo la cadena vacía

    // Construyo la nueva cadena con los reemplazos
    puntero_temporal = texto_original;
    char *posicion_actual_resultado = resultado;

    while (1) {
        const char *coincidencia = strstr(puntero_temporal, texto_a_buscar);
        
        if (coincidencia == NULL) {
            // No hay más coincidencias, copio el resto del texto
            strcpy(posicion_actual_resultado, puntero_temporal);
            break;
        }

        // Copio la parte antes de la coincidencia
        int caracteres_antes = coincidencia - puntero_temporal;
        strncpy(posicion_actual_resultado, puntero_temporal, caracteres_antes);
        posicion_actual_resultado[caracteres_antes] = '\0';
        posicion_actual_resultado += caracteres_antes;

        // Copio el texto de reemplazo
        strcpy(posicion_actual_resultado, texto_de_reemplazo);
        posicion_actual_resultado += longitud_reemplazo;

        // Muevo el puntero original
        puntero_temporal = coincidencia + longitud_busqueda;
    }

    return resultado;
}

/*
FUNCIÓN: capturar_texto_base
OBJETIVO: Capturar la plantilla base del usuario con marcadores [NOMBRE] y [CARGO]
PARÁMETROS:
  - buffer: Puntero doble donde se guardará el texto base
*/
void capturar_texto_base(char **buffer) {
    // Si ya había texto, lo libero para empezar de nuevo
    if (*buffer != NULL) {
        free(*buffer);
        *buffer = NULL;
    }
    
    char temp_buffer[2048]; // Buffer temporal para la entrada
    
    printf("\n--- 1. Capturar Texto Base ---\n");
    printf("Introduce el texto. Usa [NOMBRE] y [CARGO] como marcadores.\n");
    printf("Ej: Hola [NOMBRE], le informamos que su puesto de [CARGO]...\n");
    printf("-> ");

    // Uso fgets para leer la línea completa con espacios
    fgets(temp_buffer, 2048, stdin);
    
    // Quito el salto de línea final
    temp_buffer[strcspn(temp_buffer, "\n")] = 0;

    // Uso mi función para asignar la memoria exacta
    rellenar_buffer(buffer, temp_buffer);
    
    printf("\nTexto base guardado:\n\"%s\"\n", *buffer);
}

/*
FUNCIÓN: modificar_texto_base
OBJETIVO: Permitir modificar el texto base (agregar al final o buscar/reemplazar)
PARÁMETROS:
  - buffer: Puntero doble al texto base que se modificará
*/
void modificar_texto_base(char **buffer) {
    if (*buffer == NULL) {
        printf("Error: Primero debe capturar un texto (Opcion 1).\n");
        return;
    }

    int sub_opcion;
    printf("\n--- 2. Modificar Texto Base ---\n");
    printf("Texto actual:\n\"%s\"\n\n", *buffer);
    printf("1. Agregar texto al final\n");
    printf("2. Buscar y reemplazar\n");
    printf("Seleccione: ");

    char buffer_opcion[10];
    fgets(buffer_opcion, 10, stdin);
    if (sscanf(buffer_opcion, "%d", &sub_opcion) != 1) {
        sub_opcion = 0;
    }

    if (sub_opcion == 1) {
        char temp_agregar[1024];
        printf("Introduce texto para agregar al final:\n-> ");
        fgets(temp_agregar, 1024, stdin);
        temp_agregar[strcspn(temp_agregar, "\n")] = 0;
        
        // Uso mi función para agregar texto
        agregar_texto(buffer, temp_agregar);
        printf("Texto modificado:\n\"%s\"\n", *buffer);

    } else if (sub_opcion == 2) {
        char temp_buscar[100];
        char temp_reemplazar[100];

        printf("Texto a buscar: ");
        fgets(temp_buscar, 100, stdin);
        temp_buscar[strcspn(temp_buscar, "\n")] = 0;

        printf("Reemplazar con: ");
        fgets(temp_reemplazar, 100, stdin);
        temp_reemplazar[strcspn(temp_reemplazar, "\n")] = 0;

        // Uso mi función de buscar y reemplazar
        char *texto_modificado = buscar_y_reemplazar(*buffer, temp_buscar, temp_reemplazar);
        
        // Libero el buffer original y lo reemplazo por el nuevo
        free(*buffer);
        *buffer = texto_modificado;
        
        printf("Texto modificado:\n\"%s\"\n", *buffer);
    } else {
        printf("Opcion no valida.\n");
    }
}

/*
FUNCIÓN: capturar_personas
OBJETIVO: Capturar los datos de múltiples personas (nombre y cargo)
PARÁMETROS:
  - grupo: Puntero doble al arreglo de personas
  - num_personas: Puntero al contador de personas
*/
void capturar_personas(Persona **grupo, int *num_personas) {
    // Si ya había personas, libero esa memoria primero
    if (*grupo != NULL && *num_personas > 0) {
        printf("Borrando datos de personas anteriores...\n");
        for (int i = 0; i < *num_personas; i++) {
            free((*grupo)[i].nombre);
            free((*grupo)[i].cargo);
        }
        free(*grupo);
        *grupo = NULL;
    }

    printf("\n--- 3. Capturar Datos de Personas ---\n");
    printf("Cuantas personas desea agregar?: ");
    
    char buffer_num[10];
    fgets(buffer_num, 10, stdin);
    if (sscanf(buffer_num, "%d", num_personas) != 1) {
        *num_personas = 0;
    }

    if (*num_personas <= 0) {
        *grupo = NULL;
        *num_personas = 0;
        printf("Entrada no valida. No se agregaran personas.\n");
        return;
    }

    // Asigno memoria para el arreglo de estructuras Persona
    *grupo = (Persona *)malloc(*num_personas * sizeof(Persona));
    if (*grupo == NULL) {
        printf("Error de memoria al crear el grupo.\n");
        *num_personas = 0;
        return;
    }

    // Pido datos para cada persona
    for (int i = 0; i < *num_personas; i++) {
        char temp_nombre[100];
        char temp_cargo[100];

        printf("--- Persona %d ---\n", i + 1);
        printf("Nombre: ");
        fgets(temp_nombre, 100, stdin);
        temp_nombre[strcspn(temp_nombre, "\n")] = 0;

        printf("Cargo: ");
        fgets(temp_cargo, 100, stdin);
        temp_cargo[strcspn(temp_cargo, "\n")] = 0;

        // Uso mi función para asignar memoria y copiar los datos
        rellenar_buffer(&((*grupo)[i].nombre), temp_nombre);
        rellenar_buffer(&((*grupo)[i].cargo), temp_cargo);
    }
    printf("\n%d personas guardadas exitosamente.\n", *num_personas);
}

/*
FUNCIÓN: mostrar_resultado
OBJETIVO: Generar y mostrar documentos personalizados para cada persona
PARÁMETROS:
  - texto_base: Plantilla de texto con marcadores
  - grupo: Arreglo de personas
  - num_personas: Cantidad de personas en el arreglo
*/
void mostrar_resultado(const char *texto_base, Persona *grupo, int num_personas) {
    if (texto_base == NULL) {
        printf("Error: Falta el texto base (Opcion 1).\n");
        return;
    }
    if (grupo == NULL || num_personas == 0) {
         printf("Error: Faltan los datos de las personas (Opcion 3).\n");
        return;
    }

    printf("\n--- 4. RESULTADO PERSONALIZADO ---\n");

    for (int i = 0; i < num_personas; i++) {
        printf("\n--------------------------------------\n");
        printf("Documento para: %s (%s)\n", grupo[i].nombre, grupo[i].cargo);
        printf("--------------------------------------\n");

        // Reemplazo [NOMBRE] por el nombre real
        char *texto_temp = buscar_y_reemplazar(texto_base, "[NOMBRE]", grupo[i].nombre);
        
        // Reemplazo [CARGO] por el cargo real
        char *texto_final_persona = buscar_y_reemplazar(texto_temp, "[CARGO]", grupo[i].cargo);

        // Muestro en pantalla
        printf("%s\n", texto_final_persona);

        // Genero un archivo único para cada persona
        char nombre_archivo[200];
        sprintf(nombre_archivo, "Documento_para_%s.txt", grupo[i].nombre);

        // Creo y escribo en el archivo
        FILE *archivo_salida = fopen(nombre_archivo, "w");

        if (archivo_salida == NULL) {
            printf("¡Error! No se pudo crear el archivo %s\n", nombre_archivo);
        } else {
            // Escribo el texto personalizado en el archivo
            fprintf(archivo_salida, "%s\n", texto_final_persona);
            
            fclose(archivo_salida);
            printf(" -> (Documento guardado como: %s)\n", nombre_archivo);
        }

        // Libero la memoria de los reemplazos temporales
        free(texto_temp);
        free(texto_final_persona);
    }
    printf("\n--- Fin del Reporte ---\n");
}

/*
FUNCIÓN PRINCIPAL: main
OBJETIVO: Controlar el flujo principal del programa con un menú interactivo
*/
int main() {
    char *buffer_de_texto = NULL;   // Buffer principal para el texto base
    Persona *grupo_personas = NULL; // Arreglo dinámico de personas
    int num_personas = 0;           // Contador de personas
    int opcion;

    // Menú principal interactivo
    do {
        printf("\n=== Editor de Texto y Personalizador ===\n");
        printf("1. Crear/Capturar Texto Base\n");
        printf("2. Modificar Texto Base\n");
        printf("3. Capturar Datos de Personas\n");
        printf("4. Mostrar Resultado (Pantalla y Archivos)\n");
        printf("5. Salir\n");
        printf("==========================================\n");
        printf("Seleccione una opcion: ");
        
        // Uso un buffer para evitar problemas con la entrada
        char buffer_opcion[10];
        fgets(buffer_opcion, 10, stdin);
        
        // Convierto la entrada a número
        if (sscanf(buffer_opcion, "%d", &opcion) != 1) {
            opcion = 0; // Si no es número, es inválido
        }

        switch (opcion) {
            case 1:
                capturar_texto_base(&buffer_de_texto);
                break;
            case 2:
                modificar_texto_base(&buffer_de_texto);
                break;
            case 3:
                capturar_personas(&grupo_personas, &num_personas);
                break;
            case 4:
                mostrar_resultado(buffer_de_texto, grupo_personas, num_personas);
                break;
            case 5:
                printf("Saliendo y liberando memoria...\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 5);

    
    // --- LIBERACIÓN DE TODA LA MEMORIA DINÁMICA ANTES DE SALIR ---
    
    // Libero el buffer de texto principal
    if (buffer_de_texto != NULL) {
        free(buffer_de_texto);
    }

    // Libero la memoria del grupo de personas
    if (grupo_personas != NULL) {
        // Primero libero los strings dentro de cada estructura
        for (int i = 0; i < num_personas; i++) {
            free(grupo_personas[i].nombre);
            free(grupo_personas[i].cargo);
        }
        // Luego libero el arreglo de estructuras
        free(grupo_personas);
    }

    printf("Memoria liberada. Adios.\n");
    return 0;
}