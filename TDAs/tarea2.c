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
  ArtistaInfo *info = list_first(artistas_list);
  while (info) {
    free(info);
    info = list_next(artistas_list);
  }
  list_clean(artistas_list);
  free(artistas_list);
}

// Ahora crearemos una función que filtra artistas por su letra inicial o por la categoría "Otros" (nombres que no comienzan con letra). Nos ayudará a organizar los artistas en la búsqueda por letra.
void filtrar_artistas_por_letra(List *artistas_list, char letra, List *filtrados, List *otros) {
  ArtistaInfo *info = list_first(artistas_list);
  while (info) {
    char inicial = toupper(info->artista[0]);
    if (letra == '#') {
      // Filtrar para "Otros" (nombres que no comienzan con A-Z)
      if (!isalpha(inicial)) {
        list_pushBack(otros, info);
      }
    } else {
      // Filtrar por letra específica
      if (inicial == letra) {
        list_pushBack(filtrados, info);
      } else if (!isalpha(inicial)) {
        list_pushBack(otros, info);
      }
    }
    info = list_next(artistas_list);
  }
}

// Aquí implementaremos una función que lee canciones desde un archivo CSV y las organiza en mapas por ID, género y artista. Es el núcleo de la carga de datos, permitiendo que el programa acceda a las canciones de manera eficiente.
void leer_canciones(Map *canciones_by_id, Map *canciones_by_genero, Map *canciones_by_artista) {
  FILE *archivo = fopen("C:\\Users\\victor\\OneDrive\\Desktop\\tarea2\\TDAs\\song_dataset_.csv", "r");
  if (!archivo) {
    perror("Error al abrir el archivo");
    return;
  }

  char **campos = leer_linea_csv(archivo, ',');
  if (!campos) {
    printf("Error: No se pudieron leer los encabezados del CSV.\n");
    fclose(archivo);
    return;
  }

  int k = 0;
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    if (k++ > 60000) break;
    if (k % 10000 == 0) {
      printf("Procesadas %d canciones...\n", k);
    }

    if (!campos[0] || !campos[2] || !campos[3] || !campos[4] || !campos[18] || !campos[20]) {
      continue;
    }

    Song *song = malloc(sizeof(Song));
    if (!song) {
      fclose(archivo);
      return;
    }

    song->id = atoi(campos[0]);
    strcpy(song->title, campos[4][0] ? campos[4] : "Sin título");
    song->artists = split_string(campos[2][0] ? campos[2] : "", ";");
    if (!song->artists) {
      free(song);
      continue;
    }
    strcpy(song->album, campos[3][0] ? campos[3] : "Sin álbum");
    strcpy(song->genre, campos[20][0] ? campos[20] : "Desconocido");
    song->tempo = atof(campos[18]);

    int *id_key = malloc(sizeof(int));
    if (!id_key) {
      list_clean(song->artists);
      free(song->artists);
      free(song);
      fclose(archivo);
      return;
    }
    *id_key = song->id;
    map_insert(canciones_by_id, id_key, song);

    char *genre_key = strdup(song->genre);
    if (!genre_key) {
      free(id_key);
      list_clean(song->artists);
      free(song->artists);
      free(song);
      fclose(archivo);
      return;
    }
    MapPair *pair = map_search(canciones_by_genero, genre_key);
    if (!pair) {
      List *list = list_create();
      if (!list) {
        free(genre_key);
        free(id_key);
        list_clean(song->artists);
        free(song->artists);
        free(song);
        fclose(archivo);
        return;
      }
      list_pushBack(list, song);
      map_insert(canciones_by_genero, genre_key, list);
    } else {
      list_pushBack(pair->value, song);
      free(genre_key);
    }

    char *artist = list_first(song->artists);
    while (artist) {
      MapPair *artist_pair = map_search(canciones_by_artista, artist);
      if (!artist_pair) {
        List *list = list_create();
        if (!list) {
          free(id_key);
          list_clean(song->artists);
          free(song->artists);
          free(song);
          fclose(archivo);
          return;
        }
        list_pushBack(list, song);
        char *artist_key = strdup(artist);
        if (!artist_key) {
          list_clean(list);
          free(list);
          free(id_key);
          list_clean(song->artists);
          free(song->artists);
          free(song);
          fclose(archivo);
          return;
        }
        map_insert(canciones_by_artista, artist_key, list);
      } else {
        list_pushBack(artist_pair->value, song);
      }
      artist = list_next(song->artists);
    }
  }

  printf("Total de canciones cargadas: %d\n", k);
  fclose(archivo);
}

