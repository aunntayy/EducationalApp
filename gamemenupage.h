/**
 * This class is window which appears upon the initiation of the app and
 * (optionally) between the game levels.
 *
 * It provides the player access to the different levels of game play as
 * well as giving a brief introduction to the content that will be learned.
 *
 * This window may be dismissed and the recalled during level play.
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

class GameMenuPage : public QWidget
{
    Q_OBJECT

public:
    explicit GameMenuPage(QWidget* parent = nullptr);
    int level;


protected:
    void paintEvent(QPaintEvent* event) override;
    void showAboutInfo();
    void chooseLevel();


signals:
    void startButtonClicked();
    void levelSelected(int level);

};

#endif // GAMEMENUPAGE_H
