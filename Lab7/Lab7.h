#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lab7.h"
#include "ArchivoContactos.h"

class Lab7 : public QMainWindow
{
    Q_OBJECT

public:
    Lab7(QWidget *parent = nullptr);
    ~Lab7();

private:
    enum class Modo { Historial, Editar, Eliminar };

    Ui::Lab7Class ui;
    ArchivoContactos archivoContactos;
    Modo modoActual = Modo::Historial;

    void guardarContacto();
    void mostrarRegistro();
    void mostrarHistorial();
    void mostrarEditar();
    void mostrarEliminar();
    void cargarTabla();
    void cargarCamposDesdeFila();
    void actualizarContacto();
    void eliminarContacto();
    void aplicarModo();
    bool validarDatos(const QString& nombre, const QString& telefono,
        const QString& correo);
    void mostrarErrorArchivo(const QString& titulo, const QString& error);
};
