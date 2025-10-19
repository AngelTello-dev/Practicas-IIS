#include <stdio.h>
#include <string.h>

// Define constantes para los tamaños de los buffers.
#define TAMANO_BUFFER 1024
#define TAMANO_RESULTADO 2048

// Función que copia un texto a un buffer
void rellenarBuffer(char *bufferDestino, const char *textoFuente) {
    strcpy(bufferDestino, textoFuente);
}

// Función que busca y reemplaza texto dentro de una cadena
void buscarReemplazar(const char *textoOriginal, const char *textoBuscar, const char *textoReemplazo, char *bufferResultado) {

    // Apuntador para recorrer el texto original
    const char *punteroActual = textoOriginal;
    // Obtiene el largo del texto a buscar
    int longitudBusqueda = strlen(textoBuscar);

    // Inicia el buffer de resultado como una cadena vacía
    bufferResultado[0] = '\0';

    // Bucle para buscar todas las ocurrencias
    while (1) {
        // Encuentra la siguiente aparición del texto a buscar
        const char *coincidencia = strstr(punteroActual, textoBuscar);

        // Si ya no se encuentra el texto, sale del bucle
        if (coincidencia == NULL) {
            // Agrega el resto del texto original
            strcat(bufferResultado, punteroActual);
            // Termina el ciclo
            break;
        }

        // Calcula la distancia desde el punto actual hasta la coincidencia
        int longitudSegmento = coincidencia - punteroActual;
        // Copia el fragmento de texto antes de la palabra encontrada
        strncat(bufferResultado, punteroActual, longitudSegmento);

        // Agrega el texto de reemplazo
        strcat(bufferResultado, textoReemplazo);

        // Mueve el apuntador para continuar la búsqueda después de la palabra ya reemplazada
        punteroActual = coincidencia + longitudBusqueda;
    }
}


int main() {
    // Declara los arreglos de caracteres (cadenas)
    char Buffer [TAMANO_BUFFER];
    char textFinal[TAMANO_RESULTADO];

    // Define las cadenas de texto que se usarán
    const char *textInicio = "Hola mundo, este es un mundo maravilloso y un editor de texto basico. El mundo es genial.";
    const char *textBuscador = "mundo";
    const char *textReemplazado = "planeta";

    // Llena el buffer inicial con el texto de inicio
    rellenarBuffer(Buffer, textInicio);

    // Imprime el texto original
    printf("Texto original (Buffer inicial):\n\"%s\"\n\n", Buffer);

    // Imprime la palabra a buscar
    printf("Texto a buscar:\n\"%s\"\n\n", textBuscador);

    // Imprime la palabra de reemplazo
    printf("Texto a reemplazar:\n\"%s\"\n\n", textReemplazado);

    // Llama a la función para hacer el reemplazo
    buscarReemplazar(Buffer, textBuscador, textReemplazado, textFinal);

    // Imprime el resultado final
    printf("Texto final (Buffer de resultado):\n\"%s\"\n\n", textFinal);

    // Finaliza el programa
    return 0;
}
