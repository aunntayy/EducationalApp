/**
 * @file GameMenuPage.h
 * @brief This class represents the main menu of the game, which is displayed upon the initiation of the app and optionally between game levels.
 * It provides the player with access to different levels of gameplay and a brief introduction to the content that will be covered.
 * This window can be dismissed and recalled during level play.
 *
 * @author: Thu Ha
 * @date: 12/12/2024
 */

#ifndef GAMEMENUPAGE_H
#define GAMEMENUPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <QMessageBox>
#include <QMenuBar>
#include <QInputDialog>

// The GameMenuPage class provides the main menu interface for the game.
class GameMenuPage : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor for GameMenuPage.
     * Initializes a new instance of GameMenuPage with an optional parent widget.
     * @param parent The parent widget of this game menu page, defaulting to nullptr if not specified.
     */
    explicit GameMenuPage(QWidget* parent = nullptr);

    // Stores the current selected level number.
    int level;

protected:
    /**
     * Custom paint event to draw the menu's graphical content.
     * @param event Information about the paint event (not used directly in the method).
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * Displays the 'About' information dialog.
     * This method is triggered by a specific user interaction, typically a button press.
     */
    void showAboutInfo();

    /**
     * Allows the player to choose a game level.
     * This method usually presents a dialog or menu from which a level can be selected.
     */
    void chooseLevel();

signals:
    /**
     * Signal emitted when the start button is clicked.
     * It indicates that the user wishes to begin gameplay.
     */
    void startButtonClicked();

    /**
     * Signal emitted when a level is selected from the menu.
     * @param level The game level number that has been selected.
     */
    void levelSelected(int level);

};

#endif // GAMEMENUPAGE_H
