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
    Ui::Lab7Class ui;
    ArchivoContactos archivoContactos;
    void guardarContacto();

};
