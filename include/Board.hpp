



#include <QLayout>
#include <QPushButton>
#include <QList>


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

    // Initialization
    void initializeButtons();
    void setCurrentPlayerSymbol(QChar character);
    QGridLayout* getLayout();


public slots:
    void handleButtonClick(QPushButton* buttonObj);
    void clearBoard();

private:
    QChar player1;
    QChar player2;
    QChar currentPlayerSymbol;

    QVBoxLayout *mainLayout;
    QGridLayout *boardLayout;
    QList<QPushButton*> buttons; // All Buttons on the board

    // Rows
    QList<QPushButton*> row1;
    QList<QPushButton*> row2;
    QList<QPushButton*> row3;

    // Columns
    QList<QPushButton*> column1;
    QList<QPushButton*> column2;
    QList<QPushButton*> column3;

    // Diagonals
    QList<QPushButton*> leftRightDiagonal;
    QList<QPushButton*> rightLeftDiagonal;

    // reset button
    QPushButton *resetButton;

};