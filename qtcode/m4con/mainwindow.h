#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFile>
#include <QGuiApplication>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QFile file;
    QPushButton *pushbutton;
    void setM4State();
    bool getM4State();
private slots:
    void pushButtonClicked();
};
#endif // MAINWINDOW_H
