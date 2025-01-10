struct Producto
{
    char nombre[50];
    int cantidad;
    float precio;
};

struct Factura
{
    char nombre[50];
    int cedula;
    int Nproductos;
    int activo;
    struct Producto productos[5];
    float total;
};