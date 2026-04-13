/*
 * ==========================================================================
 *  EJERCICIO 1 — Simulación del juego "Snake"
 * ==========================================================================
 *
 *  La víbora se representa como una lista doblemente ligada donde:
 *    - La CABEZA de la lista es la cabeza de la víbora.
 *    - Cada nodo almacena la posición (fila * 100 + columna) de un segmento.
 *    - El último nodo es la cola de la víbora.
 *
 *  ¿Por qué lista doblemente ligada?
 *    • Crecer: al comer comida, se agrega un segmento al final (insertar_final).
 *    • Moverse: se agrega un nuevo segmento en la cabeza (insertar_inicio)
 *      y se elimina el último segmento (eliminar_final). Ambas operaciones
 *      requieren recorrer eficientemente la lista.
 *    • Colisión consigo misma: se necesita recorrer el cuerpo para verificar
 *      si la nueva posición de la cabeza coincide con algún segmento.
 *    • El puntero "previo" permite recorrer la víbora desde la cola cuando
 *      es necesario (por ejemplo, para imprimirla en reversa).
 *
 *  Las y los alumnos deben completar las funciones marcadas con TODO.
 *  Compile con:
 *      gcc -Wall -Wextra -o snake snake.c listadl.c
 *
 * ==========================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

typedef int DATO;

typedef struct nodo {
    DATO dato;
    struct nodo *siguiente;
    struct nodo *previo;
} dllista;

typedef struct {
    dllista *cabeza;
    dllista *cola;
} ListaDL;

ListaDL* crear_lista() {
    ListaDL *lista = (ListaDL*)malloc(sizeof(ListaDL));
    lista->cabeza = lista->cola = NULL;
    return lista;
}

void insertar_inicio(ListaDL *lista, DATO dato) {
    dllista *nuevo = (dllista*)malloc(sizeof(dllista));
    nuevo->dato = dato;
    nuevo->previo = NULL;
    nuevo->siguiente = lista->cabeza;
    if (lista->cabeza == NULL) lista->cola = nuevo;
    else lista->cabeza->previo = nuevo;
    lista->cabeza = nuevo;
}

void insertar_final(ListaDL *lista, DATO dato) {
    dllista *nuevo = (dllista*)malloc(sizeof(dllista));
    nuevo->dato = dato;
    nuevo->siguiente = NULL;
    nuevo->previo = lista->cola;
    if (lista->cola == NULL) lista->cabeza = nuevo;
    else lista->cola->siguiente = nuevo;
    lista->cola = nuevo;
}

void eliminar_final(ListaDL *lista) {
    if (lista->cola == NULL) return;
    dllista *temp = lista->cola;
    if (lista->cabeza == lista->cola) {
        lista->cabeza = lista->cola = NULL;
    } else {
        lista->cola = lista->cola->previo;
        lista->cola->siguiente = NULL;
    }
    free(temp);
}

int buscar(ListaDL *lista, DATO dato) {
    dllista *curr = lista->cabeza;
    while (curr) {
        if (curr->dato == dato) return 1;
        curr = curr->siguiente;
    }
    return -1;
}

int longitud(ListaDL *lista) {
    int cont = 0;
    dllista *curr = lista->cabeza;
    while (curr) { cont++; curr = curr->siguiente; }
    return cont;
}

void liberar_lista(ListaDL *lista) {
    dllista *curr = lista->cabeza;
    while (curr) {
        dllista *temp = curr;
        curr = curr->siguiente;
        free(temp);
    }
    free(lista);
}

/*LÓGICA*/
#define FILAS    15
#define COLUMNAS 30
#define POS(f, c) ((f) * 100 + (c))
#define FILA(p)   ((p) / 100)
#define COL(p)    ((p) % 100)

#define ARRIBA    0
#define ABAJO     1
#define IZQUIERDA 2
#define DERECHA   3

DATO generar_comida(ListaDL *vibora) {
    DATO pos;
    do {
        int f = 1 + rand() % (FILAS - 2);
        int c = 1 + rand() % (COLUMNAS - 2);
        pos = POS(f, c);
    } while (buscar(vibora, pos) != -1);
    return pos;
}

void dibujar_tablero(ListaDL *vibora, DATO comida) {
    char tablero[FILAS][COLUMNAS + 1];
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            if (f == 0 || f == FILAS - 1 || c == 0 || c == COLUMNAS - 1)
                tablero[f][c] = '#';
            else
                tablero[f][c] = '.';
        }
        tablero[f][COLUMNAS] = '\0';
    }

    tablero[FILA(comida)][COL(comida)] = '*';

    dllista *seg = vibora->cabeza;
    int primero = 1;
    while (seg != NULL) {
        tablero[FILA(seg->dato)][COL(seg->dato)] = primero ? 'O' : 'o';
        primero = 0;
        seg = seg->siguiente;
    }

    for (int f = 0; f < FILAS; f++) printf("  %s\n", tablero[f]);
}

DATO calcular_nueva_cabeza(DATO cabeza_actual, int direccion) {
    int f = FILA(cabeza_actual);
    int c = COL(cabeza_actual);
    if (direccion == ARRIBA) f--;
    else if (direccion == ABAJO) f++;
    else if (direccion == IZQUIERDA) c--;
    else if (direccion == DERECHA) c++;
    return POS(f, c);
}

int mover_vibora(ListaDL *vibora, int direccion, DATO comida) {
    DATO nueva_pos = calcular_nueva_cabeza(vibora->cabeza->dato, direccion);
    insertar_inicio(vibora, nueva_pos);
    if (nueva_pos == comida) return 1;
    eliminar_final(vibora);
    return 0;
}

   
/*MAIN */

int main() {
    srand(42);
    ListaDL *vibora = crear_lista();
    
  
    insertar_final(vibora, POS(7, 15));
    insertar_final(vibora, POS(7, 14));
    insertar_final(vibora, POS(7, 13));

    DATO comida = generar_comida(vibora);
    int puntaje = 0;

    int movimientos[] = {
        DERECHA, DERECHA, DERECHA, DERECHA, ABAJO, ABAJO, 
        IZQUIERDA, IZQUIERDA, IZQUIERDA, ARRIBA, DERECHA, DERECHA
    };
    int total = sizeof(movimientos) / sizeof(movimientos[0]);

    for (int i = 0; i < total; i++) {
       
        system("cls"); 

        int dir = movimientos[i];
        DATO siguiente = calcular_nueva_cabeza(vibora->cabeza->dato, dir);

       
        if (FILA(siguiente) <= 0 || FILA(siguiente) >= FILAS-1 || 
            COL(siguiente) <= 0 || COL(siguiente) >= COLUMNAS-1 ||
            buscar(vibora, siguiente) != -1) {
            printf("  ¡GAME OVER!\n");
            break;
        }

        
        if (mover_vibora(vibora, dir, comida)) {
            puntaje += 10;
            comida = generar_comida(vibora);
        }

       
        dibujar_tablero(vibora, comida);
        printf("  Turno: %d | Puntaje: %d | Longitud: %d\n", i+1, puntaje, longitud(vibora));
        
        fflush(stdout);
        Sleep(400); 
    }

    printf("\n  Simulacion finalizada. Presiona Enter para salir...");
    liberar_lista(vibora);
    getchar();
    return 0;
}
