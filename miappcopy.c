#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "cJSON.h"

// Define el número máximo de caracteres para las cadenas de texto
#define MAXCHAR 100

int numCursos = 0;
int numSalas = 0;
int numAsignaciones = 0;

typedef char Cadena[MAXCHAR];

enum bloque
{
    Bloque1, // Bloque1(07:00 am - 09:00 am) = 0
    Bloque2, // Bloque2(09:00 am - 11:00 am) = 1
    Bloque3, // Bloque3(11:00 am - 01:00 am) = 2
    Bloque4, // Bloque4(02:00 pm - 04:00 pm) = 3
    Bloque5, // Bloque5(04:00 pm - 06:00 pm) = 4
    Bloque6, // Bloque6(06:00 pm - 08:00 pm) = 5
    Bloque7, // Bloque7(08:00 pm - 10:00 pm) = 6
};

// Enumeración de los días de la semana
enum dia
{
    Lunes,     // Lunes = 0
    Martes,    // Martes = 1
    Miercoles, // Miercoles = 2
    Jueves,    // Jueves = 3
    Viernes,   // Viernes = 4
    Sabado,    // Sabado = 5
};

// Estructura para representar un curso
typedef struct
{
    Cadena nombre;
    int semestre;
    int num_estudiantes;
    Cadena profesor;
    int total_horas;
    int dia;
    int bloque; // Horarios disponibles
} Curso;

// Estructura para representar una  (sala)
typedef struct
{
    Cadena nombre;
    int capacidad;
    // Horarios disponibles
} Sala;

typedef struct
{
    Cadena nombre_profesor;
    Cadena nombre_sala;
    int dia_asignado;
    int bloque_asignado; // Bloques horarios asignados
} Asignamiento;

// Función para leer el archivo JSON de cursos y almacenarlos en una estructura de cursos
Curso *readJsonCursos()
{
    // Abre el archivo JSON
    FILE *fp = fopen("cursos.json", "r");
    if (fp == NULL)
    {
        printf("Error: Unable to open the file.\n");
        return NULL;
    }

    // Lee el contenido del archivo en una cadena
    char buffer[1024];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    // Analiza los datos JSON
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL || !cJSON_IsArray(json))
    {
        printf("Error: Unable to parse JSON array.\n");
        cJSON_Delete(json);
        return NULL;
    }

    // Obtén el tamaño del array
    int arraySize = cJSON_GetArraySize(json);
    Curso *cursos = (Curso *)malloc(arraySize * sizeof(Curso));
    if (cursos == NULL)
    {
        printf("Error: Unable to allocate memory for cursos.\n");
        cJSON_Delete(json);
        return NULL;
    }

    for (int i = 0; i < arraySize; i++)
    {
        cJSON *item = cJSON_GetArrayItem(json, i);
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *semestre = cJSON_GetObjectItemCaseSensitive(item, "semestre");
        cJSON *num_estudiantes = cJSON_GetObjectItemCaseSensitive(item, "num_estudiantes");
        cJSON *profesor = cJSON_GetObjectItemCaseSensitive(item, "profesor");
        cJSON *dia = cJSON_GetObjectItemCaseSensitive(item, "diapreferido");
        cJSON *bloque = cJSON_GetObjectItemCaseSensitive(item, "bloquepreferido");
        cJSON *total_horas = cJSON_GetObjectItemCaseSensitive(item, "total_horas");

        if (cJSON_IsString(name) && cJSON_IsNumber(semestre) && cJSON_IsNumber(num_estudiantes) &&
            cJSON_IsString(profesor) && cJSON_IsNumber(dia) && cJSON_IsNumber(bloque) && cJSON_IsNumber(total_horas))
        {
            // Asigna los valores a la estructura de cursos
            strcpy(cursos[i].nombre, name->valuestring);
            cursos[i].semestre = semestre->valueint;
            cursos[i].num_estudiantes = num_estudiantes->valueint;
            strcpy(cursos[i].profesor, profesor->valuestring);
            cursos[i].dia = dia->valueint;
            cursos[i].bloque = bloque->valueint;
            cursos[i].total_horas = total_horas->valueint;
            numCursos++;
        }
    }

    // Libera la memoria del objeto JSON
    cJSON_Delete(json);

    return cursos;
}

Sala *readJsonSalas()
{
    // Abre el archivo JSON
    FILE *fp = fopen("salas.json", "r");
    if (fp == NULL)
    {
        printf("Error: No se puede abrir el archivo de salas.\n");
        return NULL;
    }

    // Lee el contenido del archivo en una cadena
    char buffer[1024];
    int len = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    // Analiza los datos JSON
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL || !cJSON_IsArray(json))
    {
        printf("Error: No se puede analizar el JSON de salas.\n");
        cJSON_Delete(json);
        return NULL;
    }

    // Obtén el tamaño del array
    int arraySize = cJSON_GetArraySize(json);

    Sala *salas = (Sala *)malloc(arraySize * sizeof(Sala));
    for (int i = 0; i < arraySize; i++)
    {
        cJSON *item = cJSON_GetArrayItem(json, i);
        cJSON *nombre = cJSON_GetObjectItemCaseSensitive(item, "nombre");
        cJSON *capacidad = cJSON_GetObjectItemCaseSensitive(item, "capacidad");

        if (cJSON_IsString(nombre) && cJSON_IsNumber(capacidad))
        {
            // Asigna los valores a la estructura de salas
            strcpy(salas[i].nombre, nombre->valuestring);
            salas[i].capacidad = capacidad->valueint;
            numSalas++;
        }
    }

    // Libera la memoria del objeto JSON
    cJSON_Delete(json);
    return salas;
}

