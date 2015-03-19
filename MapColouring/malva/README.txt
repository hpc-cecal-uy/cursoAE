MapColouring en Malva:
---------------------
1) Instalar MPICH sudo apt-get install mpich
2) Cambiar en el archivo "environment" las variables MALLBA_DIR y MPI_BIN para que apunten a la carpeta de Malva y a los ejecutables de MPICH.
3) Ejecutar make all
4) En la carpeta rep/GA ejecutar make SEQ para correr el algoritmo. Se genera el archivo asignacion_colores.csv con el resultado de la asignación para ser importado a QGIS.


Detalles de la implementación:
-----------------------------

En el archivo newGA.req.cc se pueden observar las siguientes secciones de código:

1) 	Linea 39: istream& operator>> (istream& is, Problem& pbm)

En esta función se carga la dimensión del problema, y el archivo con los límites entre los barrios. Esto se carga una única vez y es accesible por cada una de las soluciones (ya que comparten el mismo problem)-

2) Línea 110: 	Direction Problem::direction() const

Se define la dirección del problema: Minimización

3) Linea 197: 	double Solution::fitness ()

En esta función se calcula el fitness de una solución, donde 0 es el fitness ideal y se suma 1 por cada barrio que tiene un barrio vecino con el mismo color.

4) Línea 353: 	void Crossover::cross(Solution& sol1,Solution& sol2) const

Acá se implementa el cruzamiento de dos soluciones. Se utilizó el cruzamiento de dos puntos (2PX), que ya viene implementado en Malva.

5) Línea 417: 	void Mutation::mutate(Solution& sol) const

Se implementa la mutación, donde el gen a mutar se cambia por un entero aleatorio {0,1,2,3} con probabilidad uniforme.

6) Línea 472: 	bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)

Se define el criterio de parada: si el fitness es cero, encontramos la solución al problema, así que dejamos de iterar. En caso de llegar al final, escribimos el archivo de salida para luego poder levantarlo en el QGIS.

