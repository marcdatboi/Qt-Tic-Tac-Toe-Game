



#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP


// Imports
#include <QMainWindow>
#include <QLayout>
#include <QWidget>
#include <QObject>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    // Member functions
    QGridLayout* getLayout();
    QWidget* getBaseWidget();

    ~MainWindow();

private:
    QWidget *baseWidget;
    QGridLayout *layout;
};

#endif