// Función para asignar cursos a salas
Asignamiento *asignarCursosASalas(Curso *cursos, Sala *salas, int numCursos, int numSalas, int *numAsignaciones)
{
    Asignamiento *asignaciones = NULL;
    asignaciones = (Asignamiento *)malloc(sizeof(Asignamiento));

    for (int j = 0; j < numCursos; j++)
    {
        for (int i = 0; i < numSalas; i++)
        {
            if (salas[i].capacidad >= cursos[j].num_estudiantes)
            {
                bool horarioDisponible = true; // Suponemos que el horario es válido

                for (int k = 0; k < *numAsignaciones; k++)
                {
                    if (asignaciones[k].dia_asignado == cursos[j].dia &&
                        asignaciones[k].bloque_asignado == cursos[j].bloque)
                    {
                        horarioDisponible = false; // Horario ocupado
                        break;
                    }
                }

                if (horarioDisponible)
                {
                    // Asignar el curso si todos los horarios son válidos
                    strcpy(asignaciones[*numAsignaciones].nombre_profesor, cursos[j].profesor);
                    strcpy(asignaciones[*numAsignaciones].nombre_sala, salas[i].nombre);
                    asignaciones[*numAsignaciones].dia_asignado = cursos[j].dia;
                    asignaciones[*numAsignaciones].bloque_asignado = cursos[j].bloque;
                    (*numAsignaciones)++;
                    asignaciones = (Asignamiento *)realloc(asignaciones, (*numAsignaciones + 1) * sizeof(Asignamiento));
                }
            }
        }
    }
    return asignaciones;
}

void mostrarAsignaciones(Asignamiento *asignaciones, int numAsignaciones)
{
    printf("Asignaciones almacenadas:\n");
    for (int i = 0; i < numAsignaciones; i++)
    {
        printf("Profesor: %s   Sala: %s   Dia_Asignado: %d  || Bloque_Horario: %d\n",
               asignaciones[i].nombre_profesor, asignaciones[i].nombre_sala,
               asignaciones[i].dia_asignado, asignaciones[i].bloque_asignado);
    }
}

void writeJsonAsignaciones(Asignamiento *assignments, int numAssignments)
{
    // Crear un objeto JSON de tipo array
    cJSON *root = cJSON_CreateArray();

    for (int i = 0; i < numAssignments; i++)
    {
        // Crear un objeto JSON para cada asignación
        cJSON *assignment = cJSON_CreateObject();

        cJSON_AddStringToObject(assignment, "nombre_profesor", assignments[i].nombre_profesor);
        cJSON_AddStringToObject(assignment, "nombre_sala", assignments[i].nombre_sala);
        cJSON_AddNumberToObject(assignment, "dia_asignado", assignments[i].dia_asignado);
        cJSON_AddNumberToObject(assignment, "bloque_asignado", assignments[i].bloque_asignado);

        // Agregar el objeto de asignación al array
        cJSON_AddItemToArray(root, assignment);
    }

    // Convertir el objeto JSON en una cadena formateada
    char *jsonString = cJSON_Print(root);

    // Escribir la cadena JSON en un archivo
    FILE *fp = fopen("asignaciones.json", "w");
    if (fp != NULL)
    {
        fprintf(fp, "%s", jsonString);
        fclose(fp);
        printf("Asignaciones guardadas en 'asignaciones.json'\n");
    }
    else
    {
        printf("Error: No se pudo abrir el archivo 'asignaciones.json' para escritura.\n");
    }

    // Liberar la memoria del objeto JSON y la cadena JSON
    cJSON_Delete(root);
    free(jsonString);
}

int main()
{
    // Otras partes de tu código...

    // Creación de instancias de cursos y salas (datos de ejemplo)
    Curso *cursos = NULL;
    Sala *salas = NULL;
    Asignamiento *asignaciones = NULL;

    // Reserva de memoria dinámica para cursos y salas
    cursos = (Curso *)malloc(sizeof(Curso));
    salas = (Sala *)malloc(sizeof(Sala));
    asignaciones = (Asignamiento *)malloc(sizeof(Asignamiento));

    // Inicialización de datos de ejemplo
    cursos = readJsonCursos();
    salas = readJsonSalas();

    // Llama a la función para asignar cursos a salas
    asignaciones = asignarCursosASalas(cursos, salas, numCursos, numSalas, &numAsignaciones);

    // Llama a la función para mostrar las asignaciones después de asignar los cursos
    // mostrarAsignaciones(asignaciones, numAsignaciones);
    writeJsonAsignaciones(asignaciones, numAsignaciones);

    // Liberar memoria cuando ya no se necesite
    free(cursos);
    free(salas);
    free(asignaciones);

    return 0;
}
