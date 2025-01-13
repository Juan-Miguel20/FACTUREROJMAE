#include <stdio.h>
#include "funciones.h" 
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int isNombreValido(char *nombre){
    for (int i = 0; i < strlen(nombre); i++){
        if (!isalpha(nombre[i]) && nombre[i] != ' '){
            return 0;
        }
    }
    return 1;
}

int isNumeroValido(char *numero){
    int punto = 0;
    for (int i = 0; i < strlen(numero); i++){
        if (numero[i] == '.'){
            if (punto) return 0;
            punto = 1;
        }else if (!isdigit(numero[i])){
             return 0;
        }
    }
    return 1;
}

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
    cadena[strcspn(cadena, "\n")] = 0;
}

void createFactura(){
    struct Factura factura;
    char buffer[50];

    printf("Ingrese el nombre del cliente: ");
    leerCadena(buffer, 50);
    while (!isNombreValido(buffer)) {
        printf("Nombre invalido. Ingrese solo letras: ");
        leerCadena(buffer, 50);
    }
    strcpy(factura.nombre, buffer);

    printf("Ingrese la cedula del cliente: ");
    leerCadena(buffer, 50);
    while (!isNumeroValido(buffer)) {
        printf("Cedula invalida. Ingrese solo numeros: ");
        leerCadena(buffer, 50);
    }
    factura.cedula = atoi(buffer);

    printf("Ingrese el numero de productos: ");
    leerCadena(buffer, 50);
    while (!isNumeroValido(buffer)) {
        printf("Numero de productos invalido. Ingrese solo numeros: ");
        leerCadena(buffer, 50);
    }
    factura.Nproductos = atoi(buffer);

    factura.total = 0;
    factura.activo = 1;
    for (int i = 0; i < factura.Nproductos; i++){
        printf("Ingrese el nombre del producto %d: ", i+1);
        leerCadena(buffer, 50);
        while (!isNombreValido(buffer)) {
            printf("Nombre del producto invalido. Ingrese solo letras: ");
            leerCadena(buffer, 50);
        }
        strcpy(factura.productos[i].nombre, buffer);

        printf("Ingrese la cantidad del producto %d: ", i+1);
        leerCadena(buffer, 50);
        while (!isNumeroValido(buffer)) {
            printf("Cantidad invalida. Ingrese solo numeros: ");
            leerCadena(buffer, 50);
        }
        factura.productos[i].cantidad = atoi(buffer);

        printf("Ingrese el precio del producto %d: ", i+1);
        leerCadena(buffer, 50);
        while (!isNumeroValido(buffer)) {
            printf("Precio invalido. Ingrese solo numeros: ");
            leerCadena(buffer, 50);
        }
        factura.productos[i].precio = atof(buffer);
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
        printf("Factura leida con exito\n");
    }
    fclose(f);
}

void updateFactura(){
    FILE *f;
    struct Factura factura;
    int hayfacturas = 0;
    char buffer[50];
    int cedula;

    f = fopen("facturas.dat", "rb+");
    if (f == NULL){
        printf("Error al abrir el archivo\n");
        return;
    }

    do {
        printf("Ingrese la cedula del cliente: ");
        leerCadena(buffer, 50);
        while (!isNumeroValido(buffer)) {
            printf("Cedula invalida. Ingrese solo numeros: ");
            leerCadena(buffer, 50);
        }
        cedula = strtol(buffer, NULL, 10);

        rewind(f); // Reiniciar el puntero del archivo para buscar desde el principio
        hayfacturas = 0;
        while(fread(&factura, sizeof(struct Factura), 1, f)){
            if (factura.cedula == cedula && factura.activo){
                hayfacturas = 1;
                break;
            }
        }

        if (!hayfacturas) {
            printf("Factura no encontrada. Vuelve a intentarlo.\n");
        }
    } while (!hayfacturas);

    printf("Ingrese el nuevo nombre del cliente: ");
    leerCadena(buffer, 50);
    while (!isNombreValido(buffer)) {
        printf("Nombre invalido. Ingrese solo letras: ");
        leerCadena(buffer, 50);
    }
    strcpy(factura.nombre, buffer);

    printf("Ingrese la nueva cedula del cliente: ");
    leerCadena(buffer, 50);
    while (!isNumeroValido(buffer)) {
        printf("Cedula invalida. Ingrese solo numeros: ");
        leerCadena(buffer, 50);
    }
    factura.cedula = strtol(buffer, NULL, 10);

    printf("Ingrese el nuevo numero de productos: ");
    leerCadena(buffer, 50);
    while (!isNumeroValido(buffer)) {
        printf("Numero de productos invalido. Ingrese solo numeros: ");
        leerCadena(buffer, 50);
    }
    factura.Nproductos = strtol(buffer, NULL, 10);

    factura.total = 0;
    for (int i = 0; i < factura.Nproductos; i++){
        printf("Ingrese el nombre del producto %d: ", i+1);
        leerCadena(buffer, 50);
        while (!isNombreValido(buffer)) {
            printf("Nombre del producto invalido. Ingrese solo letras: ");
            leerCadena(buffer, 50);
        }
        strcpy(factura.productos[i].nombre, buffer);

        printf("Ingrese la cantidad del producto %d: ", i+1);
        leerCadena(buffer, 50);
        while (!isNumeroValido(buffer)) {
            printf("Cantidad invalida. Ingrese solo numeros: ");
            leerCadena(buffer, 50);
        }
        factura.productos[i].cantidad = strtol(buffer, NULL, 10);

        printf("Ingrese el precio del producto %d: ", i+1);
        leerCadena(buffer, 50);
        while (!isNumeroValido(buffer)) {
            printf("Precio invalido. Ingrese solo numeros: ");
            leerCadena(buffer, 50);
        }
        factura.productos[i].precio = strtod(buffer, NULL);
        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }

    fseek(f, -(long)sizeof(struct Factura), SEEK_CUR);
    fwrite(&factura, sizeof(struct Factura), 1, f);
    printf("Factura actualizada con exito\n");
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

