#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "cJSON.h"

#define Cadena 100
typedef char String[Cadena];

int numAsignaciones = 0;
int numCursos = 0;
int numSalas = 0;



// Enumeracion de los bloques disponibles y sus horarios (Para facilidad de manejo de informacion)
enum Bloques
{
    Bloque1, // Bloque1(07:00 am - 09:00 am) = 0
    Bloque2, // Bloque2(09:00 am - 11:00 am) = 1
    Bloque3, // Bloque3(11:00 am - 01:00 am) = 2
    Bloque4, // Bloque4(01:00 pm - 03:00 pm) = 3
    Bloque5, // Bloque5(03:00 pm - 05:00 pm) = 4
    Bloque6, // Bloque6(05:00 pm - 07:00 pm) = 5
    Bloque7, // Bloque7(07:00 pm - 09:00 pm) = 6

};

// Enumeracion de los dias de la semana (Para facilidad de manejo de informacion)
enum Dias
{
    Lunes,     // Lunes = 0
    Martes,    // Martes = 1
    Miercoles, // Miercoles = 2
    Jueves,    // Jueves = 3
    Viernes,   // Viernes = 4
    Sabado,    // Sabado = 5

};

// Estructura de bloques (Acompaña a la estructura curso para facilidad de manejo de datos)
typedef struct
{

    int Dia;               // Dia que el profesor desea dar sus clases
    int CantidadHoras;     // Cantidad de horas totales del curso del profesor
    int BloquePreferencia; // Bloque horario en el que el profesor dara su clase

} bloqueDisponible;

// Estructura del curso (Informacion de la persona que reservara la sala)
typedef struct
{

    int ID;            // Cedula de profesor (Para identificar)
    String Nombre;     // Nombre de profesor
    String Asignatura; // Asignatura de profesor
    int TamañoCurso;   // Nuevo miembro para el tamaño de curso
    bloqueDisponible;  // Struct de bloqueDisponible
    int asignado;      // Identificador para no repetir ciclos (Posibilidad de eliminar esta variable)

} Curso;

// Estructura de salas (Toda la informacion de las salas que se reservaran)
typedef struct
{

    int ID;               // Identificador de la sala a la que se desea acceder
    String Nombre;        // Nombre de la sala
    int CapacidadAlumnos; // Capacidad de alumnos de la sala

} Sala;

// Estructura de asignacion de salas con respecto a profesores (Informacion de identificacion de la asignacion con ID de profesor e ID de sala y su bloque horario)
typedef struct
{

    int ID_profesor;    // Identificador de profesor asignado
    int ID_sala;        // Identificador de sala asignada
    int BloqueAsignado; // Bloque horario asignado a dicha sala y profesor
    int DiaAsignado;    // Dia asignado para el profesor y la sala

} Asignacion;

// Función para leer el archivo JSON de cursos y almacenarlos en una estructura de cursos
Curso *leerCursos(String nombre_json)
{
    // Abre el archivo JSON
    FILE *fp = fopen(nombre_json, "r");
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
        cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");
        cJSON *nombre = cJSON_GetObjectItemCaseSensitive(item, "nombre");
        cJSON *asignatura = cJSON_GetObjectItemCaseSensitive(item, "asignatura");
        cJSON *cant_estudiantes = cJSON_GetObjectItemCaseSensitive(item, "cant_estudiantes");
        cJSON *dia = cJSON_GetObjectItemCaseSensitive(item, "dia");
        cJSON *total_horas = cJSON_GetObjectItemCaseSensitive(item, "total_horas");
        cJSON *bloquepreferido = cJSON_GetObjectItemCaseSensitive(item, "bloquepreferido");
        cJSON *asignado = cJSON_GetObjectItemCaseSensitive(item, "asignado");

        if (cJSON_IsNumber(id) && cJSON_IsString(nombre) && cJSON_IsString(asignatura) &&
            cJSON_IsNumber(cant_estudiantes) && cJSON_IsNumber(dia) && cJSON_IsNumber(total_horas) && cJSON_IsNumber(bloquepreferido) && cJSON_IsNumber(asignado))
        {
            // Asigna los valores a la estructura de cursos
            cursos[i].ID = id->valueint;
            strcpy(cursos[i].Nombre, nombre->valuestring);
            strcpy(cursos[i].Asignatura, asignatura->valuestring);
            cursos[i].TamañoCurso = cant_estudiantes->valueint;
            cursos[i].Dia = dia->valueint;
            cursos[i].CantidadHoras = total_horas->valueint;
            cursos[i].BloquePreferencia = bloquepreferido->valueint;
            cursos[i].asignado = asignado->valueint;
            numCursos++;
        }
    }

    // Libera la memoria del objeto JSON
    cJSON_Delete(json);

    return cursos;
}

