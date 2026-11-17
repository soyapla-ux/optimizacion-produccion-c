/*******************************************************************************
 * UDLA
 * Proyecto: Optimizacion de Produccion en una Fabrica de Componentes Electronicos
 * 
 * INTEGRANTES
            IAN RICARDO CADENA
            UYAGUARI MACAS LEONOR
            DOMINIC GARZÓN
            MELANY CUEVA
            PABLO SUÁREZ

 * Descripcion: Sistema modular con memoria dinamica, punteros, validaciones
 *              de entradas (sin numeros negativos), busqueda manual, edicion,
 *              eliminacion y analisis de viabilidad con recomendaciones.
 *
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

// Funcion para comparar dos cadenas caracter por caracter 
// Retorna 1 si son iguales, 0 si son diferentes.
int compararCadenas(char *str1, char *str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    if (str1[i] == '\0' && str2[i] == '\0') {
        return 1;
    }
    return 0;
}

// Funcion para copiar una cadena de origen a destino manualmente 
void copiarCadena(char *destino, char *origen) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}

// Funcion para limpiar el bufer de entrada y evitar saltos de linea residuales
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funcion para leer una cadena de texto limpia desde teclado
void leerTexto(char *cadena, int tamano) {
    fgets(cadena, tamano, stdin);
    int i = 0;
    while (cadena[i] != '\0') {
        if (cadena[i] == '\n') {
            cadena[i] = '\0';
            break;
        }
        i++;
    }
}

int main() {
    int capacidad = 6; // Iniciamos con los 6 productos solicitados
    int i;

    // Declaracion de punteros para arreglos paralelos con memoria dinamica
    char **nombres = (char **) malloc(capacidad * sizeof(char *));
    float *tiempos = (float *) malloc(capacidad * sizeof(float));
    float *recursos = (float *) malloc(capacidad * sizeof(float));
    int *demandas = (int *) malloc(capacidad * sizeof(int));

    // Validar si la memoria inicial se asigno correctamente
    if (nombres == NULL || tiempos == NULL || recursos == NULL || demandas == NULL) {
        printf("Error critico: No se pudo reservar memoria inicial.\n");
        return 1;
    }

    // Reservar memoria interna para los nombres de los 6 productos
    for (i = 0; i < capacidad; i++) {
        nombres[i] = (char *) malloc(50 * sizeof(char));
        if (nombres[i] == NULL) {
            printf("Error critico: No se pudo reservar memoria para nombres.\n");
            return 1;
        }
    }

    // Nombres base sugeridos
    char *nombresBase[6] = {
        "Memoria RAM",
        "Discos Solidos",
        "Tarjetas Madre",
        "Teclados",
        "Placa Base",
        "Tarjetas de Video"
    };

    printf("====================================================\n");
    printf("   SISTEMA DE OPTIMIZACION DE FABRICA ELECTRONICA   \n");
    printf("====================================================\n\n");

    // 1. Ingreso de datos iniciales para los 6 productos con validacion (Sin negativos)
    printf("--- REGISTRO DE DATOS PARA LOS 6 PRODUCTOS ---\n");
    for (i = 0; i < capacidad; i++) {
        copiarCadena(nombres[i], nombresBase[i]);

        printf("\nProducto [%d]: %s\n", i + 1, nombres[i]);

        // Validacion: Tiempo de fabricacion (No negativo)
        do {
            printf("  Ingrese tiempo de fabricacion por unidad (horas): ");
            scanf("%f", &tiempos[i]);
            if (tiempos[i] < 0) {
                printf("  [Error] El tiempo no puede ser negativo. Intente de nuevo.\n");
            }
        } while (tiempos[i] < 0);

        // Validacion: Recursos necesarios (No negativo)
        do {
            printf("  Ingrese recursos necesarios por unidad (unidades): ");
            scanf("%f", &recursos[i]);
            if (recursos[i] < 0) {
                printf("  [Error] Los recursos no pueden ser negativos. Intente de nuevo.\n");
            }
        } while (recursos[i] < 0);

        // Validacion: Demanda solicitada por el cliente (No negativo)
        do {
            printf("  Ingrese cantidad demandada por el cliente: ");
            scanf("%d", &demandas[i]);
            if (demandas[i] < 0) {
                printf("  [Error] La demanda no puede ser negativa. Intente de nuevo.\n");
            }
        } while (demandas[i] < 0);
    }

    // 2. Definir limites actuales de la planta
    float tiempoDisponiblePlanta = 0;
    float recursosDisponiblesPlanta = 0;

    printf("\n--- CAPACIDAD ACTUAL DE LA PLANTA ---\n");
    do {
        printf("Ingrese el Tiempo Total disponible en la planta (horas): ");
        scanf("%f", &tiempoDisponiblePlanta);
        if (tiempoDisponiblePlanta < 0) {
            printf("[Error] El tiempo no puede ser negativo.\n");
        }
    } while (tiempoDisponiblePlanta < 0);

    do {
        printf("Ingrese los Recursos Totales disponibles en la planta: ");
        scanf("%f", &recursosDisponiblesPlanta);
        if (recursosDisponiblesPlanta < 0) {
            printf("[Error] Los recursos no pueden ser negativos.\n");
        }
    } while (recursosDisponiblesPlanta < 0);

    int opcion = 0;
    char buscarNombre[50];

    // 3. Menu interactivo de gestion
    do {
        printf("\n================ MENU PRINCIPAL ================\n");
        printf("1. Ver catalogo y demanda de productos\n");
        printf("2. Buscar y editar un producto\n");
        printf("3. Buscar y eliminar un producto\n");
        printf("4. Calcular viabilidad de produccion y recomendaciones\n");
        printf("5. Salir del sistema\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        limpiarBuffer();

        switch (opcion) {
            case 1:
                if (capacidad == 0) {
                    printf("\nNo hay productos registrados en el sistema.\n");
                } else {
                    printf("\n--- CATALOGO DE PRODUCTOS Y REQUERIMIENTOS ---\n");
                    printf("%-20s | %-10s | %-10s | %-10s\n", "Producto", "Tiempo/U", "Recursos/U", "Demanda");
                    printf("------------------------------------------------------------\n");
                    for (i = 0; i < capacidad; i++) {
                        printf("%-20s | %-10.2f | %-10.2f | %-10d\n", nombres[i], tiempos[i], recursos[i], demandas[i]);
                    }
                }
                break;

            case 2: {
                // Busqueda y edicion manual de nombre y datos
                if (capacidad == 0) {
                    printf("\nNo hay productos disponibles para editar.\n");
                    break;
                }

                printf("\nIngrese el nombre exacto del producto a buscar y editar: ");
                leerTexto(buscarNombre, sizeof(buscarNombre));

                int encontrado = -1;
                for (i = 0; i < capacidad; i++) {
                    if (compararCadenas(nombres[i], buscarNombre)) {
                        encontrado = i;
                        break;
                    }
                }

                if (encontrado != -1) {
                    printf("Producto encontrado: [%s]\n", nombres[encontrado]);

                    printf("Ingrese el nuevo nombre del producto: ");
                    char nuevoNombre[50];
                    leerTexto(nuevoNombre, sizeof(nuevoNombre));
                    copiarCadena(nombres[encontrado], nuevoNombre);

                    do {
                        printf("Nuevo tiempo por unidad (horas): ");
                        scanf("%f", &tiempos[encontrado]);
                        if (tiempos[encontrado] < 0) printf("[Error] No puede ser negativo.\n");
                    } while (tiempos[encontrado] < 0);

                    do {
                        printf("Nuevos recursos por unidad: ");
                        scanf("%f", &recursos[encontrado]);
                        if (recursos[encontrado] < 0) printf("[Error] No puede ser negativo.\n");
                    } while (recursos[encontrado] < 0);

                    do {
                        printf("Nueva cantidad demandada: ");
                        scanf("%d", &demandas[encontrado]);
                        if (demandas[encontrado] < 0) printf("[Error] No puede ser negativo.\n");
                    } while (demandas[encontrado] < 0);

                    printf("Producto actualizado exitosamente.\n");
                } else {
                    printf("Producto no encontrado en el sistema.\n");
                }
                break;
            }

            case 3: {
                // Busqueda y eliminacion con desplazamiento y realloc
                if (capacidad == 0) {
                    printf("\nNo hay productos disponibles para eliminar.\n");
                    break;
                }

                printf("\nIngrese el nombre exacto del producto a eliminar: ");
                leerTexto(buscarNombre, sizeof(buscarNombre));

                int encontrado = -1;
                for (i = 0; i < capacidad; i++) {
                    if (compararCadenas(nombres[i], buscarNombre)) {
                        encontrado = i;
                        break;
                    }
                }

                if (encontrado != -1) {
                    // Liberar la memoria del nombre del producto eliminado
                    free(nombres[encontrado]);

                    // Desplazar los elementos posteriores hacia la izquierda
                    for (i = encontrado; i < capacidad - 1; i++) {
                        nombres[i] = nombres[i + 1];
                        tiempos[i] = tiempos[i + 1];
                        recursos[i] = recursos[i + 1];
                        demandas[i] = demandas[i + 1];
                    }

                    capacidad--;

                    // Redimensionar arreglos dinamicos si aun quedan productos
                    if (capacidad > 0) {
                        nombres = (char **) realloc(nombres, capacidad * sizeof(char *));
                        tiempos = (float *) realloc(tiempos, capacidad * sizeof(float));
                        recursos = (float *) realloc(recursos, capacidad * sizeof(float));
                        demandas = (int *) realloc(demandas, capacidad * sizeof(int));
                    } else {
                        free(nombres);
                        free(tiempos);
                        free(recursos);
                        free(demandas);
                        nombres = NULL;
                        tiempos = NULL;
                        recursos = NULL;
                        demandas = NULL;
                    }

                    printf("Producto eliminado correctamente del sistema.\n");
                } else {
                    printf("Producto no encontrado en el sistema.\n");
                }
                break;
            }

            case 4: {
                // Analisis de Viabilidad y Valor Agregado
                if (capacidad == 0) {
                    printf("\nNo hay productos registrados para calcular la produccion.\n");
                    break;
                }

                float tiempoTotalReq = 0;
                float recursosTotalReq = 0;

                for (i = 0; i < capacidad; i++) {
                    tiempoTotalReq += (tiempos[i] * demandas[i]);
                    recursosTotalReq += (recursos[i] * demandas[i]);
                }

                printf("\n================ REPORTE DE VIABILIDAD ================\n");
                printf("Tiempo total requerido para el pedido : %.2f horas\n", tiempoTotalReq);
                printf("Tiempo disponible en la planta        : %.2f horas\n", tiempoDisponiblePlanta);
                printf("-------------------------------------------------------\n");
                printf("Recursos totales requeridos           : %.2f unidades\n", recursosTotalReq);
                printf("Recursos disponibles en la planta     : %.2f unidades\n", recursosDisponiblesPlanta);
                printf("=======================================================\n");

                int esViable = 1;

                if (tiempoTotalReq > tiempoDisponiblePlanta) {
                    printf("\n[ALERTA DE LIMITE DE TIEMPO]\n");
                    printf("-> El pedido excede el tiempo disponible por %.2f horas.\n", (tiempoTotalReq - tiempoDisponiblePlanta));
                    esViable = 0;
                }

                if (recursosTotalReq > recursosDisponiblesPlanta) {
                    printf("\n[ALERTA DE LIMITE DE RECURSOS]\n");
                    printf("-> Faltan %.2f unidades de recursos para completar el pedido.\n", (recursosTotalReq - recursosDisponiblesPlanta));
                    esViable = 0;
                }

                if (esViable) {
                    printf("\nRESULTADO: PEDIDO 100%% VIABLE.\n");
                    printf("La fabrica cuenta con tiempo y recursos suficientes para cubrir el lote completo.\n");
                } else {
                    printf("\nRESULTADO: PEDIDO NO VIABLE EN LAS CONDICIONES ACTUALES.\n");
                    printf("\n[RECOMENDACION DE PRODUCCION - VALOR AGREGADO]:\n");
                    if (tiempoTotalReq > tiempoDisponiblePlanta) {
                        float horasExtra = tiempoTotalReq - tiempoDisponiblePlanta;
                        printf("- Ajuste de Tiempo: Se requiere ampliar el plazo de entrega o habilitar turnos extra por %.2f horas (aprox. %.1f dias adicionales de 8h) para cumplir con el pedido.\n", horasExtra, horasExtra / 8.0);
                    }
                    if (recursosTotalReq > recursosDisponiblesPlanta) {
                        float deficitRecursos = recursosTotalReq - recursosDisponiblesPlanta;
                        printf("- Ajuste de Recursos: Se debe abastecer la planta con %.2f unidades adicionales de recursos o dividir el pedido en entregas parciales.\n", deficitRecursos);
                    }
                }
                break;
            }

            case 5:
                printf("Saliendo del sistema de optimizacion...\n");
                break;

            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }

    } while (opcion != 5);

    // Liberar memoria dinamica asignada antes de cerrar el programa
    if (capacidad > 0) {
        for (i = 0; i < capacidad; i++) {
            free(nombres[i]);
        }
        free(nombres);
        free(tiempos);
        free(recursos);
        free(demandas);
    }

    return 0;
}