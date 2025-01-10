#include <stdio.h>
#include "funciones.h"
#include <stdio.h>

void createFactura();
void readFacturas();
void updateFactura();
void deleteFactura();

int main () {
    int opcion;
    do{
        printf("1. Crear factura\n");
        printf("2. Leer facturas\n");
        printf("3. Actualizar factura\n");
        printf("4. Eliminar factura\n");
        printf("5. Salir\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &opcion);
        switch(opcion){
            case 1:
                createFactura();
                break;
            case 2:
                readFacturas();
                break;
            case 3:
                updateFactura();
                break;
            case 4:
                deleteFactura();
                break;
            case 5:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida\n");
        }
    }while(opcion != 5);
    return 0;
    
}