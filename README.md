# 🧠 PseudoDB en C  
**Un sistema de gestión de bases de datos hecho desde cero**

Este proyecto implementa un **gestor de bases de datos sencillo** usando el lenguaje C. Permite crear bases de datos y tablas, insertar registros y realizar consultas con filtros y ordenación, todo mediante comandos personalizados en una interfaz de consola.

---

## 🚀 Compilación y ejecución

```bash
gcc -g main.c lib.c -o main.exe
./main.exe
```

## 🧾 Comandos disponibles

### 🔚 `exit`
Finaliza la ejecución del programa.

- Libera toda la memoria reservada antes de salir.
- Devuelve el control a la consola de Windows.

---

### 📁 Bases de datos

#### `new db <nombre_base_de_datos>`
Crea una nueva base de datos (carpeta).

- Comprueba que el nombre es válido y que no existe ya.
- ❌ Muestra un mensaje de error si no se puede crear.

#### `use db <nombre_base_de_datos>`
Activa una base de datos existente.

- Cambia la base de datos activa.
- Actualiza el prompt con el nuevo nombre.
- ❌ Si no existe, muestra error y no cambia nada.

---

### 📑 Tablas

#### `new table <nombre_tabla> <campo> <tipo> [<campo> <tipo> ...]`
Crea una tabla en la base de datos activa.

- Crea un fichero vacío para la tabla.
- Añade la tabla a los metadatos de la base de datos.
- Reglas:
  - Nombres de campos válidos y sin duplicados.
  - Tipos permitidos: `NUM`, `DATE`, `TEXT`.
- ❌ Muestra errores si el nombre ya existe, los campos se repiten o hay tipos inválidos.

#### `use table <nombre_tabla>`
Activa una tabla de la base de datos actual.

- Carga los datos de la tabla en memoria.
- Libera los datos de la tabla anterior, si los hay.
- ❌ Si la tabla no existe, no hace nada y muestra error.

---

### ✍️ Insertar registros

#### `insert <valor> [<valor> ...]`
Inserta un nuevo registro en la tabla activa.

- Requisitos:
  - Número de valores igual al número de campos.
  - Tipos correctos para cada campo.
  - Textos con espacios deben ir entre comillas dobles (`"..."`).
- Inserta el registro en el fichero y en la lista de memoria.
- ❌ Muestra mensaje si hay errores de número o tipo de datos.

---

### 🔎 Consultas

#### `select`
Muestra todos los registros de la tabla activa.

- Incluye cabecera con nombres de campos.
- Cada registro va precedido de su número de fila.

#### `select <campo> ⊗ <valor> [ASC|DES]`
Consulta registros con condición y orden.

- `⊗` puede ser `<`, `>`, `=`.
- Opcional: `ASC` (ascendente) o `DES` (descendente).
- Orden según el tipo del **primer campo**:
  - `NUM`: orden numérico.
  - `DATE`: orden cronológico.
  - `TEXT`: orden lexicográfico.
- ❌ Muestra error si hay fallos en tipos, condición o formato.

---

## 📌 Ejemplo de sesión

```bash
new db alumnos
use db alumnos
new table notas nombre TEXT nota NUM
use table notas
insert "Ana Perez" 8.5
insert "Luis Gomez" 6.3
insert "Clara Ruiz" 9.1
select nota > 7 ASC
```

