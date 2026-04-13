/*
 * ==========================================================================
 *  EJERCICIO 2 — Carrusel de inventario (lista doblemente ligada circular)
 * ==========================================================================
 *
 *  En muchos videojuegos el jugador dispone de un inventario de herramientas
 *  que puede recorrer cíclicamente con las teclas "siguiente" y "anterior".
 *
 *  En este ejercicio se implementa un CARRUSEL CIRCULAR usando la estructura
 *  dllista directamente (sin ListaDL), conectando el último elemento con el
 *  primero en ambos sentidos. Un puntero "seleccion" indica la herramienta
 *  actualmente equipada.
 *
 *  ¿Por qué lista doblemente ligada?
 *    • Avanzar y retroceder en el carrusel en O(1) gracias a los punteros
 *      "siguiente" y "previo".
 *    • Insertar y eliminar herramientas en cualquier posición sin perder
 *      la capacidad de navegar en ambas direcciones.
 *
 *  Las y los alumnos deben completar las funciones marcadas con TODO.
 *  Compile con:
 *      gcc -Wall -Wextra -o carrusel carrusel.c listadl.c
 *
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include "listadl.h"
#include <string.h>

/* ---------- Herramientas del inventario ---------- */
/* Cada herramienta se identifica con un entero (DATO) */
#define ESPADA      0
#define ARCO        1
#define ESCUDO      2
#define POCION      3
#define BOMBA       4
#define ANTORCHA    5
#define GANCHO      6
#define LLAVE       7

#define NUM_HERRAMIENTAS 8

const char *nombre_herramienta(DATO id) {
    const char *nombres[] = {
        "Espada", "Arco", "Escudo", "Poción",
        "Bomba", "Antorcha", "Gancho", "Llave"
    };
    if (id >= 0 && id < NUM_HERRAMIENTAS)
        return nombres[id];
    return "???";
}

/* ================================================================
 *  Funciones proporcionadas (no modificar)
 * ================================================================ */

/*
 * Muestra el carrusel centrado en la selección actual.
 * Muestra 2 herramientas a cada lado de la seleccionada.
 */
void mostrar_carrusel(dllista *seleccion) {
    if (seleccion == NULL) {
        printf("  [ Inventario vacío ]\n");
        return;
    }

    /* Retroceder 2 posiciones para mostrar contexto */
    dllista *inicio = seleccion->previo->previo;

    printf("  ");
    dllista *actual = inicio;
    for (int i = 0; i < 5; i++) {
        if (actual == seleccion)
            printf(" [> %s <] ", nombre_herramienta(actual->dato));
        else
            printf("   %s   ", nombre_herramienta(actual->dato));

        if (i < 4)
            printf("-");
        actual = actual->siguiente;
    }
    printf("\n");
}

/*
 * Cuenta los elementos del carrusel circular.
 */
int contar_carrusel(dllista *cualquiera) {
    if (cualquiera == NULL)
        return 0;
    int cuenta = 1;
    dllista *actual = cualquiera->siguiente;
    while (actual != cualquiera) {
        cuenta++;
        actual = actual->siguiente;
    }
    return cuenta;
}

/*
 * Imprime todo el inventario en orden desde un nodo dado.
 */
void imprimir_inventario(dllista *desde) {
    if (desde == NULL) {
        printf("  (vacío)\n");
        return;
    }
    printf("  Inventario: ");
    dllista *actual = desde;
    do {
        printf("%s", nombre_herramienta(actual->dato));
        actual = actual->siguiente;
        if (actual != desde)
            printf(" -> ");
    } while (actual != desde);
    printf(" -> (vuelve a %s)\n", nombre_herramienta(desde->dato));
}

/* ================================================================
 *  Funciones por completar
 * ================================================================ */

/*
 *  TODO 1: insertar_en_carrusel
 *
 *  Inserta un nuevo elemento en el carrusel circular DESPUÉS del
 *  nodo "despues_de".
 *
 *  Si el carrusel está vacío (despues_de == NULL), crea un nodo
 *  que apunte a sí mismo en ambos sentidos (previo y siguiente).
 *
 *  Si no está vacío, inserta el nuevo nodo entre "despues_de" y
 *  "despues_de->siguiente", actualizando los 4 punteros necesarios.
 *
 *  Retorna: un puntero al nuevo nodo insertado.
 *
 *  Pista: usa crear_elemento() para crear el nodo.
 */
dllista *insertar_en_carrusel(dllista *despues_de, DATO dato) {
    dllista *nuevo = crear_elemento(dato);
    if (nuevo == NULL)
        return despues_de;

    if (despues_de == NULL) {
        // El carrusel está vacío: se apunta a sí mismo
        nuevo->siguiente = nuevo;
        nuevo->previo = nuevo;
        return nuevo;
    }

    // Insertar entre despues_de y despues_de->siguiente
    nuevo->siguiente = despues_de->siguiente;
    nuevo->previo = despues_de;
    despues_de->siguiente->previo = nuevo;
    despues_de->siguiente = nuevo;

    return nuevo;
}

/*
 *  TODO 2: eliminar_del_carrusel
 *
 *  Elimina el nodo "objetivo" del carrusel circular y libera su memoria.
 *
 *  Si el carrusel solo tiene un elemento (objetivo->siguiente == objetivo),
 *  libera el nodo y retorna NULL.
 *
 *  Si tiene más elementos, conecta el nodo previo con el siguiente,
 *  libera el nodo eliminado, y retorna el nodo siguiente (la nueva
 *  selección).
 *
 *  Retorna: puntero al nodo siguiente, o NULL si el carrusel queda vacío.
 */
