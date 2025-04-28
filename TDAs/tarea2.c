#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/* Aqui definimos la estructura de una canción. Cada canción tiene un ID, un título, una lista de artistas, un álbum, un género y un tempo. La lista de artistas es una lista enlazada que permite almacenar múltiples canciones para un mismo artista. */


typedef struct {
  int id;
  char title[100];
  List *artists;
  char album[100];
  char genre[100];
  float tempo;
} Song;

/*
  Aquí definimos la estructura de información del artista. Cada artista tiene un nombre y el número de canciones asociadas a él. Esta estructura nos ayudará a organizar y mostrar la información de los artistas de manera eficiente.  
*/

typedef struct {
  char *artista;
  int num_canciones;
} ArtistaInfo;

/*
  Aquí definimos la estructura de información del género. Cada género tiene un nombre y el número de canciones asociadas a él. Esta estructura nos ayudará a organizar y mostrar la información de los géneros de manera eficiente.
*/
typedef struct {
  char *genero;
  int num_canciones;
} GeneroInfo;

// Ahora vamos a crear una función que nos ayudará a limpiar el buffer de entrada del teclado. Esto es útil para evitar que caracteres sobrantes, como un salto de línea, interfieran con la lectura de datos posteriores.

void limpiar_buffer() {
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF) { }
}

// Con la suiguiente funcion vamos a leer una cadena de texto del usuario con validación. Esta función se asegura de que la entrada no esté vacía y nos perminitra cancelar la operacion con 'q'. Luego la vamos a usar para leer los nombres de artistas, géneros o listas de reproducción.


int leer_cadena(char *buffer, size_t size, const char *prompt, int max_intentos) {
  int intentos = 0; // Aqui inicializamos el contador de intentos fallidos
  
  while (intentos < max_intentos) { // Mientras no se superen los intentos máximos
    printf("%s", prompt); // mostraremos el mensaje de solicitud al usuario

    if (!fgets(buffer, size, stdin)) { // luego leeremos la entrada del usuario
      printf("Error al leer la entrada.\n"); // Si falla fgets, mostraremos el error
      intentos++; // A media que el usuario se equivoque al ingresar sus datos esto aumentara
      continue; // y si se equivoca volveremos a pedir la entrada hasta que se cumpla la condicion
    }

    buffer[strcspn(buffer, "\n")] = '\0'; // Aqui vamos a eliminar el salto de línea '\n' al final de la entrada

    if (buffer[0] == 'q' || buffer[0] == 'Q') return 0; // Si el primer carácter es 'q' o 'Q', cancelamos la operación

    // Esta condicion no permtira validar que la cadena no esté vacía o formada solo por espacios/tabs
    if (buffer[0] != '\0' && strspn(buffer, " \t\n") != strlen(buffer)) {
      return 1; // Si es válida (no vacía ni solo espacios), retornar 1 que signifca que funciono
    }

    printf("Error: Ingrese una cadena no vacía (o 'q' para cancelar).\n");
    intentos++; //Si el usuario sigue ingresando datos erroneos incrementamos el número de intentos fallidos.
  }

  printf("Demasiados intentos fallidos. Regresando al menú.\n"); // al exceder los intentos le damos un mensaje que supero los intentos permitidos 
  return 0; // Retornar 0 indicando que no se obtuvo una entrada válida y volemos al menu.
}

int leer_numero(int *numero, const char *prompt, int max_intentos) {
  char buffer[100]; // Aqui vamos a crear un buffer(un espacio de memoria) para almacenar la entrada del usuario
  int intentos = 0; // Al igual que en el caso anterior crearemos un contador de intentos fallidos

  while (intentos < max_intentos) { // Mientras no superemos los intentos máximos
    printf("%s", prompt); // Mostraremos el mensaje de solicitud de entrada al usuario

    if (!fgets(buffer, sizeof(buffer), stdin)) { // Leeremos la entrada con fgets ya que nos da mas control sobre la misma a diferencia de scanf .
      printf("Error al leer la entrada.\n"); // si fgets falla, mostraremos el error
      intentos++; // e incrementaremos el contador de intentos inmediatamente
      continue; // para volver a pedir la entrada
    }

    buffer[strcspn(buffer, "\n")] = '\0'; // Nuevamente eliminaremos el salto de línea al final de la entrada

    if (buffer[0] == 'q' || buffer[0] == 'Q') { // Si el primer carácter es 'q' o 'Q'
      return 0; // Cancelaremos la operación
    }

    // En la siguiente condicion validaremos que la entrada sea exactamente un carácter entre '1' y '9'
    if (strlen(buffer) == 1 && buffer[0] >= '1' && buffer[0] <= '9') {
      *numero = buffer[0] - '0'; // como entra en formato stirng lo convertimos su valor numérico
      return 1; // y de todo estar correcto retormanos 1
    }

    printf("Error: Ingrese un número del 1 al 9 (o 'q' para cancelar).\n"); // de no estar correcto le daremos un mensaje de error al susuario 
    intentos++; // E incrementaremos el contador de intentos
  }

  printf("Demasiados intentos fallidos.\n"); //al exceder el máximo de intentos arrojará un mensaje de error
  return 0; // Y retornaremos 0 indicando que no se obtuvo un número válido
}

// Ahora vamos a crear una función que leera un carácter (letra de A-Z o '#') con validación. Lo que nos ayudará a filtrar artistas por su letra inicial o seleccionar la categoría "Otros" para aquellos artistas cuyas iniciales sean caracteres que no esten presentes en el rango de letras de la A a la Z. También podremos cancelar la accion con 'q'.

int leer_caracter(char *caracter, const char *prompt, int max_intentos) {
  char buffer[100]; // Comenzaremos creando un buffer para almacenar la entrada del usuario
  int intentos = 0; // igualmente crearemos un contador de intentos fallidos

  while (intentos < max_intentos) { // Mientras no se superen los intentos permitidos
    printf("%s", prompt); // Mostraremos el mensaje de solicitud al usuario

    if (!fgets(buffer, sizeof(buffer), stdin)) { // Leeremos la entrada de igual forma con fgets, ya explicamos en la funcion anterior porque nos conviene mas con fget
      printf("Error al leer la entrada.\n"); // Mostraremos un mensaje de error si fgets falla
      intentos++; // e incrementaremos el contador de intentos
      continue; // y volveremos a pedir la entrada al usuario
    }

    buffer[strcspn(buffer, "\n")] = '\0'; // Siempre tener presente que debemos eliminar el salto de línea '\n' al final para evitar problemas con la lectura posterior

    if (buffer[0] == 'q' || buffer[0] == 'Q') { // Si el primer carácter es 'q' o 'Q'
      return 0; //el programa cancelara la operación ya que es una señal de cancelación .
    }

    // la siguiente condicion validara que la entrada sea un solo carácter y sea letra (A-Z, a-z) o '#'
    if (strlen(buffer) == 1 && 
        ((buffer[0] >= 'A' && buffer[0] <= 'Z') || 
        (buffer[0] >= 'a' && buffer[0] <= 'z') || 
        buffer[0] == '#')) {
      *caracter = toupper(buffer[0]); // si escriben en minuscula convertiremos a mayúscula la entrada y la guardaremos en *caracter
      return 1; // retornamos 1 si la entrada es válida
    }

    // Si no pasó la validación, mostraremos un mensaje de error
    printf("Error: Ingrese una letra de A-Z o '#' para Otros (o 'q' para cancelar).\n");
    intentos++; // e incrementaremos el contador de intentos
  }

  printf("Demasiados intentos fallidos. Regresando al menú.\n"); //Al Exceder los intentos permitidos arrojaremos el mensaje de error 
  return 0; // y el return 0 nos indicara que no se obtuvo entrada válida
}

// Ahora implementaremos una función que valida si una cadena representa un número entero. Esto nos ayudará a verificar entradas numéricas, como los valores de tempo, asegurando que sean válidos antes de procesarlos.

int es_numero_valido(const char *str) {
  char *endptr; //Primero declararemos un puntero para detectar dónde termina la conversión
  strtol(str, &endptr, 10); // luego convertiremos la cadena en un número entero en base 10
  return *endptr == '\0' && strlen(str) > 0; // Y por ultimo verificamos que la cadena no esté vacía y que la conversion haya consumido toda la cadena
}

// Aquí vamos a crear una función que lee un número entero positivo (o cero) para los valores de tempo. Nos permitirá ingresar tempos como 56, 80 o 120, con validación para evitar entradas inválidas y la opción de cancelar con 'q'; esta función al igual que las anteriores, también limita el número de intentos para evitar bucles infinitos en caso de entradas incorrectas.

int leer_tempo(int *numero, const char *prompt, int max_intentos) {
  char buffer[100]; // Primero prepararemos un buffer para guardar lo que escriba el usuario
  int intentos = 0;  // tambien inicializaremos un contador de intentos fallidos

  // Luego comenzaremos un bucle que se repetirá hasta que el usuario ingrese un número válido o exceda el número máximo de intentos
  while (intentos < max_intentos) { // Mientras no superemos el máximo de intentos permitidos
    printf("%s", prompt); // Mostraremos el mensaje para pedir la entrada
    if (!fgets(buffer, sizeof(buffer), stdin)) { // Intentaremos leer lo que el usuario escriba
      printf("Error al leer la entrada.\n"); // Si no podemos leer, avisaremos del error
      intentos++; // Sumaremos un intento fallido
      continue; // Y volveremos a pedir la entrada
    }

    buffer[strcspn(buffer, "\n")] = '\0'; // Eliminaremos el salto de línea nuevamente

    if (buffer[0] == 'q' || buffer[0] == 'Q') { // Revisaremos si el usuario quiere cancelar
      return 0; // Si es así, cancelaremos la operación
    }

    if (es_numero_valido(buffer)) { // Si el texto que escribió parece ser un número válido...
      *numero = atoi(buffer); // Lo convertiremos a un número entero
      if (*numero >= 0) { // Verificaremos que sea un número positivo o igual a cero
        return 1; // Si todo está bien, confirmaremos la entrada válida
      }
      printf("Error: Ingrese un número mayor o igual a 0.\n"); // Si no, avisaremos del error
    } else {
      printf("Error: Ingrese un número válido (o 'q' para cancelar).\n"); // Si no era un número válido, también avisaremos
    }

    intentos++; // Cada error sumará un intento fallido
  }

  printf("Demasiados intentos fallidos.\n"); // Si llegamos aquí, avisaremos que hubo demasiados intentos
  return 0; // Y cerraremos la función indicando que no se pudo leer un número válido
}


