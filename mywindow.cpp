//https://www.youtube.com/watch?v=20Rzvg3S_Gk&feature=youtu.be

#include "mywindow.h"
#include "ui_mywindow.h"

MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);

    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    pocz.x = ui->rysujFrame->x();
    pocz.y = ui->rysujFrame->y();

    backgroundColor.setRgb(0, 0, 0, 255);
    colorRysowania.setRgb(255, 0, 0, 255);

    img = new QImage(":/pizza.png");
    T = new QImage(":/pizza.png");
}

MyWindow::~MyWindow()
{
    delete ui;
}

void MyWindow::on_exitButton_clicked()
{
    qApp->quit();
}

void MyWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);

    p.drawImage(pocz.x, pocz.y, *img);
}

void MyWindow::on_cleanButton_clicked()
{
    reset();
}

void MyWindow::zamalujPiksel(punkt &piksel, QColor &color)
{
    unsigned char *ptr;
    ptr = img->bits();
    int szer = img->width();
    int skladowaRed = 4*piksel.x + szer*4*piksel.y + 2;
    int skladowaGreen = 4*piksel.x + szer*4*piksel.y + 1;
    int skladowaBlue = 4*piksel.x + szer*4*piksel.y;

    if ((piksel.x>=0)&&(piksel.y>=0)&&(piksel.x<szer)&&(piksel.y<wys))
    {
            ptr[skladowaRed]= color.red();
            ptr[skladowaGreen] = color.green();
            ptr[skladowaBlue] = color.blue();
    }
}

void MyWindow::rysujPunktyKwadraty(punkt &P)
{
    punkt pkt;

    for(pkt.x = P.x - 4; pkt.x < P.x + 4; ++ pkt.x)
    {
        for(pkt.y = P.y - 4; pkt.y <P.y + 4; ++ pkt.y)
        {
            zamalujPiksel(pkt, colorRysowania);
        }
    }
}

double MyWindow::kwadratOdleglosciPunktow(punkt &P, punkt &Q)
{
    double kwadratOdleglosci;
    kwadratOdleglosci = (P.x - Q.x)*(P.x - Q.x) + (P.y - Q.y)*(P.y - Q.y);
    return kwadratOdleglosci;
}

void MyWindow::rysujOdcinek(punkt &a, punkt &b)
{
    int x, y, yp, xp;
    punkt nowy;
    double m;

    if(a.x == b.x)
    {
        if(a.y > b.y)
        {
            for(y = b.y; y <= a.y; ++ y)
            {
                nowy.x = a.x;
                nowy.y = y;
                zamalujPiksel(nowy, colorRysowania);
            }
        }
        else
        {
            for(y = a.y; y <= b.y; ++ y)
            {
                nowy.x = a.x;
                nowy.y = y;
                zamalujPiksel(nowy, colorRysowania);
            }
        }
    }
    else
    {
        m = (double)(b.y - a.y)/(b.x - a.x);
        if(m < -1 || m > 1)
        {
            if(a.y > b.y)
            {
                for(y = b.y; y <= a.y; y++)
                {
                    x = (y - a.y)/m + a.x;
                    xp = (int)floor(x + 0.5);
                    nowy.x = xp;
                    nowy.y = y;
                    zamalujPiksel(nowy, colorRysowania);
                }
            }
            else
            {
                for(y = a.y; y <= b.y; y++)
                {
                    x = (y - a.y)/m + a.x;
                    xp = (int)floor(x + 0.5);
                    nowy.x = xp;
                    nowy.y = y;
                    zamalujPiksel(nowy, colorRysowania);
                }
            }
        }
        else
        {
            if(a.x > b.x)
            {
                for(x = b.x; x <= a.x; x++)
                {
                    y = m * (x - a.x) + a.y;
                    yp = (int)floor(y + 0.5);
                    nowy.x = x;
                    nowy.y = yp;
                    zamalujPiksel(nowy, colorRysowania);
                }
            }
            else
            {
                for(x = a.x; x <= b.x; x++)
                {
                    y = m * (x - a.x) + a.y;
                    yp = (int)floor(y + 0.5);
                    nowy.x = x;
                    nowy.y = yp;
                    zamalujPiksel(nowy, colorRysowania);
                }
            }
        }
    }

}

