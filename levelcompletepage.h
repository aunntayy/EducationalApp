/**
 * This class is a window that appears upon the completion of
 * each of the game levels.
 *
 * It congratulates the player, displays their score, summarizes
 * what was learned, and provides access to the next game level (or
 * the main game menu).
 */

#ifndef LEVELCOMPLETEPAGE_H
#define LEVELCOMPLETEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class LevelCompletePage : public QWidget
{
    Q_OBJECT

public:
    explicit LevelCompletePage(QWidget *parent = nullptr, int score = 0);
    void displayGameMenu(); // Triggers transition to the game menu
    void nextLevel();       // Triggers transition to the next level
    void updateScoreDisplay(int score);

protected:
    void paintEvent(QPaintEvent* event) override; // Custom background painting

signals:
    void gameMenuButtonClicked();  // Signal for returning to the main menu
    void nextLevelButtonClicked(); // Signal for progressing to the next level
private:
    QLabel* scoreLabel;
};

#endif // LEVELCOMPLETEPAGE_H