// Ahora crearemos funciones comparadoras que nos ayudarán a trabajar con los mapas. Estas funciones compararan claves enteras y cadenas, permitiendonos buscar y organizar los datos en los mapas de canciones, artistas y géneros de manera eficiente.

int is_equal_int(void *key1, void *key2) {

  return (*(int *)key1 == *(int *)key2);//esta linea lo que hara sera comparar los valores de las claves enteras convirtiendo los punteros genericos a punteros de tipo int , si son iguales devolvera 1 y si no devolvera 0
}


int is_equal_str(void *key1, void *key2) {
  return (strcmp((char *)key1, (char *)key2) == 0);// Y esta otra linea comparara las cadenas de caracteres, devolviendo 1 si son iguales y 0 si no lo son
}

// Aquí implementaremos una función que compara dos cadenas alfabéticamente, pero en orden inverso. Nos ayudará a ordenar artistas en la lista de búsqueda, asegurando que aparezcan de forma organizada.

int comparar_strings(void *a, void *b) {
  char *str_a = ((ArtistaInfo *)a)->artista; // Aquí extraeremos el nombre del primer artista desde la estructura
  char *str_b = ((ArtistaInfo *)b)->artista; // Luego extraeremos el nombre del segundo artista
  return strcmp(str_a, str_b) < 0 ? 1 : 0; // Finalmente compararemos ambos nombres y devolveremos 1 si el primero va antes
}


// Ahora vamos a crear una función que verifica si una cadena es válida (no vacía y no solo espacios). Esto es útil para validar entradas como nombres de artistas o géneros antes de procesarlos.
int es_cadena_valida(const char *str) {
  return str[0] != '\0' && strspn(str, " \t\n") != strlen(str); // Aqui en esta linea de codigo verificaremos que la cadena no esté vacía y que no contenga solo espacios, tabulaciones o saltos de línea 
}


// Aquí implementaremos una función que clasifica el tempo de una canción en categorías: Lenta, Moderada o Rápida. Nos ayudará a mostrar la clasificación correcta cuando busquemos canciones por rango de tempo.
const char *clasificar_tempo(float tempo) {
  if (tempo < 80.0) return "Lenta";
  if (tempo <= 120.0) return "Moderada";
  return "Rápida";//aqui simplemente clasificamos el tempo de la cancion en una de las tres categorias y devolvemos la cadena correspondiente
}

// Ahora crearemos una función que muestra los rangos de tempo disponibles (Lentas, Moderadas, Rápidas). Esto guiará al usuario antes de ingresar los valores de tempo mínimo y máximo, haciendo la búsqueda más intuitiva.

void mostrar_categorias_tempo() {
  printf("\nRangos de tempo disponibles:\n");
  printf("Lentas (Tempo menos de 80 BPM)\n");
  printf("Moderadas (Tempo entre 80 y 120 BPM)\n");
  printf("Rápidas (Tempo mayor a 120 BPM)\n");
}// funcion especifica de los prints.

// posteriormente vamos a implementar una función que valida los valores de tempo mínimo y máximo ingresados por el usuario. Nos aseguraremos de que sean números válidos y que el mínimo no sea mayor que el máximo.

int validar_rango_tempo(int *min, int *max) {
  if (!leer_tempo(min, "Tempo mínimo (o 'q' para cancelar): ", 3)) { // Primero pediremos el tempo mínimo y verificaremos que sea válido
    return 0; // Si no es válido, cancelaremos la operación
  }
  if (!leer_tempo(max, "Tempo máximo (o 'q' para cancelar): ", 3)) { // Luego pediremos el tempo máximo y también validaremos su entrada
    return 0; // Si la entrada del tempo máximo no es válida, cancelamos
  }
  if (*min > *max) { // Verificaremos si el tempo mínimo es mayor que el máximo
    printf("Error: El tempo mínimo no puede ser mayor que el máximo.\n"); // Si lo es, mostramos un error
    return 0; // Si hay un error, cancelamos la operación
  }
  return 1; // Si todo es correcto, devolvemos 1 indicando que el rango es válido
}


// Ahora vamos a proceder con la creacion de una función que filtra y muestra las canciones dentro de un rango de tempo específico. Recorrerá el mapa de canciones, seleccionará las que cumplan con el rango y las mostrará con el formato solicitado.

void filtrar_y_mostrar_canciones_por_tempo(Map *canciones_by_id, int tempo_min, int tempo_max) {
  printf("\nCanciones en el rango de tempo %d - %d:\n", tempo_min, tempo_max); //Primero mostramos un mensaje al usuario con el rango de tempo que está buscando.

  int i = 1; // Luego inicializamos un contador para poder numerar las canciones que encontremos en el rango.
  MapPair *pair = map_first(canciones_by_id); // posterior a eso tomaremos la primera canción del mapa de canciones.
  
  while (pair) { // crearemos un bucle que recorrerá todas las canciones en el mapa.
    Song *cancion = pair->value; // Accedemos a la canción que estamos revisando en ese momento.
    
    if (cancion->tempo >= tempo_min && cancion->tempo <= tempo_max) { // Si el tempo de la canción está dentro del rango que el usuario ha solicitado
      printf("%d. %s (ID: %d, Tempo: %.2f, Clasificación: %s)\n", // Mostramos la información de la canción, como su número, título, ID, tempo y su clasificación.
            i++, cancion->title, cancion->id, cancion->tempo, clasificar_tempo(cancion->tempo)); // Aumentamos el contador y mostramos la canción.
    }
    pair = map_next(canciones_by_id); // Luego pasamos a la siguiente canción en el mapa.
  }
  
  if (i == 1) { // Si no encontramos ninguna canción que cumpla con el filtro
    printf("No se encontraron canciones en ese rango de tempo.\n"); // Le decimos al usuario que no hubo canciones dentro del rango que buscaba.
  }
}// aqui muere la funcion


// Aquí implementaremos la función principal para buscar canciones por tempo. Coordinará la muestra de categorías, la validación del rango y la filtración de canciones, manteniendo la lógica clara y organizada.

void buscar_por_tempo(Map *canciones_by_id) {
  mostrar_categorias_tempo(); // Primero, mostraremos las categorías de tempo disponibles para que el usuario pueda ver sus opciones.
  
  printf("\nIngrese el rango de tempo:\n"); // Luego le pediremos al usuario que ingrese el rango de tempo que desea buscar.
  
  int tempo_min, tempo_max; // Definiremos dos variables para almacenar el rango mínimo y máximo del tempo.

  if (!validar_rango_tempo(&tempo_min, &tempo_max)) { // Validaremos que el rango ingresado por el usuario sea correcto. Si no lo es, terminaremos la función aquí.
    return; // Si el rango no es válido, regresaremos sin hacer nada más.
  }

  filtrar_y_mostrar_canciones_por_tempo(canciones_by_id, tempo_min, tempo_max); // Si el rango es válido, filtraremos las canciones por tempo y las mostraremos al usuario.
}


// Ahora vamos a crear una función que muestra una lista de canciones con un formato específico, incluyendo título, ID y género. Es útil para mostrar resultados de búsquedas por artista o género, con un mensaje personalizado si no hay resultados.

void mostrar_canciones(List *canciones, const char *contexto) {
  int i = 1; // Empezaremos numerando las canciones desde el 1.
  
  Song *cancion = list_first(canciones); // Luego, tomaremos la primera canción de la lista para comenzar a recorrerla.

  while (cancion != NULL) { // Mientras haya canciones en la lista, seguiremos mostrando una a una.
    printf("%d. %s (ID: %d, Género: %s)\n", i++, cancion->title, cancion->id, cancion->genre); // Mostraremos los detalles de cada canción, como el título, ID y género.
    cancion = list_next(canciones); // Avanzaremos a la siguiente canción en la lista.
  }
  
  if (i == 1) { // Si no se han encontrado canciones, es decir, si no se ha recorrido ninguna, haremos lo siguiente.
    printf("No se encontraron canciones para %s.\n", contexto); // Informaremos al usuario que no hay canciones en el contexto proporcionado.
  }
}


// Aquí crearemos una función que recolecta y ordena los artistas de un mapa de canciones. Nos ayudará a construir una lista ordenada alfabéticamente para la búsqueda por artista, incluyendo el número de canciones por artista.

List *recolectar_artistas(Map *canciones_by_artista) {
  List *artistas_temp = list_create(); // Primero, vamos a crear una lista temporal donde almacenaremos los artistas. 

  if (!artistas_temp) return NULL; // Si no se pudo crear la lista, devolveremos NULL.

  MapPair *pair = map_first(canciones_by_artista); // Luego, tomaremos el primer par de clave-valor del mapa de canciones por artista.
  
  while (pair) { // Mientras tengamos pares de artista y canciones, seguiremos procesándolos.
    ArtistaInfo *info = malloc(sizeof(ArtistaInfo)); // Para cada artista, vamos a reservar memoria para almacenar la información del artista.
    if (!info) { // Si no se pudo reservar memoria, limpiamos lo que hemos hecho hasta ahora y devolvemos NULL.
      list_clean(artistas_temp); // Limpiamos la lista temporal.
      free(artistas_temp); // Liberamos la memoria de la lista temporal.
      return NULL;
    }

    info->artista = pair->key; // Guardamos el nombre del artista en la estructura.
    List *canciones = pair->value; // Tomamos la lista de canciones asociadas al artista.
    info->num_canciones = list_size(canciones); // Guardamos la cantidad de canciones del artista.

    list_pushBack(artistas_temp, info); // Finalmente, añadimos la información del artista a la lista temporal.

    pair = map_next(canciones_by_artista); // Avanzamos al siguiente par de la mapa.
  }

  List *artistas_list = list_create(); // Ahora creamos una lista nueva donde almacenaremos los artistas ordenados.

  if (!artistas_list) { // Si no se pudo crear la nueva lista, limpiamos y liberamos lo que hicimos previamente y devolvemos NULL.
    ArtistaInfo *info = list_first(artistas_temp);
    while (info) { // Liberamos la memoria de los artistas de la lista temporal.
      free(info);
      info = list_next(artistas_temp);// Avanzamos al siguiente artista.
    }
    list_clean(artistas_temp); // Limpiamos la lista temporal.
    free(artistas_temp); // Liberamos la memoria de la lista temporal.
    return NULL;// Devolvemos NULL indicando que no se pudo crear la lista.
  }

  ArtistaInfo *info = list_first(artistas_temp); // Ahora tomamos el primer artista de la lista temporal.
  
  while (info) { // Mientras haya artistas en la lista temporal, los ordenamos y los agregamos a la lista final.
    list_sortedInsert(artistas_list, info, comparar_strings); // Insertamos el artista en la lista final de manera ordenada.
    info = list_next(artistas_temp); // Pasamos al siguiente artista de la lista temporal.
  }

  list_clean(artistas_temp); // Limpiamos la lista temporal.
  free(artistas_temp); // Liberamos la memoria de la lista temporal.

  return artistas_list; // Finalmente, devolvemos la lista con los artistas ordenados.
}

// Ahora implementaremos una función que muestra los artistas de una lista de manera ordenada, con su nombre y número de canciones. Es perfecta para presentar los artistas disponibles al usuario durante la búsqueda.

void mostrar_artistas_ordenados(List *artistas_list) {
  int i = 1; // Comenzamos con un contador para numerar los artistas que se mostrarán.

  ArtistaInfo *info = list_first(artistas_list); // Tomamos el primer artista de la lista ordenada.

  while (info) { // Mientras tengamos un artista en la lista, lo mostramos.
    printf("%d. %s (%d canciones)\n", i++, info->artista, info->num_canciones); // Imprimimos el número de artista, su nombre y la cantidad de canciones.
    info = list_next(artistas_list); // Pasamos al siguiente artista en la lista.
  }

  if (i == 1) { // Si no hemos mostrado ningún artista (es decir, la lista estaba vacía), mostramos un mensaje de que no hay artistas.
    printf("No hay artistas disponibles.\n");
  }
}

// Aquí vamos a crear una función que libera la memoria de una lista de artistas. Nos asegurará que no haya fugas de memoria al finalizar la búsqueda por artista, manteniendo el programa eficiente.

void liberar_artistas_list(List *artistas_list) {
  // Empezamos tomando el primer elemento de la lista.
  ArtistaInfo *info = list_first(artistas_list);
  // Cada elemento es un "ArtistaInfo", que guarda el nombre del artista y el número de canciones.
  
  // Mientras haya elementos en la lista (o sea, mientras no hayamos llegado al final de la libreta), seguimos adelante.
  while (info) {    
    free(info);// Liberamos la memoria del elemento actual, como si borráramos una página de la libreta. Esto asegura que no quede ocupando espacio.    
    info = list_next(artistas_list);// Pasamos al siguiente elemento de la lista, como si diéramos la vuelta a la siguiente página.
  }
  
  list_clean(artistas_list);// Ahora que ya borramos todos los elementos, limpiamos cualquier rastro que quede de la lista
  
  // Finalmente, liberamos la memoria de la lista misma.
  free(artistas_list);
}

// Ahora crearemos una función que filtra artistas por su letra inicial o por la categoría "Otros" (nombres que no comienzan con letra). Nos ayudará a organizar los artistas en la búsqueda por letra.

void filtrar_artistas_por_letra(List *artistas_list, char letra, List *filtrados, List *otros) {
  ArtistaInfo *info = list_first(artistas_list); // Tomaremos el primer artista de la lista para empezar a revisarlo.
  while (info) { // Mientras haya artistas por revisar, continuaremos con el proceso.
    char inicial = toupper(info->artista[0]); // Obtendremos la primera letra del nombre del artista y la convertiremos a mayúscula para comparar sin preocuparnos por minúsculas.
    if (letra == '#') { // Si buscamos "Otros" (nombres que no empiezan con una letra)...
      if (!isalpha(inicial)) { // ...y la inicial no es una letra (como un número o símbolo)...
        list_pushBack(otros, info); // ...agregaremos este artista a la lista de "Otros".
      }
    } else { // Si buscamos una letra específica, como 'A' o 'B'...
      if (inicial == letra) { // ...y la inicial del artista coincide con la letra que queremos...
        list_pushBack(filtrados, info); // ...lo agregaremos a la lista de artistas filtrados.
      } else if (!isalpha(inicial)) { // ...pero si la inicial no es una letra (como un número o símbolo)...
        list_pushBack(otros, info); // ...lo colocaremos en la lista de "Otros".
      }
    }
    info = list_next(artistas_list); // Pasaremos al siguiente artista de la lista para continuar revisando.
  }
}

// Aquí implementaremos una función que lee canciones desde un archivo CSV y las organiza en mapas por ID, género y artista. Es el núcleo de la carga de datos, permitiendo que el programa acceda a las canciones de manera eficiente.

void leer_canciones(Map *canciones_by_id, Map *canciones_by_genero, Map *canciones_by_artista) {
  FILE *archivo = fopen("C:\\Users\\victor\\OneDrive\\Desktop\\tarea2\\TDAs\\song_dataset_.csv", "r"); // Primero que todo abriremos el archivo CSV que contiene las canciones.
  if (!archivo) { // Si no podemos abrir el archivo...
    perror("Error al abrir el archivo"); // ...mostraremos un mensaje de error.
    return; // ...y terminaremos la función.
  }

  char **campos = leer_linea_csv(archivo, ','); // Leeremos la primera línea del CSV (los encabezados).
  if (!campos) { // Si no podemos leer los encabezados...
    printf("Error: No se pudieron leer los encabezados del CSV.\n"); // ...mostraremos un error.
    fclose(archivo); // ...cerraremos el archivo.
    return; // ...y terminaremos la función.
  }

  int k = 0; // Llevaremos un conteo de las canciones procesadas.
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) { // Mientras haya líneas en el CSV, las leeremos.
    if (k++ > 60000) break; // Si procesamos más de 60,000 canciones, pararemos para no sobrecargar.
    if (k % 10000 == 0) { // Cada 10,000 canciones...
      printf("Procesadas %d canciones...\n", k); // ...mostraremos un mensaje para mostrar el progreso.
    }

    if (!campos[0] || !campos[2] || !campos[3] || !campos[4] || !campos[18] || !campos[20]) { // Si faltan datos importantes en la línea...
      continue; // ...pasaremos a la siguiente línea.
    }

    Song *song = malloc(sizeof(Song)); // Reservaremos memoria para una nueva canción.
    if (!song) { // Si no podemos reservar memoria...
      fclose(archivo); // ...cerraremos el archivo.
      return; // ...y terminaremos la función.
    }

    song->id = atoi(campos[0]); // Guardaremos el ID de la canción convirtiendo el texto a número.
    strcpy(song->title, campos[4][0] ? campos[4] : "Sin título"); // Copiaremos el título, o "Sin título" si está vacío.
    song->artists = split_string(campos[2][0] ? campos[2] : "", ";"); // Dividiremos la lista de artistas en una lista, usando ";" como separador.
    if (!song->artists) { // Si no podemos crear la lista de artistas...
      free(song); // ...liberaremos la memoria de la canción.
      continue; // ...y pasaremos a la siguiente línea.
    }
    strcpy(song->album, campos[3][0] ? campos[3] : "Sin álbum"); // Copiaremos el álbum, o "Sin álbum" si está vacío.
    strcpy(song->genre, campos[20][0] ? campos[20] : "Desconocido"); // Copiaremos el género, o "Desconocido" si está vacío.
    song->tempo = atof(campos[18]); // Guardaremos el tempo convirtiendo el texto a número decimal.

    int *id_key = malloc(sizeof(int)); // Reservaremos memoria para la clave del ID.
    if (!id_key) { // Si no podemos reservar memoria...
      list_clean(song->artists); // ...limpiaremos la lista de artistas.
      free(song->artists); // ...liberaremos la memoria de la lista.
      free(song); // ...liberaremos la memoria de la canción.
      fclose(archivo); // ...cerraremos el archivo.
      return; // ...y terminaremos la función.
    }
    *id_key = song->id; // Guardaremos el ID en la clave.
    map_insert(canciones_by_id, id_key, song); // Insertaremos la canción en el mapa por ID.

    char *genre_key = strdup(song->genre); // Crearemos una copia del género para usarla como clave.
    if (!genre_key) { // Si no podemos crear la copia...
      free(id_key); // ...liberaremos la clave del ID.
      list_clean(song->artists); // ...limpiaremos la lista de artistas.
      free(song->artists); // ...liberaremos la memoria de la lista.
      free(song); // ...liberaremos la memoria de la canción.
      fclose(archivo); // ...cerraremos el archivo.
      return; // ...y terminaremos la función.
    }
    MapPair *pair = map_search(canciones_by_genero, genre_key); // Buscaremos si el género ya está en el mapa.
    if (!pair) { // Si el género no está...
      List *list = list_create(); // ...crearemos una nueva lista para las canciones de este género.
      if (!list) { // Si no podemos crear la lista...
        free(genre_key); // ...liberaremos la clave del género.
        free(id_key); // ...liberaremos la clave del ID.
        list_clean(song->artists); // ...limpiaremos la lista de artistas.
        free(song->artists); // ...liberaremos la memoria de la lista.
        free(song); // ...liberaremos la memoria de la canción.
        fclose(archivo); // ...cerraremos el archivo.
        return; // ...y terminaremos la función.
      }
      list_pushBack(list, song); // Agregaremos la canción a la nueva lista.
      map_insert(canciones_by_genero, genre_key, list); // Insertaremos la lista en el mapa por género.
    } else { // Si el género ya existe...
      list_pushBack(pair->value, song); // ...agregaremos la canción a la lista existente.
      free(genre_key); // ...y liberaremos la copia del género.
    }

    char *artist = list_first(song->artists); // Tomaremos el primer artista de la lista de artistas.
    while (artist) { // Mientras haya artistas por revisar...
      MapPair *artist_pair = map_search(canciones_by_artista, artist); // ...buscaremos si el artista ya está en el mapa.
      if (!artist_pair) { // Si el artista no está...
        List *list = list_create(); // ...crearemos una nueva lista para sus canciones.
        if (!list) { // Si no podemos crear la lista...
          free(id_key); // ...liberaremos la clave del ID.
          list_clean(song->artists); // ...limpiaremos la lista de artistas.
          free(song->artists); // ...liberaremos la memoria de la lista.
          free(song); // ...liberaremos la memoria de la canción.
          fclose(archivo); // ...cerraremos el archivo.
          return; // ...y terminaremos la función.
        }
        list_pushBack(list, song); // Agregaremos la canción a la nueva lista.
        char *artist_key = strdup(artist); // Crearemos una copia del nombre del artista para usarla como clave.
        if (!artist_key) { // Si no podemos crear la copia...
          list_clean(list); // ...limpiaremos la lista recién creada.
          free(list); // ...liberaremos la memoria de la lista.
          free(id_key); // ...liberaremos la clave del ID.
          list_clean(song->artists); // ...limpiaremos la lista de artistas.
          free(song->artists); // ...liberaremos la memoria de la lista.
          free(song); // ...liberaremos la memoria de la canción.
          fclose(archivo); // ...cerraremos el archivo.
          return; // ...y terminaremos la función.
        }
        map_insert(canciones_by_artista, artist_key, list); // Insertaremos la lista en el mapa por artista.
      } else { // Si el artista ya existe...
        list_pushBack(artist_pair->value, song); // ...agregaremos la canción a la lista existente.
      }
      artist = list_next(song->artists); // Pasaremos al siguiente artista de la lista.
    }
  }

  printf("Total de canciones cargadas: %d\n", k); // Mostraremos cuántas canciones cargamos en total.
  fclose(archivo); // Cerraremos el archivo.
}

