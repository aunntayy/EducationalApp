/**
 * @file LevelInstructionPage.h
 * @brief This class represents a window that appears at the initiation of each game level.
 * It provides players with basic information about radio technology related to the level,
 * offers guidance, sets objectives for the gameplay, and allows access to the main game menu.
 * This window can be dismissed and recalled during level play.
 *
 * @author: Phuc Hoang, Thu Ha.
 * @date: 12/12/2024
 */

#ifndef LEVELINSTRUCTIONPAGE_H
#define LEVELINSTRUCTIONPAGE_H

#include <QWidget>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtextedit.h>
#include <QDialog>

// The LevelInstructionPage class provides a custom QWidget that displays instructions and objectives for a game level.
class LevelInstructionPage : public QWidget
{
    Q_OBJECT

public:
    // Text edit widget for displaying instructional text and images.
    QTextEdit* textEdit;

    /**
     * Constructor for the LevelInstructionPage.
     * Initializes the widget with an optional parent and sets the initial level number.
     * @param parent The parent widget, defaulting to nullptr if not specified.
     * @param levelNumber The initial level number to display instructions for, defaulting to 0.
     */
    LevelInstructionPage(QWidget* parent = nullptr, int levelNumber = 0);

    /**
     * Updates the instruction text and images according to the specified level number.
     * @param levelNumber The level number for which to update instructions.
     */
    void changeInstructions(int levelNumber);

    /**
     * Handles the actions to perform when the continue button is clicked.
     * Typically this function will emit a signal to notify other components to proceed with gameplay.
     */
    void onContinueButtonClicked();

signals:
    /**
     * Signal emitted when the continue button is clicked.
     * This is used to indicate that the player has finished reading the instructions and is ready to start the level.
     */
    void continueButtonClicked();
};

#endif // LEVELINSTRUCTIONPAGE_H
