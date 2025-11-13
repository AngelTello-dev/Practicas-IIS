#include <stdio.h>
#include <stdlib.h> // malloc, realloc, free
#include <string.h> // strcpy, strlen
#include <ctype.h>  // tolower

#define MAX_STRING 100 // longitud para las cadenas

struct Alumno {
    int id;
    char nombre[MAX_STRING];
    char matricula[MAX_STRING];
    char carrera[MAX_STRING];
    int grado;
};

struct Tarea {
    int id;
    char nombre[MAX_STRING];
    char descripcion[MAX_STRING];
    char fecha_creacion[20];
};

struct Calificacion {
    int id_alumno;
    int id_tarea;
    float calificacion;
};

// struct administrador
struct SistemaGestion {
    struct Alumno* alumnos;
    int num_alumnos;
    int capacidad_alumnos;

    struct Tarea* tareas;
    int num_tareas;
    int capacidad_tareas;

    struct Calificacion* calificaciones;
    int num_calificaciones;
    int capacidad_calificaciones;
};

// funciones auxiliares
void limpiarBuffer();
void leerString(char* destino, int max_len);
struct Alumno* buscarAlumnoPorID(struct SistemaGestion* sistema, int id);
struct Tarea* buscarTareaPorID(struct SistemaGestion* sistema, int id);

// funciones de 'realloc'
void agregarAlumno(struct SistemaGestion* sistema);
void agregarTarea(struct SistemaGestion* sistema);
void agregarCalificacion(struct SistemaGestion* sistema, int id_a, int id_t, float nota);

// funciones principales
void inicializarSistema(struct SistemaGestion* sistema);
void liberarMemoria(struct SistemaGestion* sistema);

// funciones del menú
void gestionarTareas(struct SistemaGestion* sistema);
void gestionarAlumnos(struct SistemaGestion* sistema);
void asignarCalificaciones(struct SistemaGestion* sistema);
void mostrarResultados(struct SistemaGestion* sistema);


