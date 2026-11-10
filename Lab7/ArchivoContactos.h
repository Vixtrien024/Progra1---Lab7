#pragma once
#include <QString>
#include <QList>

struct Contacto
{
    QString nombre;
    QString telefono;
    QString correo;
};

class ArchivoContactos
{
public:
    QString ruta() const;
    bool guardar(const QString& nombre,
        const QString& telefono,
        const QString& correo) const;
    QList<Contacto> leer(QString* error = nullptr) const;
    bool actualizar(int indice, const Contacto& contacto,
        QString* error = nullptr) const;
    bool eliminar(int indice, QString* error = nullptr) const;

private:
    bool escribirTodos(const QList<Contacto>& contactos,
        QString* error = nullptr) const;
};