/* Ahora vamos a crear una función que permite buscar canciones por artista. Pedirá una letra inicial, mostrará artistas disponibles, y luego dejará al usuario seleccionar uno para ver sus canciones.Esta función es útil para filtrar artistas y mostrar solo aquellos que comienzan con una letra específica, facilitando la búsqueda.La función también maneja la opción de cancelar la búsqueda si el usuario lo desea.*/

void buscar_por_artista(Map *canciones_by_artista) {
  List *artistas_list = recolectar_artistas(canciones_by_artista); // Juntaremos todos los artistas en una lista para trabajar con ellos.
  if (!artistas_list) { // Si no podemos crear la lista de artistas...
    printf("Error: No se pudo recolectar la lista de artistas.\n"); // ...mostraremos un mensaje de error.
    return; // ...y terminaremos la función.
  }

  char letra_input; // Guardaremos la letra que el usuario ingrese.
  printf("\nIngrese la letra inicial del artista (A-Z, '#' para Otros, o 'q' para cancelar): "); // Pediremos al usuario que ingrese una letra.
  if (!leer_caracter(&letra_input, "", 3)) { // Si no podemos leer la letra después de varios intentos...
    liberar_artistas_list(artistas_list); // ...limpiaremos la lista de artistas.
    return; // ...y terminaremos la función.
  }
  if (letra_input != '#' && !isalpha(letra_input)) { // Si la letra no es '#' ni una letra de A-Z...
    printf("Error: Seleccione una letra de A-Z o '#' para Otros.\n"); // ...mostraremos un error.
    liberar_artistas_list(artistas_list); // ...limpiaremos la lista de artistas.
    return; // ...y terminaremos la función.
  }

  List *filtrados = list_create(); // Crearemos una lista para los artistas que empiecen con la letra elegida.
  List *otros = list_create(); // Crearemos otra lista para los artistas que no empiecen con una letra (como números o símbolos).
  if (!filtrados || !otros) { // Si no podemos crear alguna de las listas...
    printf("Error: No se pudo crear listas para filtrado.\n"); // ...mostraremos un error.
    if (filtrados) list_clean(filtrados); // ...limpiaremos la lista de filtrados si existe.
    if (otros) list_clean(otros); // ...limpiaremos la lista de otros si existe.
    free(filtrados); // ...liberaremos la memoria de la lista de filtrados.
    free(otros); // ...liberaremos la memoria de la lista de otros.
    liberar_artistas_list(artistas_list); // ...limpiaremos la lista de artistas.
    return; // ...y terminaremos la función.
  }

  filtrar_artistas_por_letra(artistas_list, letra_input, filtrados, otros); // Separaremos los artistas en las listas de filtrados y otros según la letra.

  if (letra_input == '#') { // Si el usuario eligió '#' para "Otros"...
    printf("\nArtistas en la categoría 'Otros':\n"); // ...mostraremos un encabezado para la categoría "Otros".
    mostrar_artistas_ordenados(otros); // ...y listaremos los artistas que no empiezan con letra.
  } else { // Si el usuario eligió una letra específica...
    printf("\nArtistas que comienzan con '%c':\n", letra_input); // ...mostraremos un encabezado con la letra elegida.
    mostrar_artistas_ordenados(filtrados); // ...y listaremos los artistas que empiecen con esa letra.
  }

  if (list_size(filtrados) == 0 && (letra_input != '#' || list_size(otros) == 0)) { // Si no hay artistas en la lista correspondiente...
    printf("No hay artistas disponibles para la selección.\n"); // ...informaremos que no hay artistas para elegir.
    list_clean(filtrados); // ...limpiaremos la lista de filtrados.
    list_clean(otros); // ...limpiaremos la lista de otros.
    free(filtrados); // ...liberaremos la memoria de la lista de filtrados.
    free(otros); // ...liberaremos la memoria de la lista de otros.
    liberar_artistas_list(artistas_list); // ...limpiaremos la lista de artistas.
    return; // ...y terminaremos la función.
  }

  char artista[100]; // Prepararemos un espacio para guardar el nombre del artista que el usuario elija.
  if (!leer_cadena(artista, sizeof(artista), "\nIngrese el nombre del artista (o 'q' para cancelar): ", 3)) { // Si no podemos leer el nombre del artista...
    list_clean(filtrados); // ...limpiaremos la lista de filtrados.
    list_clean(otros); // ...limpiaremos la lista de otros.
    free(filtrados); // ...liberaremos la memoria de la lista de filtrados.
    free(otros); // ...liberaremos la memoria de la lista de otros.
    liberar_artistas_list(artistas_list); // ...limpiaremos la lista de artistas.
    return; // ...y terminaremos la función.
  }

  MapPair *par = map_search(canciones_by_artista, artista); // Buscaremos el artista en el mapa de canciones por artista.
  if (!par) { // Si no encontramos al artista...
    printf("No se encontraron canciones para el artista '%s'.\n", artista); // ...informaremos que no hay canciones para ese artista.
    list_clean(filtrados); // ...limpiaremos la lista de filtrados.
    list_clean(otros); // ...limpiaremos la lista de otros.
    free(filtrados); // ...liberaremos la memoria de la lista de filtrados.
    free(otros); // ...liberaremos la memoria de la lista de otros.
    liberar_artistas_list(artistas_list); // ...limpiaremos la lista de artistas.
    return; // ...y terminaremos la función.
  }

  mostrar_canciones(par->value, artista); // Mostraremos las canciones del artista elegido.
  list_clean(filtrados); // Limpiaremos la lista de filtrados.
  list_clean(otros); // Limpiaremos la lista de otros.
  free(filtrados); // Liberaremos la memoria de la lista de filtrados.
  free(otros); // Liberaremos la memoria de la lista de otros.
  liberar_artistas_list(artistas_list); // Limpiaremos la lista de artistas.
}
// Ahora vamos a crear una función que valida el género ingresado por el usuario. Verificará si es una cadena válida y si existe en nuestro mapa de canciones por género. Devolverá 1 si todo está bien, o 0 si hay algún problema.

int validar_genero(Map *canciones_by_genero, char *genero_input, MapPair **par) {
  if (!es_cadena_valida(genero_input)) { // primero verificaremos si el género ingresado es una cadena válida.
    printf("Error: Género inválido.\n"); // Si no es válido, mostraremos un mensaje de error.
    return 0; // Terminaremos la función con un error.
  }

  *par = map_search(canciones_by_genero, genero_input); // Buscaremos el género en el mapa de canciones por género.
  if (!*par) { // Si no encontramos el género...
    printf("No se encontraron canciones para el género '%s'.\n", genero_input); // ...informaremos que no hay canciones para ese género.
    return 0; // Terminaremos la función con un error.
  }
  return 1; // Si todo está bien, confirmaremos que el género es válido y existe.
}