void MyWindow::rysujTrojkat(punkt trojkat[])
{
    rysujOdcinek(trojkat[0], trojkat[1]);
    rysujOdcinek(trojkat[1], trojkat[2]);
    rysujOdcinek(trojkat[2], trojkat[0]);

    rysujPunktyKwadraty(trojkat[0]);
    rysujPunktyKwadraty(trojkat[1]);
    rysujPunktyKwadraty(trojkat[2]);
}

void MyWindow::wyczyscTrojkat(punkt trojkat[])
{
    for(int i = 0; i < 3; ++ i)
    {
        trojkat[0].x = 0;
        trojkat[0].y = 0;
    }
}

int MyWindow::wyznacznikGlowny(punkt trojkat[])
{
    int xA = trojkat[0].x;
    int yA = trojkat[0].y;
    int xB = trojkat[1].x;
    int yB = trojkat[1].y;
    int xC = trojkat[2].x;
    int yC = trojkat[2].y;

    int wyz = (xB - xA) * (yC - yA) - (yB - yA) * (xC - xA);
    return wyz;
}

void MyWindow::obliczWspolrzedneBarycentryczne(punkt trojkat[], punkt &piksel, double &u, double &v, double &w)
{
    int xA = trojkat[0].x;
    int yA = trojkat[0].y;
    int xB = trojkat[1].x;
    int yB = trojkat[1].y;
    int xC = trojkat[2].x;
    int yC = trojkat[2].y;
    int x = piksel.x;
    int y = piksel.y;

    int wyznacznikV = (x - xA) * (yC - yA) - (y - yA) * (xC - xA);
    int wyznacznikW = (xB - xA) * (y - yA) - (yB - yA) * (x- xA);

    v = (double) 1.0*wyznacznikV/wyznacznikGlowny(trojkat);
    w = (double) 1.0*wyznacznikW/wyznacznikGlowny(trojkat);
    u = 1.0 - v - w;
}

QColor MyWindow::kolorInterpolacja(double x, double y)
{
    QColor kolor;

    punkt GL;
    punkt GP;
    punkt DL;
    punkt DP;

    GL.x = floor(x);
    DL.x = floor(x);
    GP.x = floor(x) + 1;
    DP.x = floor(x) + 1;
    DL.y = floor(y);
    DP.y = floor(y);
    GL.y = floor(y) + 1;
    GP.y = floor(y) + 1;

    QColor kolP1 = img->pixelColor(DL.x, DL.y);
    QColor kolP2 = img->pixelColor(DP.x, DP.y);
    QColor kolP3 = img->pixelColor(GP.x, GP.y);
    QColor kolP4 = img->pixelColor(GL.x, GL.y);

    int a = x - floor(x);
    int b = floor(y) + 1 - y;

    kolor.setRed(b * ((1 - a) * kolP1.red() + a * kolP2.red()) +
                  (1 - b) * ((1 - a) * kolP4.red() + a * kolP3.red()));
    kolor.setGreen(b * ((1 - a) * kolP1.green() + a * kolP2.green()) +
                  (1 - b) * ((1 - a) * kolP4.green() + a * kolP3.green()));
    kolor.setBlue(b * ((1 - a) * kolP1.blue() + a * kolP2.blue()) +
                  (1 - b) * ((1 - a) * kolP4.blue() + a * kolP3.blue()));

    std::cout << DL.x << " " << DP.x << " " << GP.x << " " << GL.x << std::endl;

    return kolor;
}

bool MyWindow::czyLiniaPrzecinaWielokat(int &y, punkt &startOdcinka, punkt &stopOdcinka){
    punkt wierzcholekGorny;
    punkt wierzcholekDolny;
    if(startOdcinka.y > stopOdcinka.y){
        wierzcholekGorny = startOdcinka;
        wierzcholekDolny = stopOdcinka;
    }
    else{
        wierzcholekGorny = stopOdcinka;
        wierzcholekDolny = startOdcinka;
    }
    if(y <= wierzcholekGorny.y && y > wierzcholekDolny.y)
        return true;
    else
        return false;
}

float MyWindow::wspolczynnikDoXPrzeciecia(punkt &startOdcinka, punkt &stopOdcinka){
    float wynik;
    wynik = 1.0*(stopOdcinka.x - startOdcinka.x)/(stopOdcinka.y - startOdcinka.y);
    return wynik;
}

