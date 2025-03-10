/**
 * This class is a window that appears upon the completion of
 * each of the game levels.
 *
 * It congratulates the player, displays their score, summarizes
 * what was learned, and provides access to the next game level (or
 * the main game menu).
 */

#include "levelcompletepage.h"
#include <QPainter>

LevelCompletePage::LevelCompletePage(QWidget *parent, int score) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(50, 50, 50, 50);

    QLabel* titleLabel = new QLabel("YOU DID IT!", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 32px; color: white; font-weight: bold;");

    // Display score
    scoreLabel = new QLabel(this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet("font-size: 18px; color: white;");
    updateScoreDisplay(score);

    // Buttons
    QPushButton* mainMenuButton = new QPushButton("Main Menu", this);
    // mainMenuButton->setStyleSheet(
    //     "font-size: 18px; background-color: green; color: white; padding: 10px; border-radius: 5px;"
    //     "QPushButton:hover { background-color: #28a745; }"
    //     "QPushButton:pressed { background-color: #1e7e34; }"
    //     );
    connect(mainMenuButton, &QPushButton::clicked, this, &LevelCompletePage::displayGameMenu);

    QPushButton* continueButton = new QPushButton("Next Level", this);
    // continueButton->setStyleSheet(
    //     "font-size: 18px; background-color: blue; color: white; padding: 10px; border-radius: 5px;"
    //     "QPushButton:hover { background-color: #007bff; }"
    //     "QPushButton:pressed { background-color: #0056b3; }"
    //     );
    connect(continueButton, &QPushButton::clicked, this, &LevelCompletePage::nextLevel);

    // Add widgets to the layout
    layout->addWidget(titleLabel);
    layout->addWidget(scoreLabel);
    layout->addWidget(mainMenuButton);
    layout->addWidget(continueButton);
    layout->setAlignment(Qt::AlignCenter);

    this->setLayout(layout);
}

void LevelCompletePage::updateScoreDisplay(int score)
{
    scoreLabel->setText(QString("Your Score: <b>%1</b>").arg(score));
}

void LevelCompletePage::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap background(":/img/levelcomplete.png"); // Replace with your image file path
    painter.drawPixmap(0, 0, width(), height(), background); // Draw and scale background to fit window
}

void LevelCompletePage::displayGameMenu()
{
    emit gameMenuButtonClicked();
}

void LevelCompletePage::nextLevel()
{
    emit nextLevelButtonClicked();
}