// Ahora crearemos una función que filtra canciones por la letra inicial de su título. Nos permitirá mostrar solo las canciones que comienzan con una letra específica dentro de un género.

List *filtrar_canciones_por_letra(List *canciones, char letra) {
  List *filtradas = list_create(); // Crearemos una nueva lista para guardar las canciones que cumplan con el criterio.
  Song *cancion = list_first(canciones); // Tomaremos la primera canción de la lista para empezar a revisarla.
  while (cancion) { // Mientras haya canciones por revisar, continuaremos.
    if (toupper(cancion->title[0]) == letra) { // Si la primera letra del título, en mayúscula, coincide con la letra dada...
      list_pushBack(filtradas, cancion); // ...agregaremos la canción a la lista filtrada.
    }
    cancion = list_next(canciones); // Pasaremos a la siguiente canción para seguir revisando.
  }
  return filtradas; // Devolveremos la lista con las canciones filtradas.
}

// En esta función, guardaremos en un archivo las canciones filtradas de un género específico que comienzan con una letra dada, incluyendo un encabezado con la información del género y la letra.

void guardar_canciones_filtradas(List *filtradas, const char *genero, char letra, const char *nombre_archivo) {
  FILE *archivo = fopen(nombre_archivo, "w"); // Abriremos un nuevo archivo en modo escritura para guardar las canciones.
  if (!archivo) { // Si no podemos crear el archivo...
    printf("Error al crear el archivo '%s'.\n", nombre_archivo); // ...mostraremos un mensaje de error.
    return; // ...y terminaremos la función.
  }

  fprintf(archivo, "Canciones del género '%s' que comienzan con '%c':\n", genero, letra); // Escribiremos un encabezado en el archivo con el género y la letra.
  Song *cancion = list_first(filtradas); // Tomaremos la primera canción de la lista filtrada para empezar a procesarla.
  while (cancion) { // Mientras haya canciones por revisar, continuaremos.
    fprintf(archivo, "%s (ID: %d)\n", cancion->title, cancion->id); // Escribiremos el título y el ID de la canción en el archivo.
    cancion = list_next(filtradas); // Pasaremos a la siguiente canción para seguir procesando.
  }

  fclose(archivo); // Cerraremos el archivo después de escribir todo.
  printf("Canciones filtradas guardadas en '%s'.\n", nombre_archivo); // Informaremos al usuario que las canciones se guardaron correctamente.
}

// En esta función, buscaremos canciones por género. Mostraremos los géneros disponibles, pediremos un género y una letra inicial, y guardaremos las canciones filtradas en un archivo.

void buscar_por_genero(Map *canciones_by_genero) {
  List *generos_list = list_create(); // Crearemos una lista para almacenar los géneros disponibles.
  if (!generos_list) { // Si no podemos crear la lista...
    printf("Error: No se pudo crear lista de géneros.\n"); // ...mostraremos un mensaje de error.
    return; // ...y terminaremos la función.
  }

  size_t max_len = 0; // Guardaremos la longitud máxima de los nombres de géneros para alinear la salida.
  int num_generos = 0; // Contaremos cuántos géneros válidos encontramos.
  MapPair *pair = map_first(canciones_by_genero); // Tomaremos el primer género del mapa para empezar a revisarlo.
  while (pair) { // Mientras haya géneros por revisar, continuaremos.
    char *genero = pair->key; // Obtendremos el nombre del género actual.
    int is_valid = 1; // Asumiremos que el género es válido hasta que se demuestre lo contrario.
    for (int i = 0; genero[i]; i++) { // Recorreremos cada carácter del género.
      if (!isalpha(genero[i]) && genero[i] != '-' && genero[i] != ' ') { // Si el carácter no es letra, guión ni espacio...
        is_valid = 0; // ...marcaremos el género como no válido.
        break; // ...y dejaremos de revisar.
      }
    }
    if (is_valid) { // Si el género es válido...
      GeneroInfo *info = malloc(sizeof(GeneroInfo)); // ...reservaremos memoria para guardar su información.
      if (!info) { // Si no podemos reservar memoria...
        list_clean(generos_list); // ...limpiaremos la lista de géneros.
        free(generos_list); // ...liberaremos la memoria de la lista.
        return; // ...y terminaremos la función.
      }
      info->genero = genero; // Guardaremos el nombre del género.
      info->num_canciones = list_size(pair->value); // Contaremos cuántas canciones tiene este género.
      list_pushBack(generos_list, info); // Agregaremos la información del género a la lista.
      size_t len = strlen(info->genero) + 4 + (info->num_canciones / 10 + 1); // Calcularemos la longitud del texto para mostrar el género.
      if (len > max_len) max_len = len; // Actualizaremos la longitud máxima si es necesario.
      num_generos++; // Incrementaremos el conteo de géneros válidos.
    }
    pair = map_next(canciones_by_genero); // Pasaremos al siguiente género.
  }

  if (num_generos == 0) { // Si no encontramos géneros válidos...
    printf("No hay géneros válidos disponibles.\n"); // ...informaremos al usuario.
    list_clean(generos_list); // ...limpiaremos la lista de géneros.
    free(generos_list); // ...liberaremos la memoria de la lista.
    return; // ...y terminaremos la función.
  }

  printf("\nGéneros disponibles:\n"); // Mostraremos un encabezado para los géneros.
  int cols = 4, col_width = max_len + 2 > 12 ? max_len + 2 : 12; // Definiremos el número de columnas y su ancho para mostrar los géneros.
  int rows = (num_generos + cols - 1) / cols; // Calcularemos cuántas filas necesitamos.
  GeneroInfo *info = list_first(generos_list); // Tomaremos el primer género de la lista para mostrarlo.
  int index = 0; // Llevaremos un conteo de los géneros mostrados.
  for (int i = 0; i < rows && index < num_generos; i++) { // Recorreremos las filas para mostrar los géneros.
    for (int j = 0; j < cols && index < num_generos; j++) { // Recorreremos las columnas para cada fila.
      printf(" %-s (%d)", info->genero, info->num_canciones); // Mostraremos el género y su número de canciones.
      int len = strlen(info->genero) + (info->num_canciones / 10 + 1) + 4; // Calcularemos la longitud del texto mostrado.
      for (int k = len; k < col_width; k++) printf(" "); // Añadiremos espacios para alinear las columnas.
      info = list_next(generos_list); // Pasaremos al siguiente género.
      index++; // Incrementaremos el conteo de géneros mostrados.
    }
    printf("\n"); // Añadiremos un salto de línea después de cada fila.
  }

  char genero_input[100]; // Prepararemos un espacio para el género que ingrese el usuario.
  if (!leer_cadena(genero_input, sizeof(genero_input), "\nIngrese el género a buscar (o 'q' para cancelar): ", 3)) { // Pediremos al usuario que ingrese un género.
    goto limpiar; // Si no podemos leer el género, saltaremos a limpiar la memoria.
  }

  MapPair *par; // Prepararemos un espacio para guardar el par del mapa encontrado.
  if (!validar_genero(canciones_by_genero, genero_input, &par)) { // Verificaremos si el género es válido y existe.
    goto limpiar; // Si no es válido, saltaremos a limpiar la memoria.
  }

  char letra_input; // Guardaremos la letra inicial que el usuario ingrese.
  if (!leer_caracter(&letra_input, "Ingrese la letra inicial (A-Z, o 'q' para cancelar): ", 3)) { // Pediremos al usuario que ingrese una letra.
    goto limpiar; // Si no podemos leer la letra, saltaremos a limpiar la memoria.
  }

  List *filtradas = filtrar_canciones_por_letra(par->value, letra_input); // Filtraremos las canciones del género que comiencen con la letra dada.
  int count = list_size(filtradas); // Contaremos cuántas canciones filtramos.
  if (count == 0) { // Si no encontramos canciones...
    printf("\nNo se encontraron canciones que comiencen con '%c' en el género '%s'.\n", letra_input, genero_input); // ...informaremos al usuario.
    list_clean(filtradas); // ...limpiaremos la lista filtrada.
    free(filtradas); // ...liberaremos la memoria de la lista.
    goto limpiar; // ...y saltaremos a limpiar la memoria.
  }

  printf("\nCanciones del género '%s' que comienzan con '%c':\n", genero_input, letra_input); // Mostraremos un encabezado con el género y la letra.
  int songs_per_row = 4, current = 0; // Definiremos cuántas canciones mostraremos por fila y llevaremos un conteo.
  Song *cancion = list_first(filtradas); // Tomaremos la primera canción filtrada para mostrarla.
  while (cancion) { // Mientras haya canciones por mostrar, continuaremos.
    printf(" %-s (ID: %d)", cancion->title, cancion->id); // Mostraremos el título y el ID de la canción.
    current++; // Incrementaremos el conteo de canciones mostradas.
    if (current % songs_per_row == 0) printf("\n"); // Si completamos una fila, añadiremos un salto de línea.
    else printf(" |"); // Si no, separaremos las canciones con una barra.
    cancion = list_next(filtradas); // Pasaremos a la siguiente canción.
  }
  if (current % songs_per_row != 0) printf("\n"); // Añadiremos un salto de línea si no terminamos en una fila completa.

  char archivo_nombre[100]; // Prepararemos un espacio para el nombre del archivo donde guardaremos las canciones.
  snprintf(archivo_nombre, sizeof(archivo_nombre), "%c_canciones.txt", letra_input); // Crearemos el nombre del archivo usando la letra inicial.
  guardar_canciones_filtradas(filtradas, genero_input, letra_input, archivo_nombre); // Guardaremos las canciones filtradas en el archivo.

  list_clean(filtradas); // Limpiaremos la lista filtrada.
  free(filtradas); // Liberaremos la memoria de la lista filtrada.

limpiar:
  info = list_first(generos_list); // Tomaremos el primer género de la lista para empezar a limpiar.
  while (info) { // Mientras haya géneros por limpiar, continuaremos.
    free(info); // Liberaremos la memoria de la información del género.
    info = list_next(generos_list); // Pasaremos al siguiente género.
  }
  list_clean(generos_list); // Limpiaremos la lista de géneros.
  free(generos_list); // Liberaremos la memoria de la lista de géneros.
}

