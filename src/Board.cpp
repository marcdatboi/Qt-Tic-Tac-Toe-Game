



#include <QVariantAnimation>
#include <QColor>
#include <QTextEdit>
#include <iostream>
#include <QTimer>
#include <QVariant>
#include "Board.hpp"

#include "MainWindow.hpp"
/*
 * TODO LIST:
 * 1. Fix tie's happening whenever a winner is found. - DONE
 * 2. Figure out how to move the board onto a separate widget
 * so you can make different screens.
 * 3. Add functionality to replay button
 *
 * Expected results:
 * 1. Ties should not happen whenever the board finds a winner OmO
 * 2. Game should transition from board to winning screen. :<
 * 3. Replay button should work like a replay button 'm'
*/

Board::Board(QChar player1, QChar player2, QWidget *parent)
    : QWidget(parent)
    , boardWidget(new QWidget(this))
    , screens(new QStackedWidget(this))

    // First Screen
    , player1(player1)
    , player2(player2)
    , currentPlayerSymbol(player1)
    , gameBoardLayout(new QVBoxLayout(this->boardWidget))
    , boardLayout(new QGridLayout())
    , hasWon(false)
    , hasTieHappened(false)

    // Second Screen
    , winningScreen(new QWidget(this))
    , winningScreenLayout(new QVBoxLayout(this->winningScreen))
{

    // Class layout setup
    QVBoxLayout *classWidgetLayout = new QVBoxLayout(this);
    classWidgetLayout->addWidget(this->screens);

    // Setting up widget and layout
    this->setupButtons(); // Set up buttons
    this->setupWinningScreen(); // Set up winning screen

    // Set up screens
    this->screens->addWidget(this->boardWidget);
    this->screens->addWidget(this->winningScreen);

}