dllista *eliminar_del_carrusel(dllista *objetivo) {
    if (objetivo == NULL)
        return NULL;

    // Caso 1: solo un elemento
    if (objetivo->siguiente == objetivo) {
        free(objetivo);
        return NULL;
    }

    // Caso 2: más de un elemento
    dllista *siguiente = objetivo->siguiente;

    objetivo->previo->siguiente = objetivo->siguiente;
    objetivo->siguiente->previo = objetivo->previo;

    free(objetivo);

    return siguiente;
}

/*
 *  TODO 3: avanzar
 *
 *  Avanza la selección "n" posiciones hacia adelante (siguiente).
 *
 *  Retorna: el nodo en la nueva posición.
 */
dllista *avanzar(dllista *seleccion, int n) {
    if (seleccion == NULL)
        return NULL;

    for (int i = 0; i < n; i++) {
        seleccion = seleccion->siguiente;
    }

    return seleccion;
}

/*
 *  TODO 4: retroceder
 *
 *  Retrocede la selección "n" posiciones hacia atrás (previo).
 *
 *  Retorna: el nodo en la nueva posición.
 */
dllista *retroceder(dllista *seleccion, int n) {
    if (seleccion == NULL)
        return NULL;

    for (int i = 0; i < n; i++) {
        seleccion = seleccion->previo;
    }

    return seleccion;
}

/* ================================================================
 *  Simulación del carrusel de inventario
 * ================================================================ */

int main(void) {
    printf("╔══════════════════════════════════════╗\n");
    printf("║     CARRUSEL DE INVENTARIO           ║\n");
    printf("╚══════════════════════════════════════╝\n\n");

    /* --- Construir el inventario inicial --- */
    printf(">> Construyendo inventario inicial...\n\n");

    dllista *seleccion = NULL;
    seleccion = insertar_en_carrusel(seleccion, ESPADA);
    seleccion = insertar_en_carrusel(seleccion, ARCO);
    seleccion = insertar_en_carrusel(seleccion, ESCUDO);
    seleccion = insertar_en_carrusel(seleccion, POCION);
    seleccion = insertar_en_carrusel(seleccion, BOMBA);

    /* Volver a la Espada como selección inicial */
    seleccion = avanzar(seleccion, 1);

    printf("  Inventario inicial (%d objetos):\n", contar_carrusel(seleccion));
    imprimir_inventario(seleccion);
    printf("\n");

    /* --- Navegar hacia adelante --- */
    printf(">> Avanzar 1 posición (siguiente):\n");
    seleccion = avanzar(seleccion, 1);
    mostrar_carrusel(seleccion);
    printf("  Equipado: %s\n\n", nombre_herramienta(seleccion->dato));

    printf(">> Avanzar 2 posiciones:\n");
    seleccion = avanzar(seleccion, 2);
    mostrar_carrusel(seleccion);
    printf("  Equipado: %s\n\n", nombre_herramienta(seleccion->dato));

    /* --- Navegar hacia atrás --- */
    printf(">> Retroceder 1 posición (anterior):\n");
    seleccion = retroceder(seleccion, 1);
    mostrar_carrusel(seleccion);
    printf("  Equipado: %s\n\n", nombre_herramienta(seleccion->dato));

    printf(">> Retroceder 3 posiciones:\n");
    seleccion = retroceder(seleccion, 3);
    mostrar_carrusel(seleccion);
    printf("  Equipado: %s\n\n", nombre_herramienta(seleccion->dato));

    /* --- Agregar herramienta --- */
    printf(">> Recoger Antorcha (se agrega después de la selección actual):\n");
    insertar_en_carrusel(seleccion, ANTORCHA);
    printf("  Inventario (%d objetos):\n", contar_carrusel(seleccion));
    imprimir_inventario(seleccion);
    mostrar_carrusel(seleccion);
    printf("\n");

    /* --- Eliminar herramienta --- */
    printf(">> Usar Bomba (se elimina del inventario):\n");
    /* Navegar hasta la bomba */
    while (seleccion->dato != BOMBA)
        seleccion = avanzar(seleccion, 1);
    printf("  Eliminando: %s\n", nombre_herramienta(seleccion->dato));
    seleccion = eliminar_del_carrusel(seleccion);
    printf("  Nueva selección: %s\n", nombre_herramienta(seleccion->dato));
    printf("  Inventario (%d objetos):\n", contar_carrusel(seleccion));
    imprimir_inventario(seleccion);
    mostrar_carrusel(seleccion);
    printf("\n");

    /* --- Dar una vuelta completa --- */
    printf(">> Vuelta completa al carrusel:\n");
    int total = contar_carrusel(seleccion);
    dllista *recorrido = seleccion;
    printf("  ");
    for (int i = 0; i < total; i++) {
        printf("%s", nombre_herramienta(recorrido->dato));
        recorrido = avanzar(recorrido, 1);
        if (i < total - 1) printf(" -> ");
    }
    printf(" -> (vuelta)\n\n");

    printf(">> Vuelta completa en reversa:\n");
    recorrido = seleccion;
    printf("  ");
    for (int i = 0; i < total; i++) {
        printf("%s", nombre_herramienta(recorrido->dato));
        recorrido = retroceder(recorrido, 1);
        if (i < total - 1) printf(" -> ");
    }
    printf(" -> (vuelta)\n\n");

    /* --- Liberar memoria --- */
    printf(">> Vaciando inventario...\n");
    while (seleccion != NULL)
        seleccion = eliminar_del_carrusel(seleccion);
    printf("  Inventario final: %d objetos\n", contar_carrusel(seleccion));

    return 0;
}
