#include "ArchivoContactos.h"
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QSaveFile>
#include <QStringList>

namespace {

void asignarError(QString* error, const QString& texto)
{
    if (error)
        *error = texto;
}

}

QString ArchivoContactos::ruta() const
{
    return QDir::homePath() + "/Lab7Datos/contactos.txt";
}

bool ArchivoContactos::guardar(const QString& nombre,
                              const QString& telefono,
                              const QString& correo) const
{
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append))
        return false;

    QString linea = nombre + ";" + telefono + ";" + correo + "\n";
    QByteArray datos = linea.toUtf8();
    bool completo = archivo.write(datos) == datos.size();
    bool vaciado = archivo.flush();
    archivo.close();
    return completo && vaciado;
}

QList<Contacto> ArchivoContactos::leer(QString* error) const
{
    QList<Contacto> contactos;
    QFile archivo(ruta());
    if (!archivo.exists())
        return contactos;

    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        asignarError(error, "No se pudo abrir el archivo: "
            + archivo.errorString());
        return contactos;
    }

    while (!archivo.atEnd()) {
        QString linea = QString::fromUtf8(archivo.readLine()).trimmed();
        if (linea.isEmpty())
            continue;

        QStringList partes = linea.split(';');
        if (partes.size() < 3)
            continue;

        Contacto contacto;
        contacto.nombre = partes.at(0);
        contacto.telefono = partes.at(1);
        contacto.correo = partes.at(2);
        contactos.append(contacto);
    }

    archivo.close();
    return contactos;
}

bool ArchivoContactos::actualizar(int indice, const Contacto& contacto,
                                 QString* error) const
{
    QString errorLectura;
    QList<Contacto> contactos = leer(&errorLectura);
    if (!errorLectura.isEmpty()) {
        asignarError(error, errorLectura);
        return false;
    }

    if (indice < 0 || indice >= contactos.size()) {
        asignarError(error, "El contacto seleccionado ya no existe.");
        return false;
    }

    contactos[indice] = contacto;
    return escribirTodos(contactos, error);
}

bool ArchivoContactos::eliminar(int indice, QString* error) const
{
    QString errorLectura;
    QList<Contacto> contactos = leer(&errorLectura);
    if (!errorLectura.isEmpty()) {
        asignarError(error, errorLectura);
        return false;
    }

    if (indice < 0 || indice >= contactos.size()) {
        asignarError(error, "El contacto seleccionado ya no existe.");
        return false;
    }

    contactos.removeAt(indice);
    return escribirTodos(contactos, error);
}

bool ArchivoContactos::escribirTodos(const QList<Contacto>& contactos,
                                    QString* error) const
{
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta)) {
        asignarError(error, "No se pudo crear la carpeta: " + carpeta);
        return false;
    }

    QSaveFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        asignarError(error, "No se pudo abrir el archivo: "
            + archivo.errorString());
        return false;
    }

    for (const Contacto& contacto : contactos) {
        QString linea = contacto.nombre + ";" + contacto.telefono + ";"
            + contacto.correo + "\n";
        QByteArray datos = linea.toUtf8();
        if (archivo.write(datos) != datos.size()) {
            asignarError(error, "No se pudo escribir el archivo: "
                + archivo.errorString());
            archivo.cancelWriting();
            return false;
        }
    }

    if (!archivo.commit()) {
        asignarError(error, "No se pudo reemplazar el archivo: "
            + archivo.errorString());
        return false;
    }

    return true;
}
