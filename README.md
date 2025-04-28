Tarea 2: Se ha creado un pequeño modelo de lo que seria un "Sistema de Gestión de Canciones"

Descripción:

Tarea 2 es un programa en C que implementa un sistema interactivo para gestionar una base de datos de canciones.
Permite cargar canciones desde un archivo CSV, buscarlas por artista, género o tempo, crear y gestionar listas de reproducción,
y generar datos aleatorios para pruebas. El programa utiliza estructuras de datos avanzadas (listas y mapas) definidas en los TDAs (list.h, map.h, extra.h)
para organizar y buscar información eficientemente.

El programa ademas nos ofrece un menú interactivo con 9 opciones:

1)Buscar canciones por artista.
2)Buscar canciones por género.
3)Buscar canciones por tempo.
4)Crear una nueva lista de reproducción.
5)Agregar canciones a una lista de reproducción.
6)Mostrar canciones de una lista de reproducción.
7)Guardar una lista de reproducción en un archivo CSV.
8)Generar canciones aleatorias para pruebas.
9)Salir del programa.


Este README explica cómo instalar, ejecutar y usar el programa, con ejemplos detallados para cada funcionalidad.

Debido a la cantidad de datos que arroja el DataFrame que usamos para estudiar la viabilidad de este programa, se han ademas asignado al codigo funciones que permiten
dar un orden y presentacion de los datos de una forma mas prolija y organizada.

Requisitos
Sistema Operativo: Windows (probado en Windows 10).
Visual Code Studio o cualquier entorno que permita trabajar con C o Compilador: GCC (MinGW o similar).
Archivos Necesarios:
TDAs/tarea2.c: Código fuente principal.
TDAs/tdas/list.c, TDAs/tdas/list.h: Implementación de listas.
TDAs/tdas/map.c, TDAs/tdas/map.h: Implementación de mapas.
TDAs/tdas/extra.c, TDAs/tdas/extra.h: Funciones auxiliares para CSV.
TDAs/song_dataset_.csv: Archivo CSV con datos de canciones (ubicado en C:\Users\<tu_usuario>\OneDrive\Desktop\tarea2\TDAs\).

Instalación
Clona o descarga el proyecto:

Asegúrate de que todos los archivos estén en la carpeta:
C:\Users\<tu_usuario>\OneDrive\Desktop\tarea2\
La estructura debe ser:
tarea2/
├── TDAs/
│   ├── tarea2.c
│   ├── song_dataset_.csv
│   ├── tdas/
│   │   ├── list.c
│   │   ├── list.h
│   │   ├── map.c
│   │   ├── map.h
│   │   ├── extra.c
│   │   ├── extra.h


Instala GCC (si no lo tienes):

Descarga e instala MinGW desde mingw-w64.org.
Añade el directorio bin de MinGW (por ejemplo, C:\MinGW\bin) a la variable de entorno PATH.
Verifica el archivo CSV:


Asegúrate de que song_dataset_.csv esté en:
C:\Users\<tu_usuario>\OneDrive\Desktop\tarea2\TDAs\
(Se hace enfasis en que el archivo CSV debe estar ubicado en la carpeta indicada anteriormente ya que la funcion que lee al mismo esta modificada
para que ubique al archivo en unicamente esta carpeta.)

El CSV debe tener columnas como ID, título, artistas, álbum, género y tempo (columna 18 para tempo).
Compilación

Abre una terminal (CMD o PowerShell) y navega al directorio del proyecto:

 cd C:\Users\<tu_usuario>\OneDrive\Desktop\tarea2

Compila el programa con GCC:

 gcc TDAs/tarea2.c TDAs/tdas/list.c TDAs/tdas/map.c TDAs/tdas/extra.c -o tarea2

Verifica que se genere el ejecutable tarea2.exe.

Nota sobre Codificación
Si ves caracteres extraños en la terminal (por ejemplo, opci├│n en lugar de opción), configura la codificación a UTF-8:
chcp 65001

Esto debería mostrar los acentos correctamente. Si persiste, usa PowerShell o ignóralo (no afecta la funcionalidad).
Ejecución
Ejecuta el programa:
./tarea2

El programa cargará las canciones desde song_dataset_.csv y mostrará un menú interactivo:
Menu:
1. Buscar por artista
2. Buscar por género
3. Buscar por tempo
4. Crear nueva lista de reproducción
5. Agregar canción a lista de reproducción
6. Mostrar canciones de una lista de reproducción
7. Guardar lista de reproducción
8. Testear con datos aleatorios
9. Salir
Seleccione una opción (1-9, o 'q' para salir):

Ingresa un número del 1 al 9 para seleccionar una opción. Usa q para cancelar en cualquier momento.

Estructura del Código

El programa está implementado en tarea2.c y utiliza las siguientes estructuras y funciones clave:

Estructuras de Datos

Song: Representa una canción con:

1)id (int): Identificador único.
2)title (char[100]): Título.
3)artists (List*): Lista de artistas.
4)album (char[100]): Álbum.
5)genre (char[100]): Género.
6)tempo (float): Tempo en BPM.

Para la organizacion de los distintos datos se opto porla creacion de varios mapas que nos permitieran manipular los datos de forma mas sencilla.

Mapas:

canciones_by_id: Mapa de ID a canciones (clave: int, valor: Song*).
canciones_by_genero: Mapa de género a lista de canciones (clave: char*, valor: List*).
canciones_by_artista: Mapa de artista a lista de canciones (clave: char*, valor: List*).
playlists: Mapa de nombre de lista a lista de canciones (clave: char*, valor: List*).

Funciones Principales:

Función:leer_canciones
Descripción:Carga canciones desde el CSV y las organiza en mapas.

Función:buscar_por_artista
DescripciónBusca canciones por letra inicial del artista y nombre.

Función:buscar_por_genero
DescripciónBusca canciones por género y letra inicial del título, guarda resultados en un archivo.

Función:buscar_por_tempo
DescripciónBusca canciones en un rango de tempo (mínimo y máximo).

Función:crear_lista_reproduccion
DescripciónCrea una nueva lista de reproducción con un nombre único.

Función:agregar_cancion_a_lista
DescripciónAgrega canciones por ID a una lista existente.

Función:mostrar_lista_reproduccion
DescripciónMuestra las canciones de una lista con detalles.

Función:guardar_lista_reproduccion
DescripciónGuarda una lista en un archivo CSV.

Función:testear_datos_aleatorios
DescripciónGenera canciones aleatorias para pruebas.

Función:liberar_canciones
DescripciónLibera la memoria de todos los mapas.

Funciones Auxiliares

Entrada: leer_cadena, leer_numero, leer_caracter, leer_tempo, es_numero_valido.

Validación: es_cadena_valida, clasificar_tempo, validar_rango_tempo.

Utilidades: limpiar_buffer, mostrar_categorias_tempo, filtrar_y_mostrar_canciones_por_tempo.

El código está modularizado, con funciones específicas para cada tarea, y comentarios en cada linea de codigo que explican su propósito de la forma mas clara 
posible, sin lengujae muy tecnico, ya que al tratarse de una apliacion que gestionara musica, se espera que los usuarios que haran uso de la misma no esten 
ilustrados con la jerga utilizada en el ambito informartico, de alli la decision de comentar cada linea del mismo.

Funcionalidades y Ejemplos de Uso

A continuación, se detalla cada opción del menú con un ejemplo de uso.

1. Buscar por Artista
Descripción: Busca artistas por letra inicial (A-Z o # para "Otros"),donde otros pertenece al grupo de artistas cuyas inciales comienzan con caracteres especiales
o letras que no estan dentro del alfabeto castellano, muestra una lista de artistas y permite seleccionar uno para ver sus canciones, al lado de los mismos tambien
se incluyo el dato de las canciones existentes dentro del archivo para ese artista, de manera que para el usuario sea mas amigable la experiencia de buscar canciones.
Al momento de imprimirse la cancion ,esta ademas tendra informacion adicional como su ID y Genero para retroalimentar al usuario.

Ejemplo de Uso:
Seleccione una opción: 1
Ingrese la letra inicial del artista (A-Z, '#' para Otros, o 'q' para cancelar): B
Artistas que comienzan con 'B':
1. Billie Eilish (3 canciones)
2. Bruno Mars (5 canciones)
Ingrese el nombre del artista (o 'q' para cancelar): Billie Eilish
1. Bad Guy (ID: 123, Género: Pop)
2. Lovely (ID: 124, Género: Pop)
3. Everything I Wanted (ID: 125, Género: Pop)
Presione una tecla para continuar...


2. Buscar por Género
Descripción: Muestra todos los géneros disponibles en un recuadro, al lado de los mismos la cantidad exacta de canciones dentro de cada genero,
al ingresar el genero de eleccion pedira adicional una letra inicial ya que las canciones se encuentran organizadas en orden alfabetico para filtrar las
mismas y al momento de ser mostradas sea una cantidad mas facil de procesar para cualquier usuario, estas tambien estan acompañadas por informacion extra
al momento de ser presentadas, para facilitar la experiencia del usuario, por ultimo guarda los resultados en un archivo de texto.

Ejemplo:
Seleccione una opción: 2
Géneros disponibles:
Pop (50)   Rock (30)   Jazz (20)   Hip Hop (40)
Ingrese el género a buscar (o 'q' para cancelar): Pop
Ingrese la letra inicial (A-Z, o 'q' para cancelar): S
Canciones del género 'Pop' que comienzan con 'S':
Shape of You (ID: 200) | Sorry (ID: 201) | Starboy (ID: 202) | ...
Canciones filtradas guardadas en 'S_canciones.txt'.
Presione una tecla para continuar...


4. Buscar por Tempo
Descripción: Muestra un cuadro de texto con las categorías de tempo (Lentas <80 BPM, Moderadas 80-120 BPM, Rápidas >120 BPM), pide un rango de tempo y
muestra canciones en ese rango. Debido a que la cantidad de canciones son muchas, se penso en modificar la funcion, pidiendo un rango de tempo minimo y
un rango maximo, filtrando de esta forma aun mas la cantidad de datos a mostrar y acercandonos de mejor forma con las exigencias esperadas por el usuario.
Las canciones se imprimiran al final con la informarcion respectiva a su clasificacion por tempo.

Ejemplo:
Seleccione una opción: 3
Rangos de tempo disponibles:
Lentas (Tempo menos de 80 BPM)
Moderadas (Tempo entre 80 y 120 BPM)
Rápidas (Tempo mayor a 120 BPM)
Ingrese el rango de tempo:
Tempo mínimo (o 'q' para cancelar): 80
Tempo máximo (o 'q' para cancelar): 120
Canciones en el rango de tempo 80 - 120:
1. Shape of You (ID: 200, Tempo: 96.00, Clasificación: Moderada)
2. Uptown Funk (ID: 201, Tempo: 115.00, Clasificación: Moderada)
Presione una tecla para continuar...


4. Crear Nueva Lista de Reproducción
Descripción: Crea una lista de reproducción con un nombre único.

Ejemplo:
Seleccione una opción: 4
Ingrese el nombre de la nueva lista de reproducción (o 'q' para cancelar): MiLista
Lista de reproducción 'MiLista' creada exitosamente.
Presione una tecla para continuar...


5. Agregar Canción a Lista de Reproducción
Descripción: Muestra las listas disponibles para mayor practicidad del usuario, pide el nombre de una lista y los IDs de canciones para agregar.Luego de ingresada
la cancion se despliega una barra de texto que preguntara si desea ingresar mas canciones en la lista, de ser si debe ingresar un nuevo ID,de ser no redirigira al
menu principal.

Ejemplo:
Seleccione una opción: 5
Listas de reproducción disponibles:
1. MiLista
Ingrese el nombre de la lista de reproducción (o 'q' para cancelar): MiLista
Ingrese el ID de la canción (o 'q' para cancelar): 123
Canción 'Bad Guy' agregada a la lista 'MiLista'.
¿Desea agregar otra canción a la lista 'MiLista'? (s/n): s
Ingrese el ID de la canción (o 'q' para cancelar): 124
Canción 'Lovely' agregada a la lista 'MiLista'.
¿Desea agregar otra canción a la lista 'MiLista'? (s/n): n
Presione una tecla para continuar...


6. Mostrar Canciones de una Lista de Reproducción
Descripción: Mostrara las distintas listas creadas con anticipacion, seleccionada la lista en cuestion mostrara las canciones de una esta
con informacion adicional de las mismas (título, ID, tempo, género).

Ejemplo:
Seleccione una opción: 6
Listas de reproducción disponibles:
1. MiLista
Ingrese el nombre de la lista de reproducción (o 'q' para cancelar): MiLista
Canciones en la lista de reproducción 'MiLista':
1. Bad Guy (ID: 123, Tempo: 135.00, Género: Pop)
2. Lovely (ID: 124, Tempo: 75.00, Género: Pop)
Presione una tecla para continuar...


7. Guardar Lista de Reproducción
Descripción: Guarda una lista de reproducción en un archivo CSV con columnas ID, título, tempo y género. Esta opcion se agrego de forma adicional, con la finalidad de
almacenar en el tiempo los datos de las listas creadas por el usuario por si en un futuro desea hacer uso de la informacion de las mismas.

Ejemplo:
Seleccione una opción: 7
Ingrese el nombre de la lista de reproducción a guardar (o 'q' para cancelar): MiLista
Lista de reproducción 'MiLista' guardada en el archivo 'MiLista.csv'.
Presione una tecla para continuar...

Contenido de MiLista.csv:
ID,Title,Tempo,Género
123,Bad Guy,135.00,Pop
124,Lovely,75.00,Pop


8. Testear con Datos Aleatorios
Descripción: Genera canciones aleatorias con títulos, artistas, géneros y tempos, y opcionalmente las agrega a una lista de reproducción. Esta funcion se creo con la
intencion de hacer testeo de las opciones 5,6,7 que manejan directamente las listas de reproduccion, esto con la finalidad de agilizar los procesos de testeo y no tener
que ingresar dato a dato para la creacion de listas de reproduccion.

Ejemplo:
Seleccione una opción: 8
Ingrese el número de canciones aleatorias a generar (o 'q' para cancelar): 40
¿Desea agregar las canciones generadas a una lista de reproducción? (s/n): s
Ingrese el nombre de la lista de reproducción (o 'q' para cancelar): TestLista
Lista de reproducción 'TestLista' creada.
Se generaron 40 canciones aleatorias.
Las canciones fueron agregadas a la lista 'TestLista'.
Presione una tecla para continuar...


9. Salir
Descripción: Cierra el programa y libera toda la memoria.

Ejemplo:
Seleccione una opción: 9
Saliendo...

A mejorar:

Implementar una interfaz de usuario más amigable.
Permitir la opcion de testeo para todo el codigo.
Mostrar por pantalla las canciones organizadas por artistas, y estos a su vez organizados por pais de origen.
Mostrar por pantalla las canciones organizadas por idioma.

Observación: El codigo a lo largo de su implementación cuenta con comentarios en cada linea que serviran para que cualquier 
persona que desee posiblemente intentar entender el funcionamiento el mismo, se busco hacer uso de nombre 
de funciones amigables para que este codigo sea accesible a todo aquel que se interese por entener su estructura.