// En esta función, permitiremos al usuario crear una nueva lista de reproducción. Pediremos un nombre y verificaremos que no exista antes, asegurándonos de que la lista se cree correctamente.

void crear_lista_reproduccion(Map *playlists) {
  char nombre_lista[100]; // Prepararemos un espacio para el nombre de la lista que ingrese el usuario.
  if (!leer_cadena(nombre_lista, sizeof(nombre_lista), "Ingrese el nombre de la nueva lista de reproducción (o 'q' para cancelar): ", 3)) { // Pediremos el nombre de la lista.
    return; // Si no podemos leer el nombre, terminaremos la función.
  }

  MapPair *pair = map_search(playlists, nombre_lista); // Buscaremos si ya existe una lista con ese nombre.
  if (pair) { // Si la lista ya existe...
    printf("Error: Ya existe una lista de reproducción con el nombre '%s'.\n", nombre_lista); // ...informaremos al usuario.
    return; // ...y terminaremos la función.
  }

  List *lista = list_create(); // Crearemos una nueva lista para la lista de reproducción.
  if (!lista) { // Si no podemos crear la lista...
    printf("Error: No se pudo crear la lista de reproducción.\n"); // ...mostraremos un mensaje de error.
    return; // ...y terminaremos la función.
  }

  char *nombre_key = strdup(nombre_lista); // Haremos una copia del nombre para usarla como clave en el mapa.
  if (!nombre_key) { // Si no podemos crear la copia...
    list_clean(lista); // ...limpiaremos la lista creada.
    free(lista); // ...liberaremos la memoria de la lista.
    return; // ...y terminaremos la función.
  }
  map_insert(playlists, nombre_key, lista); // Insertaremos la nueva lista en el mapa de listas de reproducción.
  printf("Lista de reproducción '%s' creada exitosamente.\n", nombre_lista); // Informaremos al usuario que la lista se creó correctamente.
}

// En esta función, mostraremos todas las listas de reproducción disponibles. Informaremos al usuario qué listas existen para que puedan elegir una.

void listar_playlists(Map *playlists) {
  printf("\nListas de reproducción disponibles:\n"); // Mostraremos un encabezado para las listas.
  MapPair *pair = map_first(playlists); // Tomaremos la primera lista del mapa para empezar a mostrarla.
  if (!pair) { // Si no hay listas...
    printf("No hay listas de reproducción creadas.\n"); // ...informaremos al usuario.
    return; // ...y terminaremos la función.
  }
  int i = 1; // Llevaremos un conteo para numerar las listas.
  while (pair) { // Mientras haya listas por mostrar, continuaremos.
    printf("%d. %s\n", i++, (char *)pair->key); // Mostraremos el número y el nombre de la lista.
    pair = map_next(playlists); // Pasaremos a la siguiente lista.
  }
}

// En esta función, permitiremos al usuario agregar canciones a una lista de reproducción existente. Pediremos el nombre de la lista y los IDs de las canciones, validando cada entrada.

void agregar_cancion_a_lista(Map *canciones_by_id, Map *playlists) {
  listar_playlists(playlists); // Mostraremos las listas de reproducción disponibles.
  char nombre_lista[100]; // Prepararemos un espacio para el nombre de la lista que ingrese el usuario.
  if (!leer_cadena(nombre_lista, sizeof(nombre_lista), "\nIngrese el nombre de la lista de reproducción (o 'q' para cancelar): ", 3)) { // Pediremos el nombre de la lista.
    return; // Si no podemos leer el nombre, terminaremos la función.
  }

  MapPair *pair = map_search(playlists, nombre_lista); // Buscaremos la lista en el mapa.
  if (!pair) { // Si la lista no existe...
    printf("Error: No existe una lista de reproducción con el nombre '%s'.\n", nombre_lista); // ...informaremos al usuario.
    return; // ...y terminaremos la función.
  }
  List *lista = pair->value; // Obtendremos la lista de reproducción encontrada.

  char id_cancion[100]; // Prepararemos un espacio para el ID de la canción que ingrese el usuario.
  char continuar; // Guardaremos la respuesta del usuario sobre si quiere seguir agregando canciones.
  do { // Repetiremos el proceso mientras el usuario quiera continuar.
    if (!leer_cadena(id_cancion, sizeof(id_cancion), "\nIngrese el ID de la canción (o 'q' para cancelar): ", 3)) { // Pediremos el ID de la canción.
      return; // Si no podemos leer el ID, terminaremos la función.
    }

    if (!es_numero_valido(id_cancion)) { // Si el ID no es un número válido...
      printf("Error: ID de canción inválido.\n"); // ...mostraremos un mensaje de error.
      printf("¿Desea intentarlo de nuevo? (s/n): "); // ...preguntaremos si quiere intentar de nuevo.
      scanf(" %c", &continuar); // Leeremos la respuesta.
      limpiar_buffer(); // Limpiaremos el buffer de entrada.
      if (continuar != 's' && continuar != 'S') return; // Si no quiere continuar, terminaremos.
      continue; // Si quiere intentar, volveremos al inicio del bucle.
    }

    int id = atoi(id_cancion); // Convertiremos el ID a un número entero.
    MapPair *song_pair = map_search(canciones_by_id, &id); // Buscaremos la canción en el mapa por ID.
    if (!song_pair) { // Si no encontramos la canción...
      printf("Error: No se encontró una canción con el ID '%s'.\n", id_cancion); // ...informaremos al usuario.
      printf("¿Desea intentarlo de nuevo? (s/n): "); // ...preguntaremos si quiere intentar de nuevo.
      scanf(" %c", &continuar); // Leeremos la respuesta.
      limpiar_buffer(); // Limpiaremos el buffer de entrada.
      if (continuar != 's' && continuar != 'S') return; // Si no quiere continuar, terminaremos.
      continue; // Si quiere intentar, volveremos al inicio del bucle.
    }

    Song *cancion = song_pair->value; // Obtendremos la canción encontrada.
    list_pushBack(lista, cancion); // Agregaremos la canción a la lista de reproducción.
    printf("Canción '%s' agregada a la lista '%s'.\n", cancion->title, nombre_lista); // Informaremos que la canción se agregó.

    printf("¿Desea agregar otra canción a la lista '%s'? (s/n): ", nombre_lista); // Preguntaremos si quiere agregar otra canción.
    scanf(" %c", &continuar); // Leeremos la respuesta.
    limpiar_buffer(); // Limpiaremos el buffer de entrada.
  } while (continuar == 's' || continuar == 'S'); // Repetiremos si el usuario quiere continuar.
}

// En esta función, mostraremos las canciones de una lista de reproducción específica. Pediremos el nombre de la lista y listaremos los detalles de cada canción, como título, ID, tempo y género.

void mostrar_lista_reproduccion(Map *playlists) {
  listar_playlists(playlists); // Mostraremos todas las listas de reproducción disponibles.
  char nombre_lista[100]; // Prepararemos un espacio para el nombre de la lista que ingrese el usuario.
  if (!leer_cadena(nombre_lista, sizeof(nombre_lista), "\nIngrese el nombre de la lista de reproducción (o 'q' para cancelar): ", 3)) { // Pediremos el nombre de la lista.
    return; // Si no podemos leer el nombre, terminaremos la función.
  }

  MapPair *pair = map_search(playlists, nombre_lista); // Buscaremos la lista en el mapa de listas de reproducción.
  if (!pair) { // Si no encontramos la lista...
    printf("Error: No existe una lista de reproducción con el nombre '%s'.\n", nombre_lista); // ...informaremos al usuario.
    return; // ...y terminaremos la función.
  }

  List *lista = pair->value; // Obtendremos la lista de reproducción encontrada.
  if (!list_first(lista)) { // Si la lista está vacía...
    printf("La lista de reproducción '%s' está vacía.\n", nombre_lista); // ...informaremos al usuario.
    return; // ...y terminaremos la función.
  }

  printf("\nCanciones en la lista de reproducción '%s':\n", nombre_lista); // Mostraremos un encabezado con el nombre de la lista.
  int i = 1; // Llevaremos un conteo para numerar las canciones.
  Song *cancion = list_first(lista); // Tomaremos la primera canción de la lista para empezar a mostrarla.
  while (cancion) { // Mientras haya canciones por mostrar, continuaremos.
    printf("%d. %s (ID: %d, Tempo: %.2f, Género: %s)\n", i++, cancion->title, cancion->id, cancion->tempo, cancion->genre); // Mostraremos los detalles de la canción.
    cancion = list_next(lista); // Pasaremos a la siguiente canción.
  }
}

// En esta función, guardaremos una lista de reproducción en un archivo CSV. Exportaremos las canciones con sus detalles, como ID, título, tempo y género.