// Gameplay
void Board::setupButtons()
{
    /*
     * Initializes buttons on the board and button functionality
     *
     * Return type: None
     * Params: None
     * Exceptions: None
     *
     * Detail:
     * On initialization, the constructor calls this function and appends 9 *QPushButton
     * objects to the 'allButtons' QList private field. This function also initializes and add functionality
     * to the reset button which clears the entire board.
     *
     * We do this to make access to the rows, columns and diagonals easier for checking winners
    */

    // Initializing all buttons
    for (int i = 0; i < 9; i++) {
        this->buttons.append(new QPushButton(this));
        this->buttons[i]->setFixedSize(300, 300);
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


    // --- Setting up rows ---
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

    // All rows
    this->allRows.append(this->row1);
    this->allRows.append(this->row2);
    this->allRows.append(this->row3);


    // --- Setting up columns ---
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

    this->allColumns.append(this->column1);
    this->allColumns.append(this->column2);
    this->allColumns.append(this->column3);


    // --- Diagonals ---

    // Left Right Diagonal
    this->leftRightDiagonal.append(this->buttons[0]);
    this->leftRightDiagonal.append(this->buttons[4]);
    this->leftRightDiagonal.append(this->buttons[8]);

    // Right Left Diagonal
    this->rightLeftDiagonal.append(this->buttons[2]);
    this->rightLeftDiagonal.append(this->buttons[4]);
    this->rightLeftDiagonal.append(this->buttons[6]);

    // All diagonals
    this->allDiagonals.append(this->leftRightDiagonal);
    this->allDiagonals.append(this->rightLeftDiagonal);

    // Connect all buttons to a clicked signal
    for (auto& button : this->buttons) {
        connect(button, &QPushButton::clicked, [this, button]() {
            this->handleButtonClick(button);
        });
    }

    // Add the grid to the main layout
    this->gameBoardLayout->addLayout(this->boardLayout, 1);


    // --- Creating reset button ---
    this->resetButton = new QPushButton("Reset Board", this);
    this->resetButton->setFixedSize(500, 75);
    this->gameBoardLayout->addWidget(this->resetButton, 0, Qt::AlignCenter);

    // Reset button style sheetz
    this->resetButton->setStyleSheet(QString(
        "QPushButton { font-size: 40px; }"
    ));

    // Clears the entire board when pressed
    connect(this->resetButton, &QPushButton::clicked, [this]() {
        this->clearBoard();
    });
}

void Board::handleButtonClick(QPushButton* buttonObj)
{
    /*
     * Sets the target buttons text to a QChar and doesnt allow overwriting as well
     * as checking for a winner with every move made
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
                QString style = QString("QPushButton { background-color: %1; font-size: 90px; color: %2; }").arg(color.name(), currentColor);
                buttonObj->setStyleSheet(style);
            }
        });
        redButtonFadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        return;
    }

    if (this->currentPlayerSymbol == this->player1) { // Player 1 to player 2
        buttonObj->setStyleSheet("font-size: 90px; color: red;");
        buttonObj->setText(this->currentPlayerSymbol);

        this->currentPlayerSymbol = this->player2;
        this->player1MoveCount++;
    }
    else { // Player 2 to player 1
        buttonObj->setStyleSheet("font-size: 90px; color: blue;");
        buttonObj->setText(this->currentPlayerSymbol);

        this->currentPlayerSymbol = this->player1;
        this->player2MoveCount++;
    }
    this->checkForWinner();
}

void Board::checkForWinner()
{


    // Check for winners
    this->checkRows();
    this->checkColumns();
    this->checkDiagonals();

    if (this->hasWon) { // Swap to next screen if a winner is found
        this->roundsPlayed++;
        this->totalWins++;
        this->screens->setCurrentIndex(1);
    }
    else {
        // Automatically stops checking whenever a tie is found.
        this->checkTie();
        if (this->hasTieHappened) {
            this->roundsPlayed++;
            this->tieCount++;
        }
    }


}

void Board::clearBoard() {
    for (auto& button : this->buttons) {
        button->setText("");
    }
    this->hasWon = false;
    this->hasTieHappened = false;
}

void Board::setupWinningScreen()
{
    // --- Setting up timers ---
    this->timerDisplay = new QLabel("0 Hours | 0 Minutes | 0 Seconds");

    this->gameplayTime = new QElapsedTimer();
    this->gameplayTime->start(); // start the timer

    this->timeUpdater = new QTimer(this);
    this->timeUpdater->start(1000); // Update every second
    connect(this->timeUpdater, &QTimer::timeout, this, &Board::updateTimerDisplay);


    // --- Winning message stuff ---
    QString winningStats = QString("--- STATS ---\n\n"
        "Rounds Played: %1\n"
        "Player %2 Moves: %3\n"
        "Player %4 Moves: %5\n"
        "Total Ties: %6\n"
        "Total Wins: %7")
        .arg(this->roundsPlayed)
        .arg(this->player1).arg(this->player1MoveCount)
        .arg(this->player2).arg(this->player2MoveCount)
        .arg(this->tieCount)
        .arg(this->totalWins);


    QString winningText = "🎉 Player " + QString(this->currentPlayerSymbol) + " has won the game! 🎉";

    QTextEdit *winningMessage = new QTextEdit(winningText,this);
    winningMessage->setReadOnly(true);

    QTextEdit *winningStatsObj = new QTextEdit(this);
    winningStatsObj->setPlainText(winningStats);
    winningStatsObj->setReadOnly(true);

    // Style sheets for stats (and centering too)
    winningStatsObj->setStyleSheet(
        "QTextEdit {"
        "   font-size: 22px;"          // Makes the text big and readable
        "   font-weight: bold;"        // Optional: makes it pop
        "   color: #FFFFFF;"           // White text (or choose your color)
        "   line-height: 150%;"        // Adds a bit of breathing room between lines
        "   background-color: transparent;" // Makes it blend into your screen
        "   border: none;"             // Removes the text box border
        "   text-align: center;"       // Aligns text towards the center of the QTextEdit object
        "}"
    );
    winningStatsObj->selectAll();
    winningStatsObj->setAlignment(Qt::AlignCenter);

    // Get rid of highlights
    QTextCursor cursor = winningStatsObj->textCursor();
    cursor.clearSelection();
    winningStatsObj->setTextCursor(cursor);

    // Create play again button with logic
    QPushButton *playAgainButton = new QPushButton("Play Again?", this); // Sets the screen back to board screen when clicked


    // --- Adding everything to layout ---
    this->winningScreenLayout->addWidget(winningMessage);
    this->winningScreenLayout->addWidget(winningStatsObj);
    this->winningScreenLayout->addWidget(playAgainButton);

    // Add layout to widget
    this->winningScreen->setLayout(this->winningScreenLayout);
}


// --- Helpers ---
void Board::updateTimerDisplay()
{
    qint64 elapsedMilliseconds = this->gameplayTime->elapsed();

    int seconds = (elapsedMilliseconds / 1000) % 60;
    int minutes = (elapsedMilliseconds / (1000 * 60)) % 60;
    int hours = (elapsedMilliseconds / (1000 * 60 * 60));

    QString text = QString("%1 Hours | %2 Minutes | %3 Seconds")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
    this->timerDisplay->text() = text; // Update the text
}

void Board::checkRows()
{
    for (int i = 0; i < this->allRows.length(); i++) {
        int streakCount = 0; // If this reaches 3, a winner is found
        QString characterToCheck = this->allRows[i][0]->text();

        for (int j = 0; j < this->allRows[i].length(); j++) {
            if (this->allRows[i][j]->text() != characterToCheck) return;
            streakCount++;

            if (streakCount == 3) {
                std::cout << "Found winner! R\n";
                this->hasWon = true;
                streakCount = 0;
            }
        }
    }

}

void Board::checkColumns()
{
    for (int i = 0; i < this->allColumns.length(); i++) {
        int streakCount = 0; // If this reaches 3, a winner is found
        QString characterToCheck = this->allColumns[i][0]->text();

        for (int j = 0; j < this->allColumns[i].length(); j++) {
            if (this->allColumns[i][j]->text() != characterToCheck) return;
            streakCount++;

            if (streakCount == 3) {
                std::cout << "Found winner! C\n";
                this->hasWon = true;
                streakCount = 0;
            }
        }
    }

}

void Board::checkDiagonals()
{
    for (int i = 0; i < this->allDiagonals.length(); i++) {
        int streakCount = 0; // If this reaches 3, a winner is found
        QString characterToCheck = this->allDiagonals[i][0]->text();

        for (int j = 0; j < this->allDiagonals[i].length(); j++) {
            if (this->allDiagonals[i][j]->text() != characterToCheck) return;
            streakCount++;

            if (streakCount == 3) {
                std::cout << "Found winner! D\n";
                this->hasWon = true;
                streakCount = 0;
            }
        }
    }

}

void Board::checkTie()
{
    for (auto& button : this->buttons) {
        if (button->text().isEmpty()) return;
    }
    std::cout << "Tie happened!\n";
    this->hasTieHappened = true;
    this->tieCount++;
}

void Board::setCurrentPlayerSymbol(QChar character) { this->currentPlayerSymbol = character; }
QWidget* Board::getSecondScreenWidget() { return this->winningScreen; }
QGridLayout* Board::getLayout() { return this->boardLayout; }
