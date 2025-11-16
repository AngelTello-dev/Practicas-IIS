#include <stdio.h>
#include <stdlib.h> // malloc, realloc, free, exit
#include <string.h> // strlen, strcpy, strstr, strcspn, etc.

// estructura para almacenar los datos de una persona
typedef struct {
    char *nombre;
    char *cargo;
} Persona;

void rellenar_buffer(char **buffer, const char *texto_fuente) {
    // asigna memoria (longitud del texto + 1 para '\0')
    *buffer = (char *)malloc(strlen(texto_fuente) + 1);

    if (*buffer == NULL) {
        printf("Error: No se pudo asignar memoria inicial (malloc).\n");
        exit(1); // termina el programa si falla malloc
    }
    
    // copia el texto fuente al búfer
    strcpy(*buffer, texto_fuente);
}


void agregar_texto(char **buffer, const char *texto_a_agregar) {
    // Si el buffer está vacío, llamamos a rellenar_buffer
    if (*buffer == NULL) {
        rellenar_buffer(buffer, texto_a_agregar);
        return;
    }
    
    int longitud_actual = strlen(*buffer);
    int longitud_agregar = strlen(texto_a_agregar);
    int nueva_longitud_total = longitud_actual + longitud_agregar + 1;

    // intenta expandir la memoria
    char *nuevo_buffer = (char *)realloc(*buffer, nueva_longitud_total);

    if (nuevo_buffer == NULL) {
        printf("Error: No se pudo re-asignar memoria (realloc).\n");
        // no liberamos *buffer aquí, porque realloc fallido no lo libera
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
        posicion_actual_resultado[caracteres_antes] = '\0'; // strncpy no siempre añade el nulo
        posicion_actual_resultado += caracteres_antes;

        // copiar el texto de reemplazo
        strcpy(posicion_actual_resultado, texto_de_reemplazo);
        posicion_actual_resultado += longitud_reemplazo;

        // mover el apuntador original
        puntero_temporal = coincidencia + longitud_busqueda;
    }

    return resultado;
}

// nuevas funciones
void capturar_texto_base(char **buffer) {
    // si ya había un texto, lo liberamos para empezar de nuevo
    if (*buffer != NULL) {
        free(*buffer);
        *buffer = NULL;
    }
    
    char temp_buffer[2048]; // Un búfer temporal grande para la entrada
    printf("\n--- 1. Capturar Texto Base ---\n");
    printf("Introduce el texto. Usa [NOMBRE] y [CARGO] como marcadores.\n");
    printf("Ej: Hola [NOMBRE], le informamos que su puesto de [CARGO]...\n");
    printf("-> ");

    // usar fgets para leer la línea completa (incluyendo espacios)
    fgets(temp_buffer, 2048, stdin);
    
    // quitar el salto de línea final que deja fgets
    temp_buffer[strcspn(temp_buffer, "\n")] = 0;

    // usar tu función existente para asignar la memoria exacta
    rellenar_buffer(buffer, temp_buffer);
    
    printf("\nTexto base guardado:\n\"%s\"\n", *buffer);
}

// permite modificar el texto (agregar o reemplazar).
// utiliza agregar_texto y buscar_y_reemplazar.
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
        
        // usamos tu funcion existente
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

        // usamos tu función existente
        char *texto_modificado = buscar_y_reemplazar(*buffer, temp_buscar, temp_reemplazar);
        
        // el buffer original debe ser liberado y reemplazado por el nuevo
        free(*buffer);
        *buffer = texto_modificado; // apuntar al nuevo buffer
        
        printf("Texto modificado:\n\"%s\"\n", *buffer);
    } else {
        printf("Opcion no valida.\n");
    }
}

// pide al usuario los datos de N personas.
void capturar_personas(Persona **grupo, int *num_personas) {
    // si ya había personas, liberar esa memoria primero
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

    // asignar memoria para el ARREGLO de structs
    *grupo = (Persona *)malloc(*num_personas * sizeof(Persona));
    if (*grupo == NULL) {
        printf("Error de memoria al crear el grupo.\n");
        *num_personas = 0;
        return;
    }

    // pedir datos para cada persona
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

        rellenar_buffer(&((*grupo)[i].nombre), temp_nombre);
        rellenar_buffer(&((*grupo)[i].cargo), temp_cargo);
    }
    printf("\n%d personas guardadas exitosamente.\n", *num_personas);
}

// muestra el resultado de mezclar texto y personas.
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

        // reemplazar [NOMBRE] (Usando la función del usuario)
        char *texto_temp = buscar_y_reemplazar(texto_base, "[NOMBRE]", grupo[i].nombre);
        
        // reemplazar [CARGO] (sobre el resultado anterior)
        char *texto_final_persona = buscar_y_reemplazar(texto_temp, "[CARGO]", grupo[i].cargo);

        // mostrar EN PANTALLA (Esto ya lo tenías)
        printf("%s\n", texto_final_persona);

        // --- INICIO DE LA MODIFICACIÓN (GENERAR ARCHIVO) ---
		
        // crear un nombre de archivo único
        char nombre_archivo[200];
        sprintf(nombre_archivo, "Documento_para_%s.txt", grupo[i].nombre);

        // abrir el archivo en modo escritura ("w")
        FILE *archivo_salida = fopen(nombre_archivo, "w");

        if (archivo_salida == NULL) {
            printf("¡Error! No se pudo crear el archivo %s\n", nombre_archivo);
        } else {
            // escribir el mismo texto final en el archivo
            fprintf(archivo_salida, "%s\n", texto_final_persona);
            
            // cerrar el archivo
            fclose(archivo_salida);
            printf(" -> (Documento guardado como: %s)\n", nombre_archivo);
        }
		
        // --- FIN DE LA MODIFICACIÓN ---


        // liberar la memoria de estos reemplazos temporales
        free(texto_temp);
        free(texto_final_persona);
    }
    printf("\n--- Fin del Reporte ---\n");
}


int main() {
    char *buffer_de_texto = NULL; // bufer de texto principal
    Persona *grupo_personas = NULL; // arreglo dinámico de personas
    int num_personas = 0;         // contador de personas
    int opcion;

    do {
        printf("\n=== Editor de Texto y Personalizador ===\n");
        printf("1. Crear/Capturar Texto Base\n");
        printf("2. Modificar Texto Base\n");
        printf("3. Capturar Datos de Personas\n");
        printf("4. Mostrar Resultado (Pantalla y Archivos)\n"); // <- modifique el texto para avisar que habra n archivos.
        printf("5. Salir\n");
        printf("==========================================\n");
        printf("Seleccione una opcion: ");
        
        // bufer de entrada para evitar errores con scanf
        char buffer_opcion[10];
        fgets(buffer_opcion, 10, stdin);
        
        // convertir la entrada a un entero
        if (sscanf(buffer_opcion, "%d", &opcion) != 1) {
            opcion = 0; // Si no es un numero, es invalido
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

    
    // --- LIBERACIÓN DE TODA LA MEMORIA DINÁMICA ---
    
    // liberar el buffer de texto principal
    if (buffer_de_texto != NULL) {
        free(buffer_de_texto);
    }

    // liberar la memoria del grupo de personas
    if (grupo_personas != NULL) {
        // primero liberar los punteros dentro de cada struct
        for (int i = 0; i < num_personas; i++) {
            free(grupo_personas[i].nombre); // libera el nombre
            free(grupo_personas[i].cargo);  // libera el cargo
        }
        // finalmente, liberar el arreglo de structs
        free(grupo_personas);
    }

    printf("Memoria liberada. Adios.\n");
    return 0;
}