int MyWindow::xPrzeciecia(int &y, punkt &startOdcinka, punkt &stopOdcinka){
    if(stopOdcinka.x - startOdcinka.x == 0 || stopOdcinka.y - startOdcinka.y == 0){
        if(startOdcinka.x < stopOdcinka.x)
            return startOdcinka.x;
        else
            return stopOdcinka.x;
    }
    else{
        return (startOdcinka.x + (y - startOdcinka.y) * wspolczynnikDoXPrzeciecia(startOdcinka, stopOdcinka));
    }
}

void MyWindow::wypelnijTrojkat(punkt trojkatPlain[], punkt trojkatTexture[])
{
    punkt pkt;
    punkt pktT;

    int xA = trojkatTexture[0].x;
    int yA = trojkatTexture[0].y;
    int xB = trojkatTexture[1].x;
    int yB = trojkatTexture[1].y;
    int xC = trojkatTexture[2].x;
    int yC = trojkatTexture[2].y;

    int goraTrojkata = std::min({trojkatPlain[0].y, trojkatPlain[1].y, trojkatPlain[2].y});
    int dolTrojkata = std::max({trojkatPlain[0].y, trojkatPlain[1].y, trojkatPlain[2].y});

    int startRysowania = 0;
    int stopRysowania = 0;

    for(pkt.y = goraTrojkata; pkt.y <= dolTrojkata; ++ pkt.y)
    {
        startRysowania = 0;
        stopRysowania = 0;
        if(czyLiniaPrzecinaWielokat(pkt.y, trojkatPlain[0], trojkatPlain[1]))
        {
            startRysowania = xPrzeciecia(pkt.y, trojkatPlain[0], trojkatPlain[1]);
            stopRysowania = startRysowania;
        }
        if(czyLiniaPrzecinaWielokat(pkt.y, trojkatPlain[1], trojkatPlain[2]))
        {
            if(startRysowania > xPrzeciecia(pkt.y, trojkatPlain[1], trojkatPlain[2]) || startRysowania ==0)
            {
                startRysowania = xPrzeciecia(pkt.y, trojkatPlain[1], trojkatPlain[2]);
            }
            if(stopRysowania < xPrzeciecia(pkt.y, trojkatPlain[1], trojkatPlain[2]) || stopRysowania == 0)
            {
                stopRysowania = xPrzeciecia(pkt.y, trojkatPlain[1], trojkatPlain[2]);
            }
        }
        if(czyLiniaPrzecinaWielokat(pkt.y, trojkatPlain[2], trojkatPlain[0]))
        {
            if(startRysowania > xPrzeciecia(pkt.y, trojkatPlain[2], trojkatPlain[0]) || startRysowania == 0)
            {
                startRysowania = xPrzeciecia(pkt.y, trojkatPlain[2], trojkatPlain[0]);
            }
            if(stopRysowania < xPrzeciecia(pkt.y, trojkatPlain[2], trojkatPlain[0]) || stopRysowania == 0)
            {
                stopRysowania = xPrzeciecia(pkt.y, trojkatPlain[2], trojkatPlain[0]);
            }
        }
        std::cout << "start: " << startRysowania << std::endl;
        std::cout << "stop: " << stopRysowania << std::endl;
        for(pkt.x = startRysowania; pkt.x <= stopRysowania; ++ pkt.x)
        {
            obliczWspolrzedneBarycentryczne(trojkatPlain, pkt, u, v, w);
            pktT.x = u * xA + v * xB + w * xC;
            pktT.y = u * yA + v * yB + w * yC;
            img->setPixelColor(pkt.x, pkt.y, kolorInterpolacja(pktT.x, pktT.y));
        }
    }
}

void MyWindow::czysc()
{
    punkt pkt;

    for(pkt.y = 0; pkt.y < wys; pkt.y ++)
    {
        for(pkt.x = 0; pkt.x < szer; pkt.x ++)
        {
            zamalujPiksel(pkt, backgroundColor);
        }
    }
}

void MyWindow::reset()
{
    img = new QImage(":/pizza.png");
    licznik = 0;
    indexPunktuDoPrzesuniecia1 = -1;
    indexPunktuDoPrzesuniecia2 = -1;
    wyczyscTrojkat(trojkat1);
    wyczyscTrojkat(trojkat2);
    update();
}

