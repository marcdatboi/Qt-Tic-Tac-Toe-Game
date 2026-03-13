



// Imports
#include "MainWindow.hpp"
#include "Board.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->baseWidget = new QWidget(this);
    this->layout = new QVBoxLayout(this->baseWidget);


    this->setCentralWidget(baseWidget);
    this->baseWidget->setLayout(this->layout);

    // Window Setup
    this->setWindowTitle("Qt Tic-Tac-Toe App");
    this->setFixedSize(800, 800);

    Board *board = new Board('X', 'O', this);
    board->setMinimumSize(500, 500);
    this->layout->addWidget(board, 0, Qt::AlignCenter);
}

QVBoxLayout* MainWindow::getLayout() { return this->layout; }
QWidget* MainWindow::getBaseWidget() { return this->baseWidget; }
MainWindow::~MainWindow() {};