// Ahora vamos a crear una función que permite buscar canciones por artista. Pedirá una letra inicial, mostrará artistas disponibles, y luego dejará al usuario seleccionar uno para ver sus canciones.
void buscar_por_artista(Map *canciones_by_artista) {
  List *artistas_list = recolectar_artistas(canciones_by_artista);
  if (!artistas_list) {
    printf("Error: No se pudo recolectar la lista de artistas.\n");
    return;
  }

  char letra_input;
  printf("\nIngrese la letra inicial del artista (A-Z, '#' para Otros, o 'q' para cancelar): ");
  if (!leer_caracter(&letra_input, "", 3)) {
    liberar_artistas_list(artistas_list);
    return;
  }
  if (letra_input != '#' && !isalpha(letra_input)) {
    printf("Error: Seleccione una letra de A-Z o '#' para Otros.\n");
    liberar_artistas_list(artistas_list);
    return;
  }

  List *filtrados = list_create();
  List *otros = list_create();
  if (!filtrados || !otros) {
    printf("Error: No se pudo crear listas para filtrado.\n");
    if (filtrados) list_clean(filtrados);
    if (otros) list_clean(otros);
    free(filtrados);
    free(otros);
    liberar_artistas_list(artistas_list);
    return;
  }

  filtrar_artistas_por_letra(artistas_list, letra_input, filtrados, otros);

  if (letra_input == '#') {
    printf("\nArtistas en la categoría 'Otros':\n");
    mostrar_artistas_ordenados(otros);
  } else {
    printf("\nArtistas que comienzan con '%c':\n", letra_input);
    mostrar_artistas_ordenados(filtrados);
  }

  if (list_size(filtrados) == 0 && (letra_input != '#' || list_size(otros) == 0)) {
    printf("No hay artistas disponibles para la selección.\n");
    list_clean(filtrados);
    list_clean(otros);
    free(filtrados);
    free(otros);
    liberar_artistas_list(artistas_list);
    return;
  }

  char artista[100];
  if (!leer_cadena(artista, sizeof(artista), "\nIngrese el nombre del artista (o 'q' para cancelar): ", 3)) {
    list_clean(filtrados);
    list_clean(otros);
    free(filtrados);
    free(otros);
    liberar_artistas_list(artistas_list);
    return;
  }

  MapPair *par = map_search(canciones_by_artista, artista);
  if (!par) {
    printf("No se encontraron canciones para el artista '%s'.\n", artista);
    list_clean(filtrados);
    list_clean(otros);
    free(filtrados);
    free(otros);
    liberar_artistas_list(artistas_list);
    return;
  }

  mostrar_canciones(par->value, artista);
  list_clean(filtrados);
  list_clean(otros);
  free(filtrados);
  free(otros);
  liberar_artistas_list(artistas_list);
}

// Aquí implementaremos una función que valida si un género ingresado por el usuario existe en el mapa de géneros. Nos ayudará a asegurar que la búsqueda por género sea válida antes de continuar.
int validar_genero(Map *canciones_by_genero, char *genero_input, MapPair **par) {
  if (!es_cadena_valida(genero_input)) {
    printf("Error: Género inválido.\n");
    return 0;
  }

  *par = map_search(canciones_by_genero, genero_input);
  if (!*par) {
    printf("No se encontraron canciones para el género '%s'.\n", genero_input);
    return 0;
  }
  return 1;
}

// Ahora crearemos una función que filtra canciones por la letra inicial de su título. Nos permitirá mostrar solo las canciones que comienzan con una letra específica dentro de un género.
List *filtrar_canciones_por_letra(List *canciones, char letra) {
  List *filtradas = list_create();
  Song *cancion = list_first(canciones);
  while (cancion) {
    if (toupper(cancion->title[0]) == letra) {
      list_pushBack(filtradas, cancion);
    }
    cancion = list_next(canciones);
  }
  return filtradas;
}

