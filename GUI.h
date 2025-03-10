/**
 * This class is the view for the app.
 *
 * It is responsible for displaying different game windows and
 * as well as the level being played.
 *
 * @author: Trentton Stratton, Phuc Hoang, Chanphone Visathip, Thu Ha.
 * @date: 12/12/2024
 */

#ifndef GUI_H
#define GUI_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QDial>
#include <QDebug>

#include "environment.h"
#include "gamemenupage.h"
#include "levelinstructionpage.h"
#include "levelcompletepage.h"
#include "gamemenupage.h"
#include "model.h"
//#include "levelworldpage.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // all are called by main.cpp

    /**
     * @brief Creates game control panel
     */
    void createControlPanel();

    /**
    * @brief Displays a window which contains levels that the player can
    * select as well as a quit button to exit the program.
    */
    void displayGameMenu();

    /**
    * @brief Displays a window which contains text/images that teach the player radio
    * principles, the instructions that will help the user pass the level, and a
    * button to dismiss the window.
    *
    * @param level - number representing the level about to be played
    */
    void displayInstructionWindow(int levelNumber);

    /**
    * @brief Displays a window which contains the level world and a panel of player
    * controls.
    * The player controls allow the player to adjust:
    *  -radio wave frequency
    *  -antenna type
    *  -transmit power
    *  -antenna height
    *
    * Additionally, on the control panel is a button to send a radio transmission and
    * a button to return the player to the main game menu.
    */
    void displayLevel(QVector<b2Vec2> position);


    /**
     * @brief Displays objects based on level selected by user.
     * @param vector of different objects
     */
    void displayLevelObjects(QVector<ObjectData> objects);

    /**
    * @brief Redraws the level objects according to the new positions recieved from model.
    *
    * @param vector of body positions
    */
    void updateGameWorld(); // not this one?

    /**
    * @brief Displays a window which congratulates the player, contains text/images that
    * summarizes what has been learned in the level, displays the score attained, and
    * a button to dismiss the window.
    *
    * @param level - number representing the level just played
    */
    void displayLevelCompleteWindow(int levelNumber); // pass in level if we want to summarize what was learned in level

    void updateInstructionWindow(int levelNumber);

    /**
    * @brief Preps the GUI for the new level by displaying a window to the
    * player and drawing the level world in the background.
    *
    * @param levelNumber
    */
    void setupLevel(int levelNumber);

    void updateScore(int points);
    void resetScore();

private:
    Ui::MainWindow *ui;
    Environment* level;
    QStackedWidget* stackedWidget;
    GameMenuPage* gameMenuWindow;
    LevelInstructionPage* levelInstructionWindow;
    LevelCompletePage* levelCompleteWindow;
    QDockWidget* controlPanel = nullptr;
    //LevelWorldPage* levelWorldWindow;
    int score = 0; // Game score

signals:
    void levelSelected(int levelNumber);
    void startLevelClicked(int levelNumber);

    void exitToMenu();
    void gameMenuButtonClicked();
    void instructionButtonClicked();
    void transmitButtonClicked();
    void frequencyBandSelected(const QString& selectedFrequencyBand);
    void antennaTypeSelected(const QString& selectedAntennaType);
    void powerLevelAdjusted(int selectedPowerLevel);
    void antennaHeightAdjusted(int selectedAntennaHeight);
    void antennaOrientationAdjusted(int selectedAntennaOrientation);

    void levelComplete(int score);

    void setupNextLevel(int levelNumber);
private slots:

    /**
    * @brief Actions upon dismissing the instruction window.
    *
    * @param level - number representing the level about to be played
    */
    void dismissInstructions();

};
#endif // GUI_H
