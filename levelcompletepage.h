/**
 * @file LevelCompletePage.h
 * @brief This class represents a window that appears upon the completion of each game level.
 * It congratulates the player, displays their score, summarizes the key takeaways from the level,
 * and provides options to proceed to the next level or return to the main game menu.
 *
 * @author: Trentton Stratton, Chanphone Visathip
 * @date: Date of creation or modification
 */

#ifndef LEVELCOMPLETEPAGE_H
#define LEVELCOMPLETEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

// The LevelCompletePage class defines a custom QWidget that is shown at the end of each game level.
class LevelCompletePage : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor for the LevelCompletePage.
     * Initializes the widget with an optional parent and the score achieved in the level.
     * @param parent The parent widget, defaulting to nullptr if not specified.
     * @param score The score achieved in the level, defaulting to 0.
     */
    explicit LevelCompletePage(QWidget* parent = nullptr, int score = 0);

    /**
     * Displays the game menu, allowing the player to return to the main menu.
     */
    void displayGameMenu();

    /**
     * Initiates the transition to the next game level.
     */
    void nextLevel();

    /**
     * Updates the score display on the page.
     * @param score The new score to display.
     */
    void updateScoreDisplay(int score);

protected:
    /**
     * Custom paint event to draw the background of the level completion window.
     * @param event Information about the paint event (not used directly in the method).
     */
    void paintEvent(QPaintEvent* event) override;

signals:
    /**
     * Signal emitted when the game menu button is clicked.
     * Indicates that the user wants to return to the main game menu.
     */
    void gameMenuButtonClicked();

    /**
     * Signal emitted when the next level button is clicked.
     * Indicates that the user wants to proceed to the next level.
     */
    void nextLevelButtonClicked();

private:
    QLabel* scoreLabel;  // Label for displaying the player's score
};

#endif // LEVELCOMPLETEPAGE_H