// Aquí vamos a implementar una función que guarda las canciones filtradas en un archivo de texto. Nos ayudará a exportar los resultados de la búsqueda por género, con un formato claro y legible.
void guardar_canciones_filtradas(List *filtradas, const char *genero, char letra, const char *nombre_archivo) {
  FILE *archivo = fopen(nombre_archivo, "w");
  if (!archivo) {
    printf("Error al crear el archivo '%s'.\n", nombre_archivo);
    return;
  }

  fprintf(archivo, "Canciones del género '%s' que comienzan con '%c':\n", genero, letra);
  Song *cancion = list_first(filtradas);
  while (cancion) {
    fprintf(archivo, "%s (ID: %d)\n", cancion->title, cancion->id);
    cancion = list_next(filtradas);
  }

  fclose(archivo);
  printf("Canciones filtradas guardadas en '%s'.\n", nombre_archivo);
}

// Ahora implementaremos una función que permite buscar canciones por género. Mostrará los géneros disponibles, pedirá un género y una letra inicial, y guardará las canciones filtradas en un archivo.
void buscar_por_genero(Map *canciones_by_genero) {
  List *generos_list = list_create();
  if (!generos_list) {
    printf("Error: No se pudo crear lista de géneros.\n");
    return;
  }

  size_t max_len = 0;
  int num_generos = 0;
  MapPair *pair = map_first(canciones_by_genero);
  while (pair) {
    char *genero = pair->key;
    int is_valid = 1;
    for (int i = 0; genero[i]; i++) {
      if (!isalpha(genero[i]) && genero[i] != '-' && genero[i] != ' ') {
        is_valid = 0;
        break;
      }
    }
    if (is_valid) {
      GeneroInfo *info = malloc(sizeof(GeneroInfo));
      if (!info) {
        list_clean(generos_list);
        free(generos_list);
        return;
      }
      info->genero = genero;
      info->num_canciones = list_size(pair->value);
      list_pushBack(generos_list, info);
      size_t len = strlen(info->genero) + 4 + (info->num_canciones / 10 + 1);
      if (len > max_len) max_len = len;
      num_generos++;
    }
    pair = map_next(canciones_by_genero);
  }

  if (num_generos == 0) {
    printf("No hay géneros válidos disponibles.\n");
    list_clean(generos_list);
    free(generos_list);
    return;
  }

  printf("\nGéneros disponibles:\n");
  int cols = 4, col_width = max_len + 2 > 12 ? max_len + 2 : 12;
  int rows = (num_generos + cols - 1) / cols;
  GeneroInfo *info = list_first(generos_list);
  int index = 0;
  for (int i = 0; i < rows && index < num_generos; i++) {
    for (int j = 0; j < cols && index < num_generos; j++) {
      printf(" %-s (%d)", info->genero, info->num_canciones);
      int len = strlen(info->genero) + (info->num_canciones / 10 + 1) + 4;
      for (int k = len; k < col_width; k++) printf(" ");
      info = list_next(generos_list);
      index++;
    }
    printf("\n");
  }

  char genero_input[100];
  if (!leer_cadena(genero_input, sizeof(genero_input), "\nIngrese el género a buscar (o 'q' para cancelar): ", 3)) {
    goto limpiar;
  }

  MapPair *par;
  if (!validar_genero(canciones_by_genero, genero_input, &par)) {
    goto limpiar;
  }

  char letra_input;
  if (!leer_caracter(&letra_input, "Ingrese la letra inicial (A-Z, o 'q' para cancelar): ", 3)) {
    goto limpiar;
  }

  List *filtradas = filtrar_canciones_por_letra(par->value, letra_input);
  int count = list_size(filtradas);
  if (count == 0) {
    printf("\nNo se encontraron canciones que comiencen con '%c' en el género '%s'.\n", letra_input, genero_input);
    list_clean(filtradas);
    free(filtradas);
    goto limpiar;
  }

  printf("\nCanciones del género '%s' que comienzan con '%c':\n", genero_input, letra_input);
  int songs_per_row = 4, current = 0;
  Song *cancion = list_first(filtradas);
  while (cancion) {
    printf(" %-s (ID: %d)", cancion->title, cancion->id);
    current++;
    if (current % songs_per_row == 0) printf("\n");
    else printf(" |");
    cancion = list_next(filtradas);
  }
  if (current % songs_per_row != 0) printf("\n");

  char archivo_nombre[100];
  snprintf(archivo_nombre, sizeof(archivo_nombre), "%c_canciones.txt", letra_input);
  guardar_canciones_filtradas(filtradas, genero_input, letra_input, archivo_nombre);

  list_clean(filtradas);
  free(filtradas);

limpiar:
  info = list_first(generos_list);
  while (info) {
    free(info);
    info = list_next(generos_list);
  }
  list_clean(generos_list);
  free(generos_list);
}

