gcc -g main.c lib.c -o main.exe
./main.exe

COMANDOS:

exit --> este comando finaliza la ejecución del programa.
new db <nombre_base_de_datos> --> crea una nueva base de datos.
use db <nombre_base_de_datos> --> cambia la base de datos activa.
new table <nombre_tabla> [ ...] --> crea una nueva tabla en la base de datos actual. Se creará un nuevo fichero en blanco (vacío) para dicha tabla y se modificará la lista y el fichero de metadatos para que refleje la existencia de esta nueva tabla.
use table <nombre_tabla> --> cambia la tabla activa.
insert [ ...] --> se inserta en la tabla activa el registro formado por los valores indicados.
select [ ⊗ ] [] --> este comando, cuando va sin parámetros, muestra por pantalla todos los registros de la tabla activa, primero una cabecera con los nombres de los campos y a continuación, en líneas sucesivas, todos los registros, que deberán ir precedidos de un número que indique su posición en la tabla (es decir, su posición en el fichero o la lista de filas). Si se incluyen los parámetros opcionales [ ⊗ ] se mostrarán solo los registros que cumplan la condición especificada; el carácter ‘⊗’ representa cualquiera de los siguientes operadores relacionales: < (menor), > (mayor) o = (igual). El ‘campo’ y el ‘valor’ deben ser del mismo tipo. El parámetro opcional ‘orden’ podrá ser uno de estos dos valores: ‘ASC’ o ‘DES’, para forzar a que el listado se muestre por pantalla de forma ordenada, ascendentemente o descendentemente según el valor de la primera columna de la tabla.
