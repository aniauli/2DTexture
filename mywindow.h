#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QColorDialog>
#include <QColor>
#include "punkt.h"
#include <cmath>
#include <iostream>
#include <algorithm>

namespace Ui {
    class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = 0);

    ~MyWindow();

private:
    Ui::MyWindow *ui;

    QImage *img, *T;

    QColor backgroundColor;
    QColor colorRysowania;

    int szer;
    int wys;

    punkt pocz;
    punkt pkt;
    punkt trojkat1[3];
    punkt trojkat2[3];

    double u = 0, v = 0, w = 0;

    int licznik = 0;
    double minimalnyKwadratOdleglosci1 = 16;
    double minimalnyKwadratOdleglosci2 = 16;
    int indexPunktuDoPrzesuniecia1 = -1;
    int indexPunktuDoPrzesuniecia2 = -1;

    void czysc();
    void reset();
    void zamalujPiksel(punkt &piksel, QColor &color);
    void rysujPunktyKwadraty(punkt &P);
    double kwadratOdleglosciPunktow(punkt &P, punkt &Q);
    void rysujOdcinek(punkt &a, punkt &b);
    void rysujTrojkat(punkt trojkat[]);
    void wyczyscTrojkat(punkt trojkat[]);
    int wyznacznikGlowny(punkt trojkat[]);
    void obliczWspolrzedneBarycentryczne(punkt trojkat[], punkt &piksel, double &u, double &v, double &w);
    QColor kolorInterpolacja(double x, double y);
    bool czyLiniaPrzecinaWielokat(int &y, punkt &startOdcinka, punkt &stopOdcinka);
    int xPrzeciecia(int &y, punkt &startOdcinka, punkt &stopOdcinka);
    float wspolczynnikDoXPrzeciecia(punkt &startOdcinka, punkt &stopOdcinka);
    void wypelnijTrojkat(punkt trojkatPlain[], punkt trojkatTexture[]);

private slots:
    void on_cleanButton_clicked();
    void on_exitButton_clicked();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif
