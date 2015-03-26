Para ejecutar el proyecto: descargar el IDE Netbeans, abrir el proyecto y presionar "Run".

Para la implementación se codificaron 4 clases Java:

MapColoring:
Esta clase posee la función Main, y es la encargada de crear el "engine" evolutivo y
de ejecutar la evolución.
Se utiliza la clase MapColoringFactory para crear mapas al azar.
Se crean dos operadores evolutivos: MapColoringMutation
(implementado en la clase del mismo nombre) y el cruzamiento ListCrossover (implementado 
en el Framework Watchmaker). Por otro lado se asocia la función de fitness implmentada en
la clase MapEvaluator, y se elige una selección por Ruleta y un generador de números 
aleatorios (ambos implementados en el Framework, bajo los nombres de RouletteWheelSelection
y MersenneTwisterRNG correspondientemente). Por último se agrega un EvolutionLogger, que
imprime el mejor fitness de cada generación.

MapColoringFactory:
Esta clase es la encargada de generar una población inicial de mapas. Esto se realiza asignando
un color random a cada zona.

MapColoringMutation:
Esta clase es la encargada de mutar un mapa. Para ello, se elije una zona al azar dentro 
del individuo a mutar y se cambia su color asociado, también de manera aleatoria. 

MapEvaluator: 
Esta clase es la encargada de evaluar el mapa. El primer paso que realiza es cargar el
archivo de adyacencias entre zonas en una lista de listas (donde la primer lista se 
corresponde a una zona y la segunda posee sus adyacencias). En segundo lugar, implementa la
función getFitness, la cual recibe un individuo como parámetro y devuelve su valor de fitness.
Para ello, cuenta cada adyacencia entre zonas del mismo color y retorna dicho valor. 