// Aquí crearemos una función que permite al usuario crear una nueva lista de reproducción. Pedirá un nombre y verificará que no exista antes, asegurando que la lista se cree correctamente.
void crear_lista_reproduccion(Map *playlists) {
  char nombre_lista[100];
  if (!leer_cadena(nombre_lista, sizeof(nombre_lista), "Ingrese el nombre de la nueva lista de reproducción (o 'q' para cancelar): ", 3)) {
    return;
  }

  MapPair *pair = map_search(playlists, nombre_lista);
  if (pair) {
    printf("Error: Ya existe una lista de reproducción con el nombre '%s'.\n", nombre_lista);
    return;
  }

  List *lista = list_create();
  if (!lista) {
    printf("Error: No se pudo crear la lista de reproducción.\n");
    return;
  }

  char *nombre_key = strdup(nombre_lista);
  if (!nombre_key) {
    list_clean(lista);
    free(lista);
    return;
  }
  map_insert(playlists, nombre_key, lista);
  printf("Lista de reproducción '%s' creada exitosamente.\n", nombre_lista);
}

// Ahora implementaremos una función que muestra todas las listas de reproducción disponibles. Nos ayudará a informar al usuario qué listas existen antes de realizar acciones como agregar canciones o guardarlas.
void listar_playlists(Map *playlists) {
  printf("\nListas de reproducción disponibles:\n");
  MapPair *pair = map_first(playlists);
  if (!pair) {
    printf("No hay listas de reproducción creadas.\n");
    return;
  }
  int i = 1;
  while (pair) {
    printf("%d. %s\n", i++, (char *)pair->key);
    pair = map_next(playlists);
  }
}

// Aquí vamos a crear una función que permite agregar canciones a una lista de reproducción existente. Pedirá el nombre de la lista y los IDs de las canciones, validando cada entrada para evitar errores.
void agregar_cancion_a_lista(Map *canciones_by_id, Map *playlists) {
  listar_playlists(playlists);
  char nombre_lista[100];
  if (!leer_cadena(nombre_lista, sizeof(nombre_lista), "\nIngrese el nombre de la lista de reproducción (o 'q' para cancelar): ", 3)) {
    return;
  }

  MapPair *pair = map_search(playlists, nombre_lista);
  if (!pair) {
    printf("Error: No existe una lista de reproducción con el nombre '%s'.\n", nombre_lista);
    return;
  }
  List *lista = pair->value;

  char id_cancion[100];
  char continuar;
  do {
    if (!leer_cadena(id_cancion, sizeof(id_cancion), "\nIngrese el ID de la canción (o 'q' para cancelar): ", 3)) {
      return;
    }

    if (!es_numero_valido(id_cancion)) {
      printf("Error: ID de canción inválido.\n");
      printf("¿Desea intentarlo de nuevo? (s/n): ");
      scanf(" %c", &continuar);
      limpiar_buffer();
      if (continuar != 's' && continuar != 'S') return;
      continue;
    }

    int id = atoi(id_cancion);
    MapPair *song_pair = map_search(canciones_by_id, &id);
    if (!song_pair) {
      printf("Error: No se encontró una canción con el ID '%s'.\n", id_cancion);
      printf("¿Desea intentarlo de nuevo? (s/n): ");
      scanf(" %c", &continuar);
      limpiar_buffer();
      if (continuar != 's' && continuar != 'S') return;
      continue;
    }

    Song *cancion = song_pair->value;
    list_pushBack(lista, cancion);
    printf("Canción '%s' agregada a la lista '%s'.\n", cancion->title, nombre_lista);

    printf("¿Desea agregar otra canción a la lista '%s'? (s/n): ", nombre_lista);
    scanf(" %c", &continuar);
    limpiar_buffer();
  } while (continuar == 's' || continuar == 'S');
}

