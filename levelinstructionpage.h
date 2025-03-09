/**
 * This class is window which appears upon the initiation of
 * each of the game levels.
 *
 * It teaches the player some radio basics which may pertain to the
 * level they are about to play, provides guidance and establishes an
 * objective for the level, and provides access to the main game menu.
 *
 * This window may be dismissed and the recalled during level play.
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

class LevelInstructionPage : public QWidget
{
    Q_OBJECT

public:
    QTextEdit* textEdit;

    LevelInstructionPage(QWidget *parent = nullptr, int levelNumber = 0);

    /**
    * @brief Change the text and image to reflect the new level.
    * @param levelNumber
    */
    void changeInstructions(int levelNumber);

    /**
     * @brief onContinueButtonClicked
     */
    void onContinueButtonClicked();

signals:
    void continueButtonClicked();

};

#endif // LEVELINSTRUCTIONPAGE_H
