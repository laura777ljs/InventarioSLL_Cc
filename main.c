#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int id;
    char name[40];
    int stock; // Stock requerido por el ejercicio
    struct Node* next;
} Producto;

typedef Producto *Lista; 

// Función para obtener un entero validado
int leer_entero(const char *mensaje) {
    int num;
    printf("%s", mensaje);
    while (scanf("%d", &num) != 1 || num < 0) {
        printf("Error. Ingrese solo un numero entero positivo: ");
        while (getchar() != '\n');
    }
    while (getchar() != '\n');
    return num;
}

// Función auxiliar para leer una cadena (nombre)
void leer_cadena(const char *mensaje, char *buffer, size_t size) {
    printf("%s", mensaje);
    if (fgets(buffer, size, stdin)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        } else {
            // Limpiar el buffer si el nombre es demasiado largo
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}

// Auxiliar: Crea y inicializa un nuevo producto
Lista crear_nodo(int id, const char *nombre, int stock) {
    Lista nuevo = (Lista)malloc(sizeof(Producto));
    if (nuevo == NULL) {
        perror("Error de memoria");
        exit(EXIT_FAILURE);
    }
    nuevo->id = id;
    strncpy(nuevo->name, nombre, 39);
    nuevo->name[39] = '\0';
    nuevo->stock = stock;
    nuevo->next = NULL;
    return nuevo;
}

// Busca el producto por ID
Lista buscar_producto(Lista cabeza, int id) {
    Lista actual = cabeza;
    while (actual != NULL) {
        if (actual->id == id) {
            return actual;
        }
        actual = actual->next;
    }
    return NULL;
}

// Función unificada de inserción (Inicio/Final) con validación de ID único
Lista insertar_producto(Lista cabeza, int id, const char *nombre, int stock, int posicion) {
    if (buscar_producto(cabeza, id) != NULL) {
        printf("ERROR: El ID %d ya existe en el inventario. No se inserto.\n", id);
        return cabeza;
    }

    Lista nuevo = crear_nodo(id, nombre, stock);

    if (posicion == 1) { // Insertar al INICIO
        nuevo->next = cabeza;
        printf(">> Producto ID %d insertado al INICIO.\n", id);
        return nuevo;
    } else { // Insertar al FINAL
        if (cabeza == NULL) {
            printf(">> Producto ID %d insertado al INICIO (lista vacia).\n", id);
            return nuevo;
        }

        Lista actual = cabeza;
        while (actual->next != NULL) {
            actual = actual->next;
        }
        
        actual->next = nuevo;
        printf(">> Producto ID %d insertado al FINAL.\n", id);
        return cabeza;
    }
}

// Actualizar stock por ID
void actualizar_stock(Lista cabeza) {
    int id = leer_entero("Ingrese el ID del producto a actualizar el stock: ");
    Lista prod = buscar_producto(cabeza, id);

    if (prod != NULL) {
        printf("Producto: %s (Stock actual: %d). ", prod->name, prod->stock);
        int nuevo_stock;
        do {
            nuevo_stock = leer_entero("Ingrese el NUEVO stock (>= 0): ");
        } while (nuevo_stock < 0);
        
        prod->stock = nuevo_stock;
        printf("Stock actualizado correctamente a %d.\n", prod->stock);
    } else {
        printf("ERROR: Producto con ID %d no encontrado.\n", id);
    }
}

// Eliminar producto por ID
Lista eliminar_producto(Lista cabeza) {
    int id = leer_entero("Ingrese el ID del producto a eliminar: ");
    Lista actual = cabeza;
    Lista previo = NULL;

    // El nodo a eliminar es la cabeza
    if (actual != NULL && actual->id == id) {
        cabeza = actual->next;
        free(actual);
        printf("Producto ID %d eliminado correctamente (era la cabeza).\n", id);
        return cabeza;
    }

    // Buscar el nodo a eliminar (intermedio o final)
    while (actual != NULL && actual->id != id) {
        previo = actual;
        actual = actual->next;
    }

    // Si actual es NULL, no se encontró
    if (actual == NULL) {
        printf("ERROR: Producto con ID %d no encontrado.\n", id);
        return cabeza;
    }

    // Desenlazar y liberar:
    previo->next = actual->next;
    free(actual);
    printf("Producto ID %d eliminado correctamente.\n", id);
    return cabeza;
}

// Listar el inventario completo
void listar_inventario(Lista cabeza) {
    if (cabeza == NULL) {
        printf("\n--- El inventario esta vacio ---\n");
        return;
    }
    Lista actual = cabeza;
    printf("\n--- INVENTARIO ACTUAL ---\n");
    printf("ID\tSTOCK\t\tNOMBRE\n");
    printf("-----------------------------------------\n");
    while (actual != NULL) {
        printf("%d\t%d\t\t%s\n", actual->id, actual->stock, actual->name);
        actual = actual->next;
    }
    printf("-----------------------------------------\n");
}

// Mostrar un producto (para el REQ 2)
void mostrar_producto(Lista producto) {
    if (producto != NULL) {
        printf(">> Producto encontrado: ID %d, Nombre: %s, Stock: %d\n", producto->id, producto->name, producto->stock);
    } else {
        printf(">> Producto no encontrado.\n");
    }
}


// MENÚ PRINCIPAL
void menu() {
    Lista inventario = NULL;
    int opcion;
    int id, stock, pos;
    char nombre[40];
    Lista temp;

    do {
        printf("\n======== GESTION DE INVENTARIO (SLL) ========\n");
        printf("1. Insertar producto (Inicio/Final)\n");
        printf("2. Buscar datos de producto por ID\n");
        printf("3. Actualizar stock por ID\n");
        printf("4. Eliminar producto por ID\n");
        printf("5. Listar inventario completo\n");
        printf("0. SALIR\n");
        opcion = leer_entero("Seleccione una opcion: ");

        switch (opcion) {
            case 1:
                printf("\n--- INSERCION ---\n");
                id = leer_entero("ID (entero positivo): ");
                leer_cadena("Nombre (max 39 chars): ", nombre, 40); 
                stock = leer_entero("Stock: ");

                pos = 0;
                while (pos != 1 && pos != 2) {
                    pos = leer_entero("Donde insertar (1=Inicio, 2=Final): ");
                    if (pos != 1 && pos != 2) {
                        printf("Opcion de posicion invalida. Intente de nuevo.\n");
                    }
                }
                
                inventario = insertar_producto(inventario, id, nombre, stock, pos);
                break;
            case 2:
                id = leer_entero("Ingrese el ID a buscar: ");
                temp = buscar_producto(inventario, id);
                mostrar_producto(temp);
                break;
            case 3:
                actualizar_stock(inventario);
                break;
            case 4:
                inventario = eliminar_producto(inventario);
                break;
            case 5:
                listar_inventario(inventario);
                break;
            case 0:
                printf("Saliendo y liberando memoria. ¡Adios!\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 0);
    
    // Liberar toda la memoria de la lista
    Lista actual = inventario;
    Lista siguiente;
    while(actual != NULL) {
        siguiente = actual->next;
        free(actual);
        actual = siguiente;
    }
}

int main() {
    menu();
    return 0;
}