// Ahora crearemos una función que muestra las canciones de una lista de reproducción específica. Pedirá el nombre de la lista y mostrará los detalles de cada canción, como título, ID, tempo y género.
void mostrar_lista_reproduccion(Map *playlists) {
  listar_playlists(playlists);
  char nombre_lista[100];
  if (!leer_cadena(nombre_lista, sizeof(nombre_lista), "\nIngrese el nombre de la lista de reproducción (o 'q' para cancelar): ", 3)) {
    return;
  }

  MapPair *pair = map_search(playlists, nombre_lista);
  if (!pair) {
    printf("Error: No existe una lista de reproducción con el nombre '%s'.\n", nombre_lista);
    return;
  }

  List *lista = pair->value;
  if (!list_first(lista)) {
    printf("La lista de reproducción '%s' está vacía.\n", nombre_lista);
    return;
  }

  printf("\nCanciones en la lista de reproducción '%s':\n", nombre_lista);
  int i = 1;
  Song *cancion = list_first(lista);
  while (cancion) {
    printf("%d. %s (ID: %d, Tempo: %.2f, Género: %s)\n", 
          i++, cancion->title, cancion->id, cancion->tempo, cancion->genre);
    cancion = list_next(lista);
  }
}

// Aquí implementaremos una función que guarda una lista de reproducción en un archivo CSV. Nos permitirá exportar las canciones de una lista con sus detalles, como ID, título, tempo y género.
void guardar_lista_reproduccion(Map *playlists) {
  char nombre_lista[100];
  if (!leer_cadena(nombre_lista, sizeof(nombre_lista), "Ingrese el nombre de la lista de reproducción a guardar (o 'q' para cancelar): ", 3)) {
    return;
  }

  MapPair *pair = map_search(playlists, nombre_lista);
  if (!pair) {
    printf("Error: No existe una lista de reproducción con el nombre '%s'.\n", nombre_lista);
    return;
  }

  List *lista = pair->value;
  if (!list_first(lista)) {
    printf("La lista de reproducción '%s' está vacía.\n", nombre_lista);
    return;
  }

  char nombre_archivo[150];
  snprintf(nombre_archivo, sizeof(nombre_archivo), "%s.csv", nombre_lista);
  FILE *archivo = fopen(nombre_archivo, "w");
  if (!archivo) {
    perror("Error al abrir el archivo para guardar");
    return;
  }

  fprintf(archivo, "ID,Title,Tempo,Género\n");
  Song *cancion = list_first(lista);
  while (cancion) {
    fprintf(archivo, "%d,%s,%.2f,%s\n", cancion->id, cancion->title, cancion->tempo, cancion->genre);
    cancion = list_next(lista);
  }

  fclose(archivo);
  printf("Lista de reproducción '%s' guardada en el archivo '%s'.\n", nombre_lista, nombre_archivo);
}

