Implementación en ECJ
---------------------

************************
Es necesario agregar la ruta a la carpeta ecj en el CLASSPATH. Por ejemplo, editando el archivo ~/.bashrc y agregando la siguiente línea:

CLASSPATH=:/home/usuario/MapColouring/ecj/

y luego ejecutando el comando:

source ~/.bashrc
************************


EJECUCION:
---------
Para ejecutar el algoritmo ir a la carpeta ecj/ec/app/mapcolouring y ejecutar el siguiente comando:

java ec.Evolve -file mapcolouring.params

que genera los archivos de salida out.stat (con las estadísticas de la ejecución) y asignacion_colores.csv (con el resultado para importar en QGIS).

Los parametros se pueden configurar en mapcolouring.params



CLASES MODIFICADAS:
------------------

-ec.vector.IntegerVectorSpecies: 
	Se modificó la clase para que cargue los límites de cada barrio desde el archivo de texto (ver procedimiento setup).

-ec.simple.SimpleStatistics:
	Se modificó el método finalStatistics para que genere el archivo de salida asignacion_colores.csv que luego puede ser importado al QGIS.

-ec.app.mapcolouring.Mapcolouring:
	Implementa la función de fitness, sumando 1 por cada barrio que limita con otro del mismo color. Por tratarse de un problema de minimización se usa el opuesto. Se fija 0 como el valor de fitness óptimo (ningún barrio limita con otro del mismo color).



