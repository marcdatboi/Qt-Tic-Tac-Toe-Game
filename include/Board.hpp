



#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QList>
#include <QElapsedTimer>
#include <QStackedWidget>


class Board : public QWidget
{
    /*
     * Controls all the buttons and arranges them in a 3x3 grid during
     * gameplay.
     *
     * Stops users from clicking spots if they're already occupied.
     * This class has its own widget and layout!
    */
    Q_OBJECT

public:
    Board(QChar player1, QChar player2, QWidget *parent = nullptr);


    // --- First Screen ---

    // Initialization
    void setupButtons();

    // Gameplay
    void setCurrentPlayerSymbol(QChar character); // Sets the current players turn
    void checkForWinner(); // Check for a winner after every turn

    // Helpers
    void checkRows();
    void checkColumns();
    void checkDiagonals();
    void checkTie();

    // Getters
    QGridLayout* getLayout();


    // --- Second Screen ---
    void setupWinningScreen();
    QWidget* getSecondScreenWidget();

public slots:


    // --- First Screen ---
    void handleButtonClick(QPushButton* buttonObj);
    void clearBoard();


private slots:


    // --- Second Screen ---
    void updateTimerDisplay();

private:
    QStackedWidget *screens;


    // --- First Screen ---
    QWidget *boardWidget;
    QChar player1;
    QChar player2;
    QChar currentPlayerSymbol;
    bool hasWon;
    bool hasTieHappened;

    QVBoxLayout *gameBoardLayout; // This layout contains the grid and reset button
    QGridLayout *boardLayout; // This is the board you interact with
    QList<QPushButton*> buttons; // All Buttons on the board


    // Rows
    QList<QPushButton*> row1;
    QList<QPushButton*> row2;
    QList<QPushButton*> row3;
    QList<QList<QPushButton*>> allRows;


    // Columns
    QList<QPushButton*> column1;
    QList<QPushButton*> column2;
    QList<QPushButton*> column3;
    QList<QList<QPushButton*>> allColumns;


    // Diagonals
    QList<QPushButton*> leftRightDiagonal;
    QList<QPushButton*> rightLeftDiagonal;
    QList<QList<QPushButton*>> allDiagonals;

    // reset button
    QPushButton *resetButton;


    // --- Second Screen ---
    QWidget *winningScreen;
    QVBoxLayout *winningScreenLayout;

    // Gameplay stats
    QElapsedTimer *gameplayTime;
    QLabel *timerDisplay;
    QTimer *timeUpdater;
    int roundsPlayed;
    int player1MoveCount;
    int player2MoveCount;
    int tieCount;
    int totalWins;
};