void guardar_lista_reproduccion(Map *playlists) {
  char nombre_lista[100]; // Prepararemos un espacio para el nombre de la lista que ingrese el usuario.
  if (!leer_cadena(nombre_lista, sizeof(nombre_lista), "Ingrese el nombre de la lista de reproducción a guardar (o 'q' para cancelar): ", 3)) { // Pediremos el nombre de la lista.
    return; // Si no podemos leer el nombre, terminaremos la función.
  }

  MapPair *pair = map_search(playlists, nombre_lista); // Buscaremos la lista en el mapa de listas de reproducción.
  if (!pair) { // Si no encontramos la lista...
    printf("Error: No existe una lista de reproducción con el nombre '%s'.\n", nombre_lista); // ...informaremos al usuario.
    return; // ...y terminaremos la función.
  }

  List *lista = pair->value; // Obtendremos la lista de reproducción encontrada.
  if (!list_first(lista)) { // Si la lista está vacía...
    printf("La lista de reproducción '%s' está vacía.\n", nombre_lista); // ...informaremos al usuario.
    return; // ...y terminaremos la función.
  }

  char nombre_archivo[150]; // Prepararemos un espacio para el nombre del archivo CSV.
  snprintf(nombre_archivo, sizeof(nombre_archivo), "%s.csv", nombre_lista); // Crearemos el nombre del archivo usando el nombre de la lista.
  FILE *archivo = fopen(nombre_archivo, "w"); // Abriremos el archivo en modo escritura.
  if (!archivo) { // Si no podemos abrir el archivo...
    perror("Error al abrir el archivo para guardar"); // ...mostraremos un mensaje de error.
    return; // ...y terminaremos la función.
  }

  fprintf(archivo, "ID,Title,Tempo,Género\n"); // Escribiremos un encabezado en el archivo CSV.
  Song *cancion = list_first(lista); // Tomaremos la primera canción de la lista para empezar a guardarla.
  while (cancion) { // Mientras haya canciones por guardar, continuaremos.
    fprintf(archivo, "%d,%s,%.2f,%s\n", cancion->id, cancion->title, cancion->tempo, cancion->genre); // Escribiremos los detalles de la canción en el archivo.
    cancion = list_next(lista); // Pasaremos a la siguiente canción.
  }

  fclose(archivo); // Cerraremos el archivo después de escribir todo.
  printf("Lista de reproducción '%s' guardada en el archivo '%s'.\n", nombre_lista, nombre_archivo); // Informaremos al usuario que la lista se guardó.
}

// En esta función, generaremos canciones aleatorias para pruebas. Crearemos canciones con datos variados y las agregaremos a los mapas, con la opción de incluirlas en una lista de reproducción.

void testear_datos_aleatorios(Map *canciones_by_id, Map *canciones_by_genero, Map *canciones_by_artista, Map **playlists) {
  srand(time(NULL)); // Inicializaremos el generador de números aleatorios.
  char *titulos[] = {"Song A", "Song B", "Song C", "Song D", "Song E"}; // Definiremos una lista de títulos base.
  char *artistas[] = {"Artist 1", "50 Cent", "*NSYNC", "Artist 4"}; // Definiremos una lista de artistas.
  char *albumes[] = {"Album X", "Album Y", "Album Z"}; // Definiremos una lista de álbumes.
  char *generos[] = {"Pop", "Rock", "Jazz", "Electronic", "Hip Hop"}; // Definiremos una lista de géneros.
  int num_titulos = 5, num_artistas = 4, num_albumes = 3, num_generos = 5; // Guardaremos el número de elementos en cada lista.

  int num_canciones; // Guardaremos el número de canciones a generar.
  if (!leer_tempo(&num_canciones, "Ingrese el número de canciones aleatorias a generar (o 'q' para cancelar): ", 3)) { // Pediremos al usuario cuántas canciones generar.
    return; // Si no podemos leer el número, terminaremos la función.
  }
  if (num_canciones < 0) { // Si el número es negativo...
    printf("Error: El número de canciones no puede ser negativo.\n"); // ...mostraremos un error.
    return; // ...y terminaremos la función.
  }

  int base_id = 1000; // Estableceremos un ID base para las canciones generadas.
  MapPair *pair = map_first(canciones_by_id); // Tomaremos la primera canción del mapa por ID.
  while (pair) { // Mientras haya canciones en el mapa, continuaremos.
    if (*(int *)pair->key >= base_id) { // Si el ID de la canción es mayor o igual al base...
      base_id = *(int *)pair->key + 1; // ...actualizaremos el base para evitar duplicados.
    }
    pair = map_next(canciones_by_id); // Pasaremos a la siguiente canción.
  }

  char agregar_lista; // Guardaremos si el usuario quiere agregar las canciones a una lista.
  char nombre_lista[100] = ""; // Prepararemos un espacio para el nombre de la lista.
  List *lista = NULL; // Prepararemos un espacio para la lista de reproducción.
  printf("¿Desea agregar las canciones generadas a una lista de reproducción? (s/n): "); // Preguntaremos si quiere agregar las canciones a una lista.
  scanf(" %c", &agregar_lista); // Leeremos la respuesta.
  limpiar_buffer(); // Limpiaremos el buffer de entrada.
  if (agregar_lista == 's' || agregar_lista == 'S') { // Si el usuario quiere agregar a una lista...
    if (!leer_cadena(nombre_lista, sizeof(nombre_lista), "Ingrese el nombre de la lista de reproducción (o 'q' para cancelar): ", 3)) { // ...pediremos el nombre de la lista.
      return; // Si no podemos leer el nombre, terminaremos la función.
    }

    pair = map_search(*playlists, nombre_lista); // Buscaremos si la lista ya existe.
    if (!pair) { // Si la lista no existe...
      lista = list_create(); // ...crearemos una nueva lista.
      if (!lista) { // Si no podemos crear la lista...
        printf("Error: No se pudo crear la lista de reproducción.\n"); // ...mostraremos un error.
        return; // ...y terminaremos la función.
      }
      char *nombre_key = strdup(nombre_lista); // Haremos una copia del nombre para el mapa.
      if (!nombre_key) { // Si no podemos crear la copia...
        list_clean(lista); // ...limpiaremos la lista.
        free(lista); // ...liberaremos la memoria de la lista.
        return; // ...y terminaremos la función.
      }
      map_insert(*playlists, nombre_key, lista); // Insertaremos la lista en el mapa de listas.
      printf("Lista de reproducción '%s' creada.\n", nombre_lista); // Informaremos que la lista se creó.
    } else { // Si la lista ya existe...
      lista = pair->value; // ...usaremos la lista existente.
    }
  }

  for (int i = 0; i < num_canciones; i++) { // Para cada canción que queremos generar, continuaremos.
    Song *song = malloc(sizeof(Song)); // Reservaremos memoria para una nueva canción.
    if (!song) { // Si no podemos reservar memoria...
      printf("Error: No se pudo asignar memoria para Song aleatoria %d.\n", i); // ...mostraremos un error.
      return; // ...y terminaremos la función.
    }

    song->id = base_id + i; // Asignaremos un ID único a la canción.
    snprintf(song->title, 100, "%s %d", titulos[rand() % num_titulos], song->id); // Crearemos un título combinando un título base y el ID.
    song->artists = list_create(); // Crearemos una lista para los artistas de la canción.
    if (!song->artists) { // Si no podemos crear la lista...
      free(song); // ...liberaremos la memoria de la canción.
      return; // ...y terminaremos la función.
    }
    int num_artistas_cancion = (rand() % 2) + 1; // Decidiremos cuántos artistas tendrá la canción (1 o 2).
    for (int j = 0; j < num_artistas_cancion; j++) { // Para cada artista, continuaremos.
      char *artista = artistas[rand() % num_artistas]; // Elegiremos un artista al azar.
      list_pushBack(song->artists, strdup(artista)); // Agregaremos una copia del nombre del artista a la lista.
    }
    snprintf(song->album, 100, "%s %d", albumes[rand() % num_albumes], song->id); // Crearemos un nombre de álbum combinando un álbum base y el ID.
    strcpy(song->genre, generos[rand() % num_generos]); // Escogeremos un género al azar.
    song->tempo = 60.0 + (rand() % 141); // Asignaremos un tempo aleatorio entre 60 y 200.

    int *id_key = malloc(sizeof(int)); // Reservaremos memoria para la clave del ID.
    if (!id_key) { // Si no podemos reservar memoria...
      list_clean(song->artists); // ...limpiaremos la lista de artistas.
      free(song->artists); // ...liberaremos la memoria de la lista.
      free(song); // ...liberaremos la memoria de la canción.
      return; // ...y terminaremos la función.
    }
    *id_key = song->id; // Guardaremos el ID en la clave.
    map_insert(canciones_by_id, id_key, song); // Insertaremos la canción en el mapa por ID.

    char *genre_key = strdup(song->genre); // Haremos una copia del género para usarla como clave.
    if (!genre_key) { // Si no podemos crear la copia...
      list_clean(song->artists); // ...limpiaremos la lista de artistas.
      free(song->artists); // ...liberaremos la memoria de la lista.
      free(song); // ...liberaremos la memoria de la canción.
      return; // ...y terminaremos la función.
    }
    pair = map_search(canciones_by_genero, genre_key); // Buscaremos el género en el mapa por género.
    if (!pair) { // Si el género no existe...
      List *list = list_create(); // ...crearemos una nueva lista para las canciones de este género.
      if (!list) { // Si no podemos crear la lista...
        free(genre_key); // ...liberaremos la clave del género.
        list_clean(song->artists); // ...limpiaremos la lista de artistas.
        free(song->artists); // ...liberaremos la memoria de la lista.
        free(song); // ...liberaremos la memoria de la canción.
        return; // ...y terminaremos la función.
      }
      list_pushBack(list, song); // Agregaremos la canción a la nueva lista.
      map_insert(canciones_by_genero, genre_key, list); // Insertaremos la lista en el mapa por género.
    } else { // Si el género ya existe...
      list_pushBack(pair->value, song); // ...agregaremos la canción a la lista existente.
      free(genre_key); // ...y liberaremos la copia del género.
    }

    char *artist = list_first(song->artists); // Tomaremos el primer artista de la canción.
    while (artist) { // Mientras haya artistas por procesar, continuaremos.
      MapPair *artist_pair = map_search(canciones_by_artista, artist); // Buscaremos el artista en el mapa por artista.
      if (!artist_pair) { // Si el artista no existe...
        List *list = list_create(); // ...crearemos una nueva lista para sus canciones.
        if (!list) { // Si no podemos crear la lista...
          list_clean(song->artists); // ...limpiaremos la lista de artistas.
          free(song->artists); // ...liberaremos la memoria de la lista.
          free(song); // ...liberaremos la memoria de la canción.
          return; // ...y terminaremos la función.
        }
        list_pushBack(list, song); // Agregaremos la canción a la nueva lista.
        char *artist_key = strdup(artist); // Haremos una copia del nombre del artista.
        if (!artist_key) { // Si no podemos crear la copia...
          list_clean(list); // ...limpiaremos la lista creada.
          free(list); // ...liberaremos la memoria de la lista.
          list_clean(song->artists); // ...limpiaremos la lista de artistas.
          free(song->artists); // ...liberaremos la memoria de la lista.
          free(song); // ...liberaremos la memoria de la canción.
          return; // ...y terminaremos la función.
        }
        map_insert(canciones_by_artista, artist_key, list); // Insertaremos la lista en el mapa por artista.
      } else { // Si el artista ya existe...
        list_pushBack(artist_pair->value, song); // ...agregaremos la canción a la lista existente.
      }
      artist = list_next(song->artists); // Pasaremos al siguiente artista.
    }

    if (agregar_lista == 's' || agregar_lista == 'S') { // Si el usuario quiso agregar a una lista...
      list_pushBack(lista, song); // ...agregaremos la canción a la lista de reproducción.
    }
  }

  printf("Se generaron %d canciones aleatorias.\n", num_canciones); // Informaremos cuántas canciones generamos.
  if (agregar_lista == 's' || agregar_lista == 'S' && nombre_lista[0] != '\0') { // Si se agregaron a una lista...
    printf("Las canciones fueron agregadas a la lista '%s'.\n", nombre_lista); // ...informaremos al usuario.
  }
}

// En esta función, liberaremos toda la memoria usada por los mapas de canciones, géneros, artistas y listas de reproducción. Nos aseguraremos de no dejar fugas de memoria.

void liberar_canciones(Map *canciones_by_id, Map *canciones_by_genero, Map *canciones_by_artista, Map *playlists) {
  MapPair *pair = map_first(canciones_by_id); // Tomaremos la primera canción del mapa por ID.
  while (pair) { // Mientras haya canciones por liberar, continuaremos.
    Song *song = pair->value; // Obtendremos la canción actual.
    char *artist = list_first(song->artists); // Tomaremos el primer artista de la canción.
    while (artist) { // Mientras haya artistas por liberar, continuaremos.
      free(artist); // Liberaremos la memoria del nombre del artista.
      artist = list_next(song->artists); // Pasaremos al siguiente artista.
    }
    list_clean(song->artists); // Limpiaremos la lista de artistas.
    free(song->artists); // Liberaremos la memoria de la lista de artistas.
    free(song); // Liberaremos la memoria de la canción.
    free(pair->key); // Liberaremos la memoria de la clave del ID.
    pair = map_next(canciones_by_id); // Pasaremos a la siguiente canción.
  }
  map_clean(canciones_by_id); // Limpiaremos el mapa por ID.
  free(canciones_by_id); // Liberaremos la memoria del mapa.

  pair = map_first(canciones_by_genero); // Tomaremos el primer género del mapa por género.
  while (pair) { // Mientras haya géneros por liberar, continuaremos.
    List *list = pair->value; // Obtendremos la lista de canciones del género.
    list_clean(list); // Limpiaremos la lista de canciones.
    free(pair->key); // Liberaremos la memoria de la clave del género.
    free(list); // Liberaremos la memoria de la lista.
    pair = map_next(canciones_by_genero); // Pasaremos al siguiente género.
  }
  map_clean(canciones_by_genero); // Limpiaremos el mapa por género.
  free(canciones_by_genero); // Liberaremos la memoria del mapa.

  pair = map_first(canciones_by_artista); // Tomaremos el primer artista del mapa por artista.
  while (pair) { // Mientras haya artistas por liberar, continuaremos.
    List *list = pair->value; // Obtendremos la lista de canciones del artista.
    list_clean(list); // Limpiaremos la lista de canciones.
    free(pair->key); // Liberaremos la memoria de la clave del artista.
    free(list); // Liberaremos la memoria de la lista.
    pair = map_next(canciones_by_artista); // Pasaremos al siguiente artista.
  }
  map_clean(canciones_by_artista); // Limpiaremos el mapa por artista.
  free(canciones_by_artista); // Liberaremos la memoria del mapa.

  pair = map_first(playlists); // Tomaremos la primera lista del mapa de listas de reproducción.
  while (pair) { // Mientras haya listas por liberar, continuaremos.
    List *list = pair->value; // Obtendremos la lista de reproducción.
    list_clean(list); // Limpiaremos la lista de canciones.
    free(pair->key); // Liberaremos la memoria de la clave del nombre.
    free(list); // Liberaremos la memoria de la lista.
    pair = map_next(playlists); // Pasaremos a la siguiente lista.
  }
  map_clean(playlists); // Limpiaremos el mapa de listas.
  free(playlists); // Liberaremos la memoria del mapa.
}

// En esta función principal, coordinaremos el programa. Mostraremos un menú interactivo, procesaremos las opciones del usuario y gestionaremos las canciones y listas de reproducción.

int main() {
  Map *canciones_by_id = map_create(is_equal_int); // Crearemos un mapa para canciones por ID.
  Map *canciones_by_genero = map_create(is_equal_str); // Crearemos un mapa para canciones por género.
  Map *canciones_by_artista = map_create(is_equal_str); // Crearemos un mapa para canciones por artista.
  Map *playlists = map_create(is_equal_str); // Crearemos un mapa para listas de reproducción.

  if (!canciones_by_id || !canciones_by_genero || !canciones_by_artista || !playlists) { // Si no podemos crear alguno de los mapas...
    printf("Error al crear mapas.\n"); // ...mostraremos un error.
    if (canciones_by_id) { map_clean(canciones_by_id); free(canciones_by_id); } // Liberaremos el mapa por ID si existe.
    if (canciones_by_genero) { map_clean(canciones_by_genero); free(canciones_by_genero); } // Liberaremos el mapa por género si existe.
    if (canciones_by_artista) { map_clean(canciones_by_artista); free(canciones_by_artista); } // Liberaremos el mapa por artista si existe.
    if (playlists) { map_clean(playlists); free(playlists); } // Liberaremos el mapa de listas si existe.
    return 1; // Terminaremos el programa con un error.
  }

  leer_canciones(canciones_by_id, canciones_by_genero, canciones_by_artista); // Cargaremos las canciones desde el archivo CSV.

  if (!map_first(canciones_by_id)) { // Si no se cargaron canciones...
    printf("Advertencia: No se cargaron canciones. Algunas opciones pueden no funcionar.\n"); // ...advertiremos al usuario.
  }

  int opcion = 0; // Guardaremos la opción que elija el usuario.
  while (opcion != 9) { // Mientras el usuario no elija salir, continuaremos.
    printf("\nMenu:\n"); // Mostraremos el menú de opciones.
    printf("1. Buscar por artista\n"); // Opción para buscar por artista.
    printf("2. Buscar por género\n"); // Opción para buscar por género.
    printf("3. Buscar por tempo\n"); // Opción para buscar por tempo.
    printf("4. Crear nueva lista de reproducción\n"); // Opción para crear una lista.
    printf("5. Agregar canción a lista de reproducción\n"); // Opción para agregar canciones a una lista.
    printf("6. Mostrar canciones de una lista de reproducción\n"); // Opción para mostrar una lista.
    printf("7. Guardar lista de reproducción\n"); // Opción para guardar una lista.
    printf("8. Testear con datos aleatorios\n"); // Opción para generar datos de prueba.
    printf("9. Salir\n"); // Opción para salir del programa.
    if (!leer_numero(&opcion, "Seleccione una opción (1-9, o 'q' para salir): ", 3)) { // Pediremos al usuario que elija una opción.
      continue; // Si no podemos leer la opción, volveremos al inicio del bucle.
    }
    if (opcion < 1 || opcion > 9) { // Si la opción no es válida...
      printf("Error: Seleccione una opción válida (1-9).\n"); // ...mostraremos un error.
      continue; // ...y volveremos al inicio del bucle.
    }
    switch (opcion) { // Según la opción elegida, ejecutaremos una función.
      case 1: buscar_por_artista(canciones_by_artista); break; // Buscaremos por artista.
      case 2: buscar_por_genero(canciones_by_genero); break; // Buscaremos por género.
      case 3: buscar_por_tempo(canciones_by_id); break; // Buscaremos por tempo.
      case 4: crear_lista_reproduccion(playlists); break; // Crearemos una nueva lista.
      case 5: agregar_cancion_a_lista(canciones_by_id, playlists); break; // Agregaremos canciones a una lista.
      case 6: mostrar_lista_reproduccion(playlists); break; // Mostraremos una lista.
      case 7: guardar_lista_reproduccion(playlists); break; // Guardaremos una lista.
      case 8: testear_datos_aleatorios(canciones_by_id, canciones_by_genero, canciones_by_artista, &playlists); break; // Generaremos datos aleatorios.
      case 9: printf("Saliendo...\n"); break; // Informaremos que el programa está terminando.
    }
    if (opcion != 9) { // Si no estamos saliendo...
      presioneTeclaParaContinuar(); // ...esperaremos que el usuario presione una tecla.
      limpiarPantalla(); // ...y limpiaremos la pantalla para la próxima iteración.
    }
  }

  liberar_canciones(canciones_by_id, canciones_by_genero, canciones_by_artista, playlists); // Liberaremos toda la memoria usada por los mapas.
  return 0; // Terminaremos el programa exitosamente.
}