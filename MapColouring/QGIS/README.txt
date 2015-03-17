Instrucciones para ver el mapa en QGIS:
--------------------------------------

1) Descargar QGIS desde http://www2.qgis.org/es/site/
2) Arrastrar el archivo Barrios_con_limitantes.shp al QGIS
3) Arrastrar el archivo asignacion_colores.csv al QGIS
4) En las propiedades de la capa "Barrios_con_limitantes" ir a la opción "Joins" y agregar un Join con las siguiente propiedades:

Join Layer: asignacion_colores
Join field: field_1
Target field: NROBARRIO

5) Dentro de la misma ventana de propiedades, ir a "Style" cambiar "Single Symbol" a "Categorized" y en "Column" elegir "asignacion_colores_field_2". Elegir Classify y aplicar los cambios.