int main() {
    struct SistemaGestion sistema;
    inicializarSistema(&sistema);
    
    int opcion;

    do {
        printf("\n--- GESTIÓN DE ALUMNOS Y TAREAS ---\n");
        printf("1. Gestionar Alumnos\n");
        printf("2. Gestionar Tareas\n");
        printf("3. Asignar Calificaciones\n");
        printf("4. Mostrar Resultados\n");
        printf("0. Salir\n");
        printf("Elige una opción: ");
        
        scanf("%d", &opcion);
        limpiarBuffer();

        switch (opcion) {
            case 1:
                gestionarAlumnos(&sistema);
                break;
            case 2:
                gestionarTareas(&sistema);
                break;
            case 3:
                asignarCalificaciones(&sistema);
                break;
            case 4:
                mostrarResultados(&sistema);
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 0);

    liberarMemoria(&sistema);
    return 0;
}

// limpia el buffer de entrada
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// lee un string de forma segura, evitando desbordamiento
void leerString(char* destino, int max_len) {
    fgets(destino, max_len, stdin);
    size_t len = strlen(destino);
    if (len > 0 && destino[len - 1] == '\n') {
        destino[len - 1] = '\0';
    } else {
        limpiarBuffer();
    }
}

// busca un alumno por su ID, devuelve apuntador o NULL
struct Alumno* buscarAlumnoPorID(struct SistemaGestion* sistema, int id) {
    for (int i = 0; i < sistema->num_alumnos; i++) {
        if (sistema->alumnos[i].id == id) {
            return &sistema->alumnos[i];
        }
    }
    return NULL;
}

// busca una tarea por su ID, devuelve apuntador o NULL
struct Tarea* buscarTareaPorID(struct SistemaGestion* sistema, int id) {
    for (int i = 0; i < sistema->num_tareas; i++) {
        if (sistema->tareas[i].id == id) {
            return &sistema->tareas[i];
        }
    }
    return NULL;
}


// pide datos y añade un alumno al arreglo dinámico
void agregarAlumno(struct SistemaGestion* sistema) {
    char nombre[MAX_STRING], matricula[MAX_STRING], carrera[MAX_STRING];
    int grado;

    printf("Nombre completo: ");
    leerString(nombre, MAX_STRING);
    printf("Matrícula: ");
    leerString(matricula, MAX_STRING);
    printf("Carrera: ");
    leerString(carrera, MAX_STRING);
    printf("Grado/Semestre: ");
    scanf("%d", &grado);
    limpiarBuffer();

    // validar de semestres, utilizo el strstr para guardar palabras reservadas para identificar Ingenieria o sus variates tendran un limite de 10 semestres
	// si de lo contrario no coincide se reduce a 8 semestres y identificado como licenciados
    if (strstr(carrera, "Ingenieria") || strstr(carrera, "ingenieria") || strstr(carrera, "Ing") || strstr(carrera, "ing")) {
        if (grado < 1 || grado > 10) {
            printf("Error: Ing. en Software es de 10 semestres. Grado inválido.\n");
            return;
        }
    } else {
        if (grado < 1 || grado > 8) {
            printf("Error: Licenciaturas son de 8 semestres. Grado inválido.\n");
            return;
        }
    }


    // gestionar memoria (realloc)
    if (sistema->num_alumnos == sistema->capacidad_alumnos) {
        int nueva_cap = (sistema->capacidad_alumnos == 0) ? 10 : sistema->capacidad_alumnos * 2;
        struct Alumno* nuevo_arr = realloc(sistema->alumnos, nueva_cap * sizeof(struct Alumno));
        
        if (nuevo_arr == NULL) {
            printf("Error: No se pudo reasignar memoria para alumnos.\n");
            return;
        }
        sistema->alumnos = nuevo_arr;
        sistema->capacidad_alumnos = nueva_cap;
    }
    
    // agregar el nuevo alumno
    struct Alumno* nuevo_alumno = &sistema->alumnos[sistema->num_alumnos];
    nuevo_alumno->id = (sistema->num_alumnos > 0) ? sistema->alumnos[sistema->num_alumnos - 1].id + 1 : 1;
    strcpy(nuevo_alumno->nombre, nombre);
    strcpy(nuevo_alumno->matricula, matricula);
    strcpy(nuevo_alumno->carrera, carrera);
    nuevo_alumno->grado = grado;
    
    sistema->num_alumnos++;
    printf("¡Alumno '%s' agregado con ID %d!\n", nuevo_alumno->nombre, nuevo_alumno->id);
}

// pide datos y añade una tarea al arreglo dinámico
void agregarTarea(struct SistemaGestion* sistema) {
    char nombre[MAX_STRING], descripcion[MAX_STRING], fecha[20];
    
    printf("Nombre de la tarea: ");
    leerString(nombre, MAX_STRING);
    printf("Descripción: ");
    leerString(descripcion, MAX_STRING);
    printf("Fecha Creación (YYYY-MM-DD): ");
    leerString(fecha, 20);

    // gestionar memoria (realloc)
    if (sistema->num_tareas == sistema->capacidad_tareas) {
        int nueva_cap = (sistema->capacidad_tareas == 0) ? 10 : sistema->capacidad_tareas * 2;
        struct Tarea* nuevo_arr = realloc(sistema->tareas, nueva_cap * sizeof(struct Tarea));
        
        if (nuevo_arr == NULL) {
            printf("Error: No se pudo reasignar memoria para tareas.\n");
            return;
        }
        sistema->tareas = nuevo_arr;
        sistema->capacidad_tareas = nueva_cap;
    }
    
    // agregar la nueva tarea
    struct Tarea* nueva_tarea = &sistema->tareas[sistema->num_tareas];
    nueva_tarea->id = (sistema->num_tareas > 0) ? sistema->tareas[sistema->num_tareas - 1].id + 1 : 1;
    strcpy(nueva_tarea->nombre, nombre);
    strcpy(nueva_tarea->descripcion, descripcion);
    strcpy(nueva_tarea->fecha_creacion, fecha);
    
    sistema->num_tareas++;
    printf("¡Tarea '%s' agregada con ID %d!\n", nueva_tarea->nombre, nueva_tarea->id);
}

// añade una calificación al arreglo dinámico
void agregarCalificacion(struct SistemaGestion* sistema, int id_a, int id_t, float nota) {
    // gestionar memoria con realloc
    if (sistema->num_calificaciones == sistema->capacidad_calificaciones) {
        int nueva_cap = (sistema->capacidad_calificaciones == 0) ? 10 : sistema->capacidad_calificaciones * 2;
        struct Calificacion* nuevo_arr = realloc(sistema->calificaciones, nueva_cap * sizeof(struct Calificacion));
        
        if (nuevo_arr == NULL) {
            printf("Error: No se pudo reasignar memoria para calificaciones.\n");
            return;
        }
        sistema->calificaciones = nuevo_arr;
        sistema->capacidad_calificaciones = nueva_cap;
    }
    
    // agregar la nueva calificación
    struct Calificacion* nueva_calif = &sistema->calificaciones[sistema->num_calificaciones];
    nueva_calif->id_alumno = id_a;
    nueva_calif->id_tarea = id_t;
    nueva_calif->calificacion = nota;
    
    sistema->num_calificaciones++;
    printf("¡Calificación asignada correctamente!\n");
}

// se usa 0los apuntadores a NULL y contadores a 0
void inicializarSistema(struct SistemaGestion* sistema) {
    sistema->alumnos = NULL;
    sistema->num_alumnos = 0;
    sistema->capacidad_alumnos = 0;

    sistema->tareas = NULL;
    sistema->num_tareas = 0;
    sistema->capacidad_tareas = 0;

    sistema->calificaciones = NULL;
    sistema->num_calificaciones = 0;
    sistema->capacidad_calificaciones = 0;
}

// libera toda la memoria dinámica reservada
void liberarMemoria(struct SistemaGestion* sistema) {
    free(sistema->alumnos);
    free(sistema->tareas);
    free(sistema->calificaciones);
    
    printf("Memoria liberada. Adiós.\n");
}

// inicio del menu 

// sub-menu para gestionar alumnos
void gestionarAlumnos(struct SistemaGestion* sistema) {
    int opcion;
    do {
        printf("\n--- Gestión de Alumnos ---\n");
        printf("1. Agregar Alumno\n");
        printf("2. Listar Alumnos\n");
        printf("0. Volver al menú principal\n");
        printf("Opción: ");
        
        scanf("%d", &opcion);
        limpiarBuffer();

        switch (opcion) {
            case 1:
                agregarAlumno(sistema);
                break;
            case 2:
                // solo mostramos la seccion de alumnos
                printf("\n--- Lista de Alumnos (%d) ---\n", sistema->num_alumnos);
                if (sistema->num_alumnos == 0) {
                    printf("No hay alumnos registrados.\n");
                } else {
                    for (int i = 0; i < sistema->num_alumnos; i++) {
                        struct Alumno* a = &sistema->alumnos[i];
                        printf("ID: %d | %s | %s | %s | Semestre: %d\n",
                               a->id, a->matricula, a->nombre, a->carrera, a->grado);
                    }
                }
                break;
            case 0:
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 0);
}

// sub-menu para gestionar tareas
void gestionarTareas(struct SistemaGestion* sistema) {
    int opcion;
    do {
        printf("\n--- Gestión de Tareas ---\n");
        printf("1. Agregar Tarea\n");
        printf("2. Listar Tareas\n");
        printf("0. Volver al menú principal\n");
        printf("Opción: ");
        
        scanf("%d", &opcion);
        limpiarBuffer();

        switch (opcion) {
            case 1:
                agregarTarea(sistema);
                break;
            case 2:
                // solo mostramos la sección de tareas
                printf("\n--- Lista de Tareas (%d) ---\n", sistema->num_tareas);
                if (sistema->num_tareas == 0) {
                    printf("No hay tareas registradas.\n");
                } else {
                    for (int i = 0; i < sistema->num_tareas; i++) {
                        struct Tarea* t = &sistema->tareas[i];
                        printf("ID: %d | %s (%s) | %s\n",
                               t->id, t->nombre, t->fecha_creacion, t->descripcion);
                    }
                }
                break;
            case 0:
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 0);
}

// proceso para asignar calificaciones
void asignarCalificaciones(struct SistemaGestion* sistema) {
    printf("\n--- Asignar Calificaciones ---\n");

    if (sistema->num_alumnos == 0 || sistema->num_tareas == 0) {
        printf("Error: Debe registrar al menos un alumno y una tarea antes de calificar.\n");
        return;
    }

    // mostramos listas para que el usuario sepa qué IDs usar
    printf("Alumnos disponibles:\n");
    for (int i = 0; i < sistema->num_alumnos; i++) {
        printf("  ID: %d, Nombre: %s\n", sistema->alumnos[i].id, sistema->alumnos[i].nombre);
    }
    printf("Tareas disponibles:\n");
    for (int i = 0; i < sistema->num_tareas; i++) {
        printf("  ID: %d, Nombre: %s\n", sistema->tareas[i].id, sistema->tareas[i].nombre);
    }

    int id_a, id_t;
    float nota;

    printf("\nIngrese el ID del Alumno: ");
    scanf("%d", &id_a);
    limpiarBuffer();

    printf("Ingrese el ID de la Tarea: ");
    scanf("%d", &id_t);
    limpiarBuffer();

    // validar que los IDs existan
    if (buscarAlumnoPorID(sistema, id_a) == NULL) {
        printf("Error: El ID de alumno %d no existe.\n", id_a);
        return;
    }
    if (buscarTareaPorID(sistema, id_t) == NULL) {
        printf("Error: El ID de tarea %d no existe.\n", id_t);
        return;
    }

    printf("Ingrese la Calificación (0-100): ");
    scanf("%f", &nota);
    limpiarBuffer();

    if (nota < 0 || nota > 100) {
        printf("Error: Calificación inválida.\n");
        return;
    }

    // si todo es válido, la agregamos
    agregarCalificacion(sistema, id_a, id_t, nota);
}

// muestra todos los datos de los 3 arreglos
void mostrarResultados(struct SistemaGestion* sistema) {
    printf("\n--- REPORTE COMPLETO DEL SISTEMA ---\n");

    // vista Alumnos
    printf("\n=== Alumnos (%d) ===\n", sistema->num_alumnos);
    if (sistema->num_alumnos == 0) {
        printf("No hay alumnos registrados.\n");
    } else {
        for (int i = 0; i < sistema->num_alumnos; i++) {
            struct Alumno* a = &sistema->alumnos[i];
            printf("ID: %d | %s | %s | %s | Semestre: %d\n",
                   a->id, a->matricula, a->nombre, a->carrera, a->grado);
        }
    }

    // vista tareas
    printf("\n=== Tareas (%d) ===\n", sistema->num_tareas);
    if (sistema->num_tareas == 0) {
        printf("No hay tareas registradas.\n");
    } else {
        for (int i = 0; i < sistema->num_tareas; i++) {
            struct Tarea* t = &sistema->tareas[i];
            printf("ID: %d | %s (%s) | %s\n",
                   t->id, t->nombre, t->fecha_creacion, t->descripcion);
        }
    }

    // vista calificaciones
    printf("\n=== Calificaciones Asignadas (%d) ===\n", sistema->num_calificaciones);
    if (sistema->num_calificaciones == 0) {
        printf("No hay calificaciones asignadas.\n");
    } else {
        for (int i = 0; i < sistema->num_calificaciones; i++) {
            struct Calificacion* c = &sistema->calificaciones[i];
            
            struct Alumno* a = buscarAlumnoPorID(sistema, c->id_alumno);
            struct Tarea* t = buscarTareaPorID(sistema, c->id_tarea);
            
            char* nombre_a = (a != NULL) ? a->nombre : "ID Alumno Desconocido";
            char* nombre_t = (t != NULL) ? t->nombre : "ID Tarea Desconocida";

            printf("Alumno: %-20s (ID: %d) | Tarea: %-15s (ID: %d) | Nota: %.2f\n",
                   nombre_a, c->id_alumno, nombre_t, c->id_tarea, c->calificacion);
        }
    }
}
