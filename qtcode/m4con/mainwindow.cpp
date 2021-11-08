#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(800,480);
    pushbutton = new QPushButton(this);
    pushbutton->setMinimumSize(200,50);
    pushbutton->setGeometry(300,215,pushbutton->width(),pushbutton->height());

    system("echo none > /sys/class/leds/sys-led/trigger");
    system("cd /lib/firmware");
    system("echo m4test_CM4.elf > /sys/class/remoteproc/remoteproc0/firmware");
    qDebug()<<"load firmware success!"<< endl;

    connect(pushbutton, SIGNAL(clicked()), this, SLOT(pushButtonClicked()));
    pushbutton->setText("Start M4");
}

MainWindow::~MainWindow()
{
    delete pushbutton;
}

void MainWindow::setM4State()
{
    bool state = getM4State();
    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QByteArray buf[2] = {"start", "stop"};
    if(state)
        file.write(buf[1]);
    else
        file.write(buf[0]);

    file.close();
}

bool MainWindow::getM4State()
{
    file.setFileName("/sys/class/remoteproc/remoteproc0/state");
    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QTextStream in(&file);
    QString buf = in.readLine();
    qDebug()<<"state: "<<buf<<endl;
    file.close();
    if(buf == "running"){
        pushbutton->setText("Start M4");
        return true;
    }else{
        pushbutton->setText("Stop M4");
        return false;
    }
}

void MainWindow::pushButtonClicked()
{
    setM4State();
}

