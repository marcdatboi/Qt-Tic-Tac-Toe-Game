



#include <QVariantAnimation>
#include <QColor>
#include <QVariant>
#include "Board.hpp"

Board::Board(QChar player1, QChar player2, QWidget *parent)
    : QWidget(parent)
    , player1(player1)
    , player2(player2)
    , currentPlayerSymbol(player1)
    , mainLayout(new QVBoxLayout(this))
    , boardLayout(new QGridLayout())
{

    // Setting up widget and layout
    this->initializeButtons(); // Set up buttons
}

void Board::initializeButtons()
{
    /*
     * Initializes button status for rows, columns and diagonals as well as
     * make buttons clickable
     *
     * Return type: None
     * Params: None
     * Exceptions: None
     *
     * Detail:
     * On initialization, the constructor calls this function and appends 9 *QPushButton
     * objects to the 'allButtons' QList private field.
     *
     * We do this to make access to the rows, columns and diagonals easier for checking winners
    */

    // Initializing all buttons
    for (int i = 0; i < 9; i++) {
        this->buttons.append(new QPushButton(this));
        this->buttons[i]->setFixedSize(200, 200);
        this->buttons[i]->setStyleSheet(QString(
            "QPushButton: { background-color: gray; }"
            "QPushButton:hover { background-color: #555555; }"
        ));
    }

    int row = 0;
    int col = 0;
    for (int i = 0; i < this->buttons.length(); i++) {
        if (col == 3) {
            col = 0;
            row++;
        }

        this->buttons[i]->setMinimumSize(100, 100);
        this->boardLayout->addWidget(this->buttons[i], row, col);
        col++;

    }


    // Setting up rows
    for (int i = 0; i < 9; i++) {

        // Third row
        if (i >= 6) {
            this->row3.append(this->buttons[i]);
        }
        else if (i >= 3) { // 2nd row
            this->row2.append(this->buttons[i]);
        }
        else { // 1st row
            this->row1.append(this->buttons[i]);
        }
    }

    // Setting up columns
    int checkpoint = 0;
    int index = 0;
    while (true) {

        // 1st Column
        if (checkpoint == 0) {

            if (index > 6) {
                checkpoint = 1;
                index = 1;
                continue;
            }
            this->column1.append(this->buttons[index]);
        }

        // 2nd Column
        else if (checkpoint == 1) {

            if (index > 7) {
                checkpoint = 2;
                index = 2;
                continue;
            }
            this->column2.append(this->buttons[index]);
        }

        // 3rd Column
        else if (checkpoint == 2) {

            if (index > 8) break;
            this->column3.append(this->buttons[index]);
        }
        index += 3;
    }


    // --- Diagonals ---

    // Left Right Diagonal
    this->leftRightDiagonal.append(this->buttons[0]);
    this->leftRightDiagonal.append(this->buttons[4]);
    this->leftRightDiagonal.append(this->buttons[8]);

    // Right Left Diagonal
    this->rightLeftDiagonal.append(this->buttons[2]);
    this->rightLeftDiagonal.append(this->buttons[4]);
    this->rightLeftDiagonal.append(this->buttons[6]);

    // Connect all buttons to a clicked signal
    for (auto& button : this->buttons) {
        connect(button, &QPushButton::clicked, [this, button]() {
            this->handleButtonClick(button);
        });
    }
    // Add the grid to the main layout
    this->mainLayout->addLayout(this->boardLayout, 1);


    // Creating reset button
    this->resetButton = new QPushButton("Reset Board", this);
    this->mainLayout->addWidget(this->resetButton);
    this->resetButton->setMinimumSize(200, 50);
    connect(this->resetButton, &QPushButton::clicked, [this]() {
        this->clearBoard();
    });
}
void Board::handleButtonClick(QPushButton* buttonObj) {
    /*
     * Sets the target buttons text to a QChar and doesnt allow overwriting
     *
     * Return type: None
     * Params: QPushButton*
     * Exceptions thrown: None
     *
     * Detail:
     * If the user tries pressing a button and the spot is empty, the function
     * will change the text on the button to a QChar object.
     *
     * If a QChar object is already present, the function will return early to stop
     * anyone from overwriting the spot and turn the button that was attempted to be overriden a
     * red color.
    */

    // Disallow the user to overwrite anything already there
    if (!buttonObj->text().isEmpty()) {

        // Setting up animation
        QVariantAnimation *redButtonFadeAnimation = new QVariantAnimation(this);
        redButtonFadeAnimation->setDuration(500);
        redButtonFadeAnimation->setStartValue(QColor("indianred"));
        redButtonFadeAnimation->setEndValue(QColor(51, 51, 51));
        QString currentColor = buttonObj->text() == this->player1 ? "red" : "blue"; // Yes this is intentional to keep colors consistent.

        connect(redButtonFadeAnimation, &QVariantAnimation::valueChanged, this, [=](const QVariant &value) {
            QColor color = value.value<QColor>();
            if (color.isValid()) {
                // Only update the background-color part
                QString style = QString("QPushButton { background-color: %1; font-size: 60px; color: %2; }").arg(color.name(), currentColor);
                buttonObj->setStyleSheet(style);
            }
        });
        redButtonFadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        return;
    }

    if (this->currentPlayerSymbol == this->player1) { // Player 1 to player 2
        buttonObj->setStyleSheet("font-size: 60px; color: red;");
        buttonObj->setText(this->currentPlayerSymbol);

        this->currentPlayerSymbol = this->player2;
    }
    else { // Player 2 to player 1
        buttonObj->setStyleSheet("font-size: 60px; color: blue;");
        buttonObj->setText(this->currentPlayerSymbol);

        this->currentPlayerSymbol = this->player1;
    }
}

void Board::setCurrentPlayerSymbol(QChar character) { this->currentPlayerSymbol = character; }

void Board::clearBoard() {
    for (auto& button : this->buttons) {
        button->setText("");
    }
}

QGridLayout* Board::getLayout() { return this->boardLayout; }
