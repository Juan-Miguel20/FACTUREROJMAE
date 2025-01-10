#include <stdio.h>
#include "funciones.h" 
#include <string.h>

void saveFactura(struct Factura *factura){
    FILE *f;
    f = fopen("facturas.dat", "ab+");
    if (f == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }else{
        fwrite(factura, sizeof(struct Factura), 1, f);
        printf("Factura guardada con exito\n");
    }

    fclose(f);
}

void leerCadena(char *cadena, int longitud){
    fflush(stdin);
    fgets(cadena, longitud, stdin);
    int len = strlen(cadena)-1;
    cadena[len] = '\0';
}

void createFactura(){
    struct Factura factura;
    printf("Ingrese el nombre del cliente: ");
    leerCadena(factura.nombre, 50);
    printf("Ingrese la cedula del cliente: ");
    scanf("%d", &factura.cedula);
    printf("Ingrese el numero de productos: ");
    scanf("%d", &factura.Nproductos);
    factura.total = 0;
    factura.activo = 1;
    for (int i = 0; i < factura.Nproductos; i++){
        printf("Ingrese el nombre del producto %d: ", i+1);
        leerCadena(factura.productos[i].nombre, 50);
        printf("Ingrese la cantidad del producto %d: ", i+1);
        scanf("%d", &factura.productos[i].cantidad);
        printf("Ingrese el precio del producto %d: ", i+1);
        scanf("%f", &factura.productos[i].precio);
        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;

    }   
    saveFactura(&factura);
}

void readFacturas(){
    FILE *f;
    struct Factura factura;
    f = fopen("facturas.dat", "rb");
    if (f == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }
    int hayfacturas = 0;
    struct Factura facturas;

    while(fread(&factura, sizeof(struct Factura), 1, f)==1){
        if (factura.activo){
            hayfacturas = 1;
            printf("Nombre del cliente: %s\n", factura.nombre);
            printf("Cedula del cliente: %d\n", factura.cedula);
            printf("#\t\tNombre\t\tPrecio\t\tCantidad\n");
            for (int i = 0; i < factura.Nproductos; i++){
                printf("%d\t\t%s\t\t%.2f\t\t%d\n", i+1, factura.productos[i].nombre, 
                                                        factura.productos[i].precio,
                                                        factura.productos[i].cantidad);
            }
            printf("Total: %.2f\n", factura.total);
        }
    }
    if(hayfacturas){
        printf("No hay facturas leidas con exito\n");
    }
    fclose(f);
}

void updateFactura(){
    FILE *f;
    struct Factura factura;
    int hayfacturas = 0;
    int cedula;
    printf("Ingrese la cedula del cliente: ");
    scanf("%d", &cedula);
    f = fopen("facturas.dat", "rb+");
    if (f == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }
    while(fread(&factura, sizeof(struct Factura), 1, f)){
        if (factura.activo){
            hayfacturas = 1;
            printf("Ingrese el nuevo nombre del cliente: ");
            leerCadena(factura.nombre, 50);
            printf("Ingrese la nueva cedula del cliente: ");
            scanf("%d", &factura.cedula);
            printf("Ingrese el nuevo numero de productos: ");
            scanf("%d", &factura.Nproductos);
            factura.total = 0;
            for (int i = 0; i < factura.Nproductos; i++){
                printf("Ingrese el nombre del producto %d: ", i+1);
                leerCadena(factura.productos[i].nombre, 50);
                printf("Ingrese la cantidad del producto %d: ", i+1);
                scanf("%d", &factura.productos[i].cantidad);
                printf("Ingrese el precio del producto %d: ", i+1);
                scanf("%f", &factura.productos[i].precio);
                factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
            }
            fseek(f, -(long)sizeof(struct Factura), SEEK_CUR);
            fwrite(&factura, sizeof(struct Factura), 1, f);
            printf("Factura actualizada con exito\n");
            fclose(f);
            return;
        }
    }
    printf("Factura no encontrada\n");
    fclose(f);
}

void deleteFactura(){
    FILE *f;
    struct Factura factura;
    int cedula;
    int encontrado = 0;
    printf("Ingrese la cedula del cliente: ");
    scanf("%d", &cedula);
    f = fopen("facturas.dat", "rb+");
    if (f == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }

    while(fread(&factura, sizeof(struct Factura), 1, f)){
        if (factura.cedula == cedula && factura.activo != 0){
            encontrado = 1;
            factura.activo = 0; // Mark as deleted
            fseek(f, -(long)sizeof(struct Factura), SEEK_CUR);
            fwrite(&factura, sizeof(struct Factura), 1, f);
            printf("Factura eliminada con exito\n");
            fclose(f);
            return;
        }
    }
    int encontrada = 0;
    int posicion = 0;
    while(fread(&factura, sizeof(struct Factura), 1, f)){
        if (factura.cedula == cedula && factura.activo == 0){
            encontrada = 1;
            break;
        }
        posicion=ftell(f);
    }
    if (!encontrado) {
        printf("Factura no encontrada\n");
    }
    fclose(f);
}


