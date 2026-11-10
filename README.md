# Lab 7 - Contactos

Aplicación de escritorio en Qt Widgets (C++) para registrar, visualizar,
editar y eliminar contactos guardados en un archivo de texto.

## Funcionalidad

- **Registrar**: valida los tres campos y agrega el contacto al final del archivo.
- **Historial**: lee el archivo completo y muestra los contactos en una tabla
  (`QTableWidget`) con columnas Nombre, Teléfono y Correo.
- **Editar**: permite seleccionar una fila, modificar sus datos y reescribir el
  archivo completo conservando el resto de los contactos.
- **Eliminar**: pide confirmación y quita únicamente el contacto seleccionado
  del archivo y de la tabla.

El menú **Navegación** contiene las cuatro opciones y cada una cambia de página o
de modo dentro de la aplicación.

## Ruta del archivo de datos

```text
C:/Users/<usuario>/Lab7Datos/contactos.txt
```

En Windows normalmente equivale a `%USERPROFILE%\Lab7Datos\contactos.txt`.
La carpeta se crea automáticamente al guardar el primer contacto.

Cada línea tiene el formato:

```text
nombre;telefono;correo
```

## Cómo compilar y ejecutar

1. Abrir `Lab7/Lab7.slnx` en Visual Studio.
2. Seleccionar la configuración `Debug` y la plataforma `x64`.
3. Compilar con **Compilar > Recompilar solución**.
4. Ejecutar con **Ctrl + F5**.

## Cómo se comprobó editar y eliminar

1. Se crearon tres contactos de prueba: Ana Mora, Carlos Rojas y María Solís.
2. En **Navegación > Historial** se verificó que aparecieran las tres filas sin
   duplicarse al volver a abrir la página.
3. En **Navegación > Editar** se seleccionó el segundo contacto (Carlos Rojas),
   se cambió su teléfono y se presionó **Actualizar**. La tabla y el archivo
   conservaron la misma cantidad de contactos.
4. En **Navegación > Eliminar** se seleccionó el tercer contacto (María Solís),
   se confirmó la eliminación y se verificó que desapareciera de la tabla y del
   archivo.
5. Se cerró y abrió la aplicación: quedaron dos contactos y el segundo mantuvo el
   teléfono modificado.
6. También se probó cancelar una eliminación, editar sin seleccionar una fila y
   abrir el historial cuando el archivo está vacío.

## Archivo de ejemplo

`contactos_ejemplo.txt` contiene tres contactos de prueba. Para usarlo, copiarlo a
`%USERPROFILE%\Lab7Datos\contactos.txt` (o guardar contactos desde la aplicación).

## Capturas para la entrega

Agregar en la entrega:

- `capturas/guardado.png`: guardado con mensaje de confirmación.
- `capturas/historial.png`: historial final con los contactos.