Sala *leerSalas(String nombre_json)
{
    // Abre el archivo JSON
    FILE *fp = fopen(nombre_json, "r");
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
        cJSON *id = cJSON_GetObjectItemCaseSensitive(item, "id");
        cJSON *nombre = cJSON_GetObjectItemCaseSensitive(item, "nombre");
        cJSON *cant_alumnos = cJSON_GetObjectItemCaseSensitive(item, "cant_alumnos");

        if (cJSON_IsNumber(id) && cJSON_IsString(nombre) && cJSON_IsNumber(cant_alumnos))
        {
            // Asigna los valores a la estructura de salas
            salas[numSalas].ID = id->valueint;
            strcpy(salas[numSalas].Nombre, nombre->valuestring);
            salas[numSalas].CapacidadAlumnos = cant_alumnos->valueint;
            numSalas++;
        }
    }

    // Libera la memoria del objeto JSON
    cJSON_Delete(json);
    return salas;
}

// Asignacion de salas a profesores y guardado en arreglo
Asignacion *asignamiento_salas()
{
    for (int i = 0; i < numCursos; i++)
    {
        for (int j = 0; j < numSalas; j++)
        {
            bool existe = false;

            if (cursos[i].TamañoCurso <= salas[j].CapacidadAlumnos && cursos[i].asignado == 0)
            {
                for (int k = 0; k <= numAsignaciones; k++)
                {
                    if (cursos[i].Dia == asignaciones[k].DiaAsignado && salas[j].ID == asignaciones[k].ID_sala)
                    {
                        if (cursos[i].BloquePreferencia == asignaciones[k].BloqueAsignado)
                        {
                            existe = true;
                            break;
                        }
                    }
                }

                if (!existe)
                {
                    asignaciones[numAsignaciones].ID_profesor = cursos[i].ID;
                    asignaciones[numAsignaciones].ID_sala = salas[j].ID;
                    asignaciones[numAsignaciones].DiaAsignado = cursos[i].Dia;
                    asignaciones[numAsignaciones].BloqueAsignado = cursos[i].BloquePreferencia;
                    cursos[i].asignado = 1;
                    numAsignaciones++;
                    asignaciones = (Asignacion *)realloc(asignaciones, (numAsignaciones + 1) * sizeof(Asignacion));
                }
            }
        }
    }

    return asignaciones;
}


Asignacion *asignaciones = NULL;
Curso *cursos = NULL;
Sala *salas = NULL;

int libAsignacion_Salas()
{

    // Reserva de memoria dinámica para cursos y salas
    cursos = (Curso *)malloc(sizeof(Curso));
    salas = (Sala *)malloc(sizeof(Sala));
    asignaciones = (Asignacion *)malloc(sizeof(Asignacion));

    String nombre_json;
    printf("Nombre del JSON de cursos:");
    scanf(" %s", nombre_json);
    cursos = leerCursos(nombre_json);
    printf("Nombre del JSON de salas:");
    scanf(" %s", nombre_json);
    salas = leerSalas(nombre_json);
    asignaciones = asignamiento_salas();

    printf("Cursos almacenados:\n");
    for (int i = 0; i < numCursos; i++)
    {
        printf("Curso %d: %s\n", i + 1, cursos[i].Nombre);
    }

    printf("\nSalas almacenadas:\n");
    for (int i = 0; i < numSalas; i++)
    {
        printf("Sala %d: %d\n", i + 1, salas[i].ID);
    }

    printf("\nAsignaciones almacenadas:\n");
    for (int i = 0; i < numAsignaciones; i++)
    {
        printf("ID_Profesor: %d  || ID_Sala: %d  || Dia_Asignado: %d  || Bloque_Horario: %d\n", asignaciones[i].ID_profesor, asignaciones[i].ID_sala, asignaciones[i].DiaAsignado, asignaciones[i].BloqueAsignado);
    }

    // Liberar memoria cuando ya no se necesite
    free(cursos);
    free(salas);
    free(asignaciones);

    return 0;
}