void MyWindow::mousePressEvent(QMouseEvent *event)
{
    pkt.x = event->x();
    pkt.y = event->y();

    pkt.x -= pocz.x;
    pkt.y -= pocz.y;

    *T = *img;

    if(licznik <= 2)
    {
        trojkat1[licznik] = pkt;
        rysujPunktyKwadraty(pkt);
    }
    else if(licznik > 2 && licznik <= 5)
    {
        trojkat2[licznik - 3] = pkt;
        rysujPunktyKwadraty(pkt);
    }
    else
    {
        for(int i = 0; i < 3; ++ i)
        {
            if(kwadratOdleglosciPunktow(trojkat1[i], pkt) < minimalnyKwadratOdleglosci1)
            {
                minimalnyKwadratOdleglosci1 = kwadratOdleglosciPunktow(trojkat1[i], pkt);
                indexPunktuDoPrzesuniecia1  = i;
            }
            else if(kwadratOdleglosciPunktow(trojkat2[i], pkt) < minimalnyKwadratOdleglosci2)
            {
                minimalnyKwadratOdleglosci2 = kwadratOdleglosciPunktow(trojkat2[i], pkt);
                indexPunktuDoPrzesuniecia2  = i;
            }
        }
    }

    update();
}

void MyWindow::mouseMoveEvent(QMouseEvent *event)
{
    punkt pkt;

    pkt.x = event->x();
    pkt.y = event->y();

    pkt.x -= pocz.x;
    pkt.y -= pocz.y;


    if(licznik <= 2)
    {
        trojkat1[licznik] = pkt;
        *img = *T;
        rysujPunktyKwadraty(pkt);
    }
    else if(licznik > 2 && licznik <=5)
    {
        trojkat2[licznik - 3] = pkt;
        *img = *T;
        rysujPunktyKwadraty(pkt);
    }
    else if(indexPunktuDoPrzesuniecia1 != -1)
    {
        trojkat1[indexPunktuDoPrzesuniecia1].x = pkt.x;
        trojkat1[indexPunktuDoPrzesuniecia1].y = pkt.y;

        img = new QImage(":/pizza.png");

        rysujTrojkat(trojkat1);
        rysujTrojkat(trojkat2);

    }
    else if(indexPunktuDoPrzesuniecia2 != -1)
    {
        trojkat2[indexPunktuDoPrzesuniecia2].x = pkt.x;
        trojkat2[indexPunktuDoPrzesuniecia2].y = pkt.y;

        img = new QImage(":/pizza.png");

        rysujTrojkat(trojkat1);
        rysujTrojkat(trojkat2);
    }

    if(licznik >= 5)
    {
        if(trojkat1[0].x < szer/2)
        {
            wypelnijTrojkat(trojkat2, trojkat1);
        }
        else
        {
            wypelnijTrojkat(trojkat1, trojkat2);
        }
    }

    update();
}

void MyWindow::mouseReleaseEvent(QMouseEvent *event)
{
    punkt pkt;

    pkt.x = event->x();
    pkt.y = event->y();

    pkt.x -= pocz.x;
    pkt.y -= pocz.y;

    if(licznik == 2)
    {
        rysujTrojkat(trojkat1);
    }
    else if(licznik == 5)
    {
        rysujTrojkat(trojkat2);
    }
    else if(indexPunktuDoPrzesuniecia1 != -1)
    {
        trojkat1[indexPunktuDoPrzesuniecia1].x = pkt.x;
        trojkat1[indexPunktuDoPrzesuniecia1].y = pkt.y;

        rysujTrojkat(trojkat1);
        indexPunktuDoPrzesuniecia1 = -1;
        minimalnyKwadratOdleglosci1 = 16;
    }
    else if(indexPunktuDoPrzesuniecia2 != -1)
    {
        trojkat2[indexPunktuDoPrzesuniecia2].x = pkt.x;
        trojkat2[indexPunktuDoPrzesuniecia2].y = pkt.y;

        rysujTrojkat(trojkat2);
        indexPunktuDoPrzesuniecia2 = -1;
        minimalnyKwadratOdleglosci2 = 16;
    }

   if(licznik >= 5)
    {
        if(trojkat1[0].x < szer/2)
        {
            wypelnijTrojkat(trojkat2, trojkat1);
        }
        else
        {
            wypelnijTrojkat(trojkat1, trojkat2);
        }
    }

    licznik ++;

    update();
}