// Ahora vamos a crear una función que genera datos aleatorios de canciones para pruebas. Nos permitirá simular un conjunto de canciones con artistas, géneros y tempos variados, útil para depurar el programa.
void testear_datos_aleatorios(Map *canciones_by_id, Map *canciones_by_genero, Map *canciones_by_artista, Map **playlists) {
  srand(time(NULL));
  char *titulos[] = {"Song A", "Song B", "Song C", "Song D", "Song E"};
  char *artistas[] = {"Artist 1", "50 Cent", "*NSYNC", "Artist 4"};
  char *albumes[] = {"Album X", "Album Y", "Album Z"};
  char *generos[] = {"Pop", "Rock", "Jazz", "Electronic", "Hip Hop"};
  int num_titulos = 5, num_artistas = 4, num_albumes = 3, num_generos = 5;

  int num_canciones;
  if (!leer_tempo(&num_canciones, "Ingrese el número de canciones aleatorias a generar (o 'q' para cancelar): ", 3)) {
    return;
  }
  if (num_canciones < 0) {
    printf("Error: El número de canciones no puede ser negativo.\n");
    return;
  }

  int base_id = 1000;
  MapPair *pair = map_first(canciones_by_id);
  while (pair) {
    if (*(int *)pair->key >= base_id) {
      base_id = *(int *)pair->key + 1;
    }
    pair = map_next(canciones_by_id);
  }

  char agregar_lista;
  char nombre_lista[100] = "";
  List *lista = NULL;
  printf("¿Desea agregar las canciones generadas a una lista de reproducción? (s/n): ");
  scanf(" %c", &agregar_lista);
  limpiar_buffer();
  if (agregar_lista == 's' || agregar_lista == 'S') {
    if (!leer_cadena(nombre_lista, sizeof(nombre_lista), "Ingrese el nombre de la lista de reproducción (o 'q' para cancelar): ", 3)) {
      return;
    }

    pair = map_search(*playlists, nombre_lista);
    if (!pair) {
      lista = list_create();
      if (!lista) {
        printf("Error: No se pudo crear la lista de reproducción.\n");
        return;
      }
      char *nombre_key = strdup(nombre_lista);
      if (!nombre_key) {
        list_clean(lista);
        free(lista);
        return;
      }
      map_insert(*playlists, nombre_key, lista);
      printf("Lista de reproducción '%s' creada.\n", nombre_lista);
    } else {
      lista = pair->value;
    }
  }

  for (int i = 0; i < num_canciones; i++) {
    Song *song = malloc(sizeof(Song));
    if (!song) {
      printf("Error: No se pudo asignar memoria para Song aleatoria %d.\n", i);
      return;
    }

    song->id = base_id + i;
    snprintf(song->title, 100, "%s %d", titulos[rand() % num_titulos], song->id);
    song->artists = list_create();
    if (!song->artists) {
      free(song);
      return;
    }
    int num_artistas_cancion = (rand() % 2) + 1;
    for (int j = 0; j < num_artistas_cancion; j++) {
      char *artista = artistas[rand() % num_artistas];
      list_pushBack(song->artists, strdup(artista));
    }
    snprintf(song->album, 100, "%s %d", albumes[rand() % num_albumes], song->id);
    strcpy(song->genre, generos[rand() % num_generos]);
    song->tempo = 60.0 + (rand() % 141);

    int *id_key = malloc(sizeof(int));
    if (!id_key) {
      list_clean(song->artists);
      free(song->artists);
      free(song);
      return;
    }
    *id_key = song->id;
    map_insert(canciones_by_id, id_key, song);

    char *genre_key = strdup(song->genre);
    if (!genre_key) {
      list_clean(song->artists);
      free(song->artists);
      free(song);
      return;
    }
    pair = map_search(canciones_by_genero, genre_key);
    if (!pair) {
      List *list = list_create();
      if (!list) {
        free(genre_key);
        list_clean(song->artists);
        free(song->artists);
        free(song);
        return;
      }
      list_pushBack(list, song);
      map_insert(canciones_by_genero, genre_key, list);
    } else {
      list_pushBack(pair->value, song);
      free(genre_key);
    }

    char *artist = list_first(song->artists);
    while (artist) {
      MapPair *artist_pair = map_search(canciones_by_artista, artist);
      if (!artist_pair) {
        List *list = list_create();
        if (!list) {
          list_clean(song->artists);
          free(song->artists);
          free(song);
          return;
        }
        list_pushBack(list, song);
        char *artist_key = strdup(artist);
        if (!artist_key) {
          list_clean(list);
          free(list);
          list_clean(song->artists);
          free(song->artists);
          free(song);
          return;
        }
        map_insert(canciones_by_artista, artist_key, list);
      } else {
        list_pushBack(artist_pair->value, song);
      }
      artist = list_next(song->artists);
    }

    if (agregar_lista == 's' || agregar_lista == 'S') {
      list_pushBack(lista, song);
    }
  }

  printf("Se generaron %d canciones aleatorias.\n", num_canciones);
  if (agregar_lista == 's' || agregar_lista == 'S' && nombre_lista[0] != '\0') {
    printf("Las canciones fueron agregadas a la lista '%s'.\n", nombre_lista);
  }
}

// Aquí crearemos una función que libera toda la memoria utilizada por los mapas de canciones, géneros, artistas y listas de reproducción. Nos asegurará que no haya fugas de memoria al cerrar el programa.
void liberar_canciones(Map *canciones_by_id, Map *canciones_by_genero, Map *canciones_by_artista, Map *playlists) {
  MapPair *pair = map_first(canciones_by_id);
  while (pair) {
    Song *song = pair->value;
    char *artist = list_first(song->artists);
    while (artist) {
      free(artist);
      artist = list_next(song->artists);
    }
    list_clean(song->artists);
    free(song->artists);
    free(song);
    free(pair->key);
    pair = map_next(canciones_by_id);
  }
  map_clean(canciones_by_id);
  free(canciones_by_id);

  pair = map_first(canciones_by_genero);
  while (pair) {
    List *list = pair->value;
    list_clean(list);
    free(pair->key);
    free(list);
    pair = map_next(canciones_by_genero);
  }
  map_clean(canciones_by_genero);
  free(canciones_by_genero);

  pair = map_first(canciones_by_artista);
  while (pair) {
    List *list = pair->value;
    list_clean(list);
    free(pair->key);
    free(list);
    pair = map_next(canciones_by_artista);
  }
  map_clean(canciones_by_artista);
  free(canciones_by_artista);

  pair = map_first(playlists);
  while (pair) {
    List *list = pair->value;
    list_clean(list);
    free(pair->key);
    free(list);
    pair = map_next(playlists);
  }
  map_clean(playlists);
  free(playlists);
}

// Ahora implementaremos la función principal del programa. Nos permitirá mostrar un menú interactivo, procesar las opciones del usuario y coordinar las demás funciones para gestionar las canciones y listas de reproducción.
int main() {
  Map *canciones_by_id = map_create(is_equal_int);
  Map *canciones_by_genero = map_create(is_equal_str);
  Map *canciones_by_artista = map_create(is_equal_str);
  Map *playlists = map_create(is_equal_str);

  if (!canciones_by_id || !canciones_by_genero || !canciones_by_artista || !playlists) {
    printf("Error al crear mapas.\n");
    if (canciones_by_id) { map_clean(canciones_by_id); free(canciones_by_id); }
    if (canciones_by_genero) { map_clean(canciones_by_genero); free(canciones_by_genero); }
    if (canciones_by_artista) { map_clean(canciones_by_artista); free(canciones_by_artista); }
    if (playlists) { map_clean(playlists); free(playlists); }
    return 1;
  }

  leer_canciones(canciones_by_id, canciones_by_genero, canciones_by_artista);

  if (!map_first(canciones_by_id)) {
    printf("Advertencia: No se cargaron canciones. Algunas opciones pueden no funcionar.\n");
  }

  int opcion = 0;
  while (opcion != 9) {
    printf("\nMenu:\n");
    printf("1. Buscar por artista\n");
    printf("2. Buscar por género\n");
    printf("3. Buscar por tempo\n");
    printf("4. Crear nueva lista de reproducción\n");
    printf("5. Agregar canción a lista de reproducción\n");
    printf("6. Mostrar canciones de una lista de reproducción\n");
    printf("7. Guardar lista de reproducción\n");
    printf("8. Testear con datos aleatorios\n");
    printf("9. Salir\n");
    if (!leer_numero(&opcion, "Seleccione una opción (1-9, o 'q' para salir): ", 3)) {
      continue;
    }
    if (opcion < 1 || opcion > 9) {
      printf("Error: Seleccione una opción válida (1-9).\n");
      continue;
    }
    switch (opcion) {
      case 1: buscar_por_artista(canciones_by_artista); break;
      case 2: buscar_por_genero(canciones_by_genero); break;
      case 3: buscar_por_tempo(canciones_by_id); break;
      case 4: crear_lista_reproduccion(playlists); break;
      case 5: agregar_cancion_a_lista(canciones_by_id, playlists); break;
      case 6: mostrar_lista_reproduccion(playlists); break;
      case 7: guardar_lista_reproduccion(playlists); break;
      case 8: testear_datos_aleatorios(canciones_by_id, canciones_by_genero, canciones_by_artista, &playlists); break;
      case 9: printf("Saliendo...\n"); break;
    }
    if (opcion != 9) {
      presioneTeclaParaContinuar();
      limpiarPantalla();
    }
  }

  liberar_canciones(canciones_by_id, canciones_by_genero, canciones_by_artista, playlists);
  return 0;
}