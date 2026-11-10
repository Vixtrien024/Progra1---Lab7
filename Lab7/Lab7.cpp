#include "Lab7.h"
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QStringList>
#include <QAction>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QSignalBlocker>

Lab7::Lab7(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.tablaContactos->setColumnCount(3);
    ui.tablaContactos->setHorizontalHeaderLabels(
        { "Nombre", "Teléfono", "Correo" });
    ui.tablaContactos->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tablaContactos->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.tablaContactos->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tablaContactos->horizontalHeader()->setStretchLastSection(true);
    ui.tablaContactos->verticalHeader()->setVisible(false);

    connect(ui.btnGuardar, &QPushButton::clicked,
        this, &Lab7::guardarContacto);
    connect(ui.actionRegistrar, &QAction::triggered,
        this, &Lab7::mostrarRegistro);
    connect(ui.actionHistorial, &QAction::triggered,
        this, &Lab7::mostrarHistorial);
    connect(ui.actionEditar, &QAction::triggered,
        this, &Lab7::mostrarEditar);
    connect(ui.actionEliminar, &QAction::triggered,
        this, &Lab7::mostrarEliminar);
    connect(ui.btnActualizar, &QPushButton::clicked,
        this, &Lab7::actualizarContacto);
    connect(ui.btnEliminar, &QPushButton::clicked,
        this, &Lab7::eliminarContacto);
    connect(ui.btnRecargar, &QPushButton::clicked,
        this, &Lab7::cargarTabla);
    connect(ui.tablaContactos, &QTableWidget::itemSelectionChanged,
        this, &Lab7::cargarCamposDesdeFila);

    ui.stackedWidget->setCurrentWidget(ui.page_3);
    aplicarModo();
    statusBar()->showMessage("Ingresá un contacto y presioná Guardar.");
}

Lab7::~Lab7()
{
}

bool Lab7::validarDatos(const QString& nombre, const QString& telefono,
                        const QString& correo)
{
    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos",
                             "Completá los tres campos.");
        return false;
    }

    for (const QString& dato : QStringList{nombre, telefono, correo}) {
        if (dato.contains(';') || dato.contains('\n') ||
            dato.contains('\r')) {
            QMessageBox::warning(this, "Dato no permitido",
                "No usés punto y coma ni saltos de línea.");
            return false;
        }
    }

    return true;
}

void Lab7::guardarContacto()
{
    QString nombre = ui.txtNombre->text().trimmed();
    QString telefono = ui.txtTelefono->text().trimmed();
    QString correo = ui.txtCorreo->text().trimmed();

    if (!validarDatos(nombre, telefono, correo))
        return;

    if (!archivoContactos.guardar(nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo guardar",
            "Revisá permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    ui.txtNombre->clear();
    ui.txtTelefono->clear();
    ui.txtCorreo->clear();
    cargarTabla();
    QMessageBox::information(this, "Guardado completo",
        "El contacto se guardó correctamente.\nArchivo: "
        + archivoContactos.ruta());
    ui.txtNombre->setFocus();
    statusBar()->showMessage("Contacto guardado.", 5000);
}

void Lab7::mostrarRegistro()
{
    ui.stackedWidget->setCurrentWidget(ui.page_3);
    ui.txtNombre->setFocus();
    statusBar()->showMessage("Registrar contacto.");
}

void Lab7::mostrarHistorial()
{
    modoActual = Modo::Historial;
    ui.stackedWidget->setCurrentWidget(ui.paginaHistorial);
    cargarTabla();
    aplicarModo();
    statusBar()->showMessage("Historial de contactos.");
}

void Lab7::mostrarEditar()
{
    modoActual = Modo::Editar;
    ui.stackedWidget->setCurrentWidget(ui.paginaHistorial);
    cargarTabla();
    aplicarModo();

    if (ui.tablaContactos->rowCount() == 0) {
        statusBar()->showMessage("No hay contactos para editar.");
        return;
    }

    ui.tablaContactos->selectRow(0);
    ui.txtEditarNombre->setFocus();
    statusBar()->showMessage("Seleccioná un contacto y modificá sus datos.");
}

void Lab7::mostrarEliminar()
{
    modoActual = Modo::Eliminar;
    ui.stackedWidget->setCurrentWidget(ui.paginaHistorial);
    cargarTabla();
    aplicarModo();

    if (ui.tablaContactos->rowCount() == 0)
        statusBar()->showMessage("No hay contactos para eliminar.");
    else
        statusBar()->showMessage("Seleccioná un contacto y presioná Eliminar.");
}

void Lab7::cargarTabla()
{
    QString error;
    QList<Contacto> contactos = archivoContactos.leer(&error);
    if (!error.isEmpty()) {
        mostrarErrorArchivo("No se pudo leer el historial", error);
        return;
    }

    QSignalBlocker bloqueo(ui.tablaContactos);
    ui.tablaContactos->clearContents();
    ui.tablaContactos->setRowCount(static_cast<int>(contactos.size()));

    for (int fila = 0; fila < contactos.size(); ++fila) {
        const Contacto& contacto = contactos.at(fila);
        ui.tablaContactos->setItem(fila, 0,
            new QTableWidgetItem(contacto.nombre));
        ui.tablaContactos->setItem(fila, 1,
            new QTableWidgetItem(contacto.telefono));
        ui.tablaContactos->setItem(fila, 2,
            new QTableWidgetItem(contacto.correo));
    }

    ui.tablaContactos->clearSelection();
    ui.txtEditarNombre->clear();
    ui.txtEditarTelefono->clear();
    ui.txtEditarCorreo->clear();
}

void Lab7::cargarCamposDesdeFila()
{
    int fila = ui.tablaContactos->currentRow();
    if (fila < 0) {
        ui.txtEditarNombre->clear();
        ui.txtEditarTelefono->clear();
        ui.txtEditarCorreo->clear();
        return;
    }

    QTableWidgetItem* itemNombre = ui.tablaContactos->item(fila, 0);
    QTableWidgetItem* itemTelefono = ui.tablaContactos->item(fila, 1);
    QTableWidgetItem* itemCorreo = ui.tablaContactos->item(fila, 2);

    ui.txtEditarNombre->setText(itemNombre ? itemNombre->text() : QString());
    ui.txtEditarTelefono->setText(
        itemTelefono ? itemTelefono->text() : QString());
    ui.txtEditarCorreo->setText(itemCorreo ? itemCorreo->text() : QString());
}

void Lab7::actualizarContacto()
{
    if (modoActual != Modo::Editar) {
        QMessageBox::information(this, "Editar contacto",
            "Elegí Editar en el menú Navegación para modificar un contacto.");
        return;
    }

    int fila = ui.tablaContactos->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Sin selección",
            "Seleccioná un contacto en la tabla.");
        return;
    }

    QString nombre = ui.txtEditarNombre->text().trimmed();
    QString telefono = ui.txtEditarTelefono->text().trimmed();
    QString correo = ui.txtEditarCorreo->text().trimmed();

    if (!validarDatos(nombre, telefono, correo))
        return;

    Contacto contacto{ nombre, telefono, correo };
    QString error;
    if (!archivoContactos.actualizar(fila, contacto, &error)) {
        mostrarErrorArchivo("No se pudo actualizar el contacto", error);
        return;
    }

    cargarTabla();
    if (fila < ui.tablaContactos->rowCount())
        ui.tablaContactos->selectRow(fila);

    QMessageBox::information(this, "Contacto actualizado",
        "Los datos se guardaron correctamente.");
    statusBar()->showMessage("Contacto actualizado.", 5000);
}

void Lab7::eliminarContacto()
{
    if (modoActual != Modo::Eliminar) {
        QMessageBox::information(this, "Eliminar contacto",
            "Elegí Eliminar en el menú Navegación para borrar un contacto.");
        return;
    }

    int fila = ui.tablaContactos->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Sin selección",
            "Seleccioná un contacto en la tabla.");
        return;
    }

    QTableWidgetItem* itemNombre = ui.tablaContactos->item(fila, 0);
    QString nombre = itemNombre ? itemNombre->text() : "el contacto";

    QMessageBox::StandardButton respuesta = QMessageBox::question(this,
        "Eliminar contacto",
        "¿Eliminar a \"" + nombre + "\"?\nEsta acción no se puede deshacer.",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (respuesta != QMessageBox::Yes)
        return;

    QString error;
    if (!archivoContactos.eliminar(fila, &error)) {
        mostrarErrorArchivo("No se pudo eliminar el contacto", error);
        return;
    }

    cargarTabla();
    QMessageBox::information(this, "Contacto eliminado",
        "El contacto se eliminó correctamente.");
    statusBar()->showMessage("Contacto eliminado.", 5000);
}

void Lab7::aplicarModo()
{
    const bool editando = modoActual == Modo::Editar;
    const bool eliminando = modoActual == Modo::Eliminar;

    ui.txtEditarNombre->setEnabled(editando);
    ui.txtEditarTelefono->setEnabled(editando);
    ui.txtEditarCorreo->setEnabled(editando);
    ui.btnActualizar->setEnabled(editando);
    ui.btnEliminar->setEnabled(eliminando);

    switch (modoActual) {
    case Modo::Historial:
        ui.lblModo->setText("Historial de contactos");
        break;
    case Modo::Editar:
        ui.lblModo->setText("Editar contacto seleccionado");
        break;
    case Modo::Eliminar:
        ui.lblModo->setText("Eliminar contacto seleccionado");
        break;
    }
}

void Lab7::mostrarErrorArchivo(const QString& titulo, const QString& error)
{
    QString detalle = error.isEmpty()
        ? QString("Revisá permisos y espacio disponible.")
        : error;
    QMessageBox::critical(this, titulo,
        detalle + "\nArchivo: " + archivoContactos.ruta());
}
