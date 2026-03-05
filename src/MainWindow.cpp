



// Imports
#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->layout = new QGridLayout(this);
    this->baseWidget = new QWidget(this);
    this->baseWidget->setLayout(this->layout);

    // Window Setup
    this->setWindowTitle("Qt Tic-Tac-Toe App");
    this->resize(800, 800);

}

QGridLayout* MainWindow::getLayout() { return this->layout; }
QWidget* MainWindow::getBaseWidget() { return this->baseWidget; }
MainWindow::~MainWindow() {};
