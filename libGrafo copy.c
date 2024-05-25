#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define cant_bloques 6
#define dias 5
#define Cadena 100
typedef char String[Cadena];
typedef int DispBloques[cant_bloques];
typedef int DiaDisponible[dias];

// Enumeracion de los bloques disponibles y sus horarios (Para facilidad de manejo de informacion)

/*
 * Tener encuenta que si el profesor no elige el bloque #1 puede ocurrir error ya que el array inicia lleno de 0.*****************
 **/
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

    DiaDisponible Dia;             // Dia que el profesor desea dar sus clases
    int CantidadHoras;             // Cantidad de horas totales del curso del profesor
    DispBloques BloquePreferencia; // Array de bloque horario en el que el profesor desea dar su clase [0, 1, 2, 3, 4, 5, 6]

} bloqueDisponible;

// Estructura del curso (Informacion de la persona que reservara la sala)
typedef struct
{

    int ID;            // Cedula de profesor (Para identificar)
    String Nombre;     // Nombre de profesor
    String Asignatura; // Asignatura de profesor
    int TamañoCurso;   // Nuevo miembro para el tamaño de curso
    bloqueDisponible;  // Struct de bloqueDisponible
    bool asignado;

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

int main()
{
    Asignacion *asignaciones = NULL;
    Curso *cursos = NULL;
    Sala *salas = NULL;

    int numAsignaciones = 0;
    int numCursos = 0;
    int numSalas = 0;

    // Reserva de memoria dinámica para cursos y salas
    cursos = (Curso *)malloc(sizeof(Curso));
    salas = (Sala *)malloc(sizeof(Sala));
    asignaciones = (Asignacion *)malloc(sizeof(Asignacion));

    // Inicializacion de las struct
    // Llenado de Curso
    cursos[numCursos].ID = 1006428216;
    strcpy(cursos[numCursos].Nombre, "Santiago Diaz Franki");
    strcpy(cursos[numCursos].Asignatura, "Matemáticas");
    cursos[numCursos].TamañoCurso = 30;
    cursos[numCursos].Dia[0] = Lunes;
    cursos[numCursos].CantidadHoras = 4;
    cursos[numCursos].BloquePreferencia[0] = Bloque1;
    cursos[numCursos].BloquePreferencia[1] = Bloque2;
    cursos[numCursos].asignado = false;
    numCursos++;

    // Redimensionar el arreglo de cursos
    cursos = (Curso *)realloc(cursos, (numCursos + 1) * sizeof(Curso));

    cursos[numCursos].ID = 1006428217;
    strcpy(cursos[numCursos].Nombre, "Juan Perez");
    strcpy(cursos[numCursos].Asignatura, "Fisica");
    cursos[numCursos].TamañoCurso = 25;
    cursos[numCursos].Dia[0] = Lunes;
    cursos[numCursos].CantidadHoras = 2;
    cursos[numCursos].BloquePreferencia[0] = Bloque2;
    cursos[numCursos].BloquePreferencia[1] = Bloque3;
    cursos[numCursos].asignado = false;
    numCursos++;

    // Redimensionar el arreglo de cursos
    cursos = (Curso *)realloc(cursos, (numCursos + 1) * sizeof(Curso));

    cursos[numCursos].ID = 1006428218;
    strcpy(cursos[numCursos].Nombre, "Arnulfo Castrillo");
    strcpy(cursos[numCursos].Asignatura, "Programacion");
    cursos[numCursos].TamañoCurso = 27;
    cursos[numCursos].Dia[0] = Lunes;
    cursos[numCursos].CantidadHoras = 2;
    cursos[numCursos].BloquePreferencia[0] = Bloque1;
    cursos[numCursos].BloquePreferencia[1] = Bloque2;
    cursos[numCursos].asignado = false;
    numCursos++;

    // Redimensionar el arreglo de cursos
    cursos = (Curso *)realloc(cursos, (numCursos + 1) * sizeof(Curso));

    cursos[numCursos].ID = 1006428219;
    strcpy(cursos[numCursos].Nombre, "Juan Carlos Guitierrez");
    strcpy(cursos[numCursos].Asignatura, "Analisis Numerico");
    cursos[numCursos].TamañoCurso = 30;
    cursos[numCursos].Dia[0] = Lunes;
    cursos[numCursos].CantidadHoras = 2;
    cursos[numCursos].BloquePreferencia[0] = Bloque1;
    cursos[numCursos].BloquePreferencia[1] = Bloque4;
    cursos[numCursos].BloquePreferencia[2] = Bloque5;
    cursos[numCursos].asignado = false;
    numCursos++;

    // Redimensionar el arreglo de cursos
    cursos = (Curso *)realloc(cursos, (numCursos + 1) * sizeof(Curso));

    // Llenado de Salas
    salas[numSalas].ID = 101;
    strcpy(salas[numSalas].Nombre, "Sala A");
    salas[numSalas].CapacidadAlumnos = 30;
    numSalas++;

    // Redimensionar el arreglo de salas
    salas = (Sala *)realloc(salas, (numSalas + 1) * sizeof(Sala));

    salas[numSalas].ID = 102;
    strcpy(salas[numSalas].Nombre, "Sala B");
    salas[numSalas].CapacidadAlumnos = 25;
    numSalas++;

    // Redimensionar el arreglo de salas
    salas = (Sala *)realloc(salas, (numSalas + 1) * sizeof(Sala));

    salas[numSalas].ID = 103;
    strcpy(salas[numSalas].Nombre, "Sala C");
    salas[numSalas].CapacidadAlumnos = 30;
    numSalas++;

    // Redimensionar el arreglo de salas
    salas = (Sala *)realloc(salas, (numSalas + 1) * sizeof(Sala));

    // Asignacion de salas a profesores y guardado en arreglo
    for (int i = 0; i < numCursos; i++)
    {
        for (int j = 0; j < numSalas; j++)
        {
            if (cursos[i].TamañoCurso <= salas[j].CapacidadAlumnos && !cursos[i].asignado)
            {
                for (int k = 0; k < 5; k++)
                {
                }
            }
        }
    }

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
