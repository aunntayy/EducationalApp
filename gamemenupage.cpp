/**
 * This class is window which appears upon the initiation of the app and
 * (optionally) between the game levels.
 *
 * It provides the player access to the different levels of game play as
 * well as giving a brief introduction to the content that will be learned.
 *
 * This window may be dismissed and the recalled during level play.
 */

#include "gamemenupage.h"

GameMenuPage::GameMenuPage(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    QVBoxLayout* menuLayout = new QVBoxLayout(this);
    menuLayout->setAlignment(Qt::AlignCenter);
    QPushButton* loadButton = new QPushButton("Choose Level", this);
    loadButton->setStyleSheet(
        "width: 300px;"
        "height: 80px;"
        "font-size: 18px;"
        "background-color: green;"
        "border-radius: 2px;"
        "color: white;"
        "padding: 10px;"
    );
    connect(loadButton, &QPushButton::clicked, this, &GameMenuPage::chooseLevel);
    menuLayout->addWidget(loadButton);
    QPushButton* startButton = new QPushButton("START", this);
    startButton->setStyleSheet(
        "width: 300px;"
        "height: 150px;"
        "font-size: 50px;"
        "background-color: green;"
        "border-radius: 2px;"
        "color: white;"
        "padding: 10px;");
    connect(startButton, &QPushButton::clicked, this, &GameMenuPage::startButtonClicked);
    menuLayout->addWidget(startButton);

    //QHBoxLayout* aboutBtnLayout = new QHBoxLayout(this);
    //aboutBtnLayout->setAlignment(Qt::AlignRight);
    QPushButton* aboutButton = new QPushButton("About", this);
    aboutButton->setFixedSize(50, 50);
    aboutButton->move(1140, 840);
    aboutButton->setStyleSheet(
        "font-size: 12px;"
        "background-color: white;"
        "border-radius: 2px;"
        "color: black;"
        "padding: 2px;"
    );
    connect(aboutButton, &QPushButton::clicked, this, &GameMenuPage::showAboutInfo);

    layout->addLayout(menuLayout);
    this->setLayout(layout);
}

void GameMenuPage::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap background(":/img/gamemenupage.png"); // Load background image from resources
    painter.drawPixmap(0, 0, width(), height(), background); // Scale to widget size
}

void GameMenuPage::showAboutInfo()
{
    QString aboutText = "<div style='background-color: #1e1e1e; color: #f0f0f0; padding: 20px; border-radius: 10px;'>"
                        "<h3 style='text-align: center; color: #ff6347;'>About the Application</h3>"
                        "<p>This application is designed to educate users on radio wave propagation and its interaction with the environment. "
                        "The game explores concepts like antenna types, frequency bands, and practical applications of radio communication.</p>"

                        "<h4 style='color: #ffa500;'>Level Information:</h4>"
                        "<ul>"
                        "<li><b>Level 1:</b> <i>Understanding Transmit Power</i><br>"
                        "Learn how transmit power affects the reach of radio waves.</li>"
                        "<li><b>Level 2:</b> <i>Exploring Antenna Types</i><br>"
                        "Experiment with dish, yagi, and dipole antennas to optimize communication.</li>"
                        "<li><b>Level 3:</b> <i>Adjusting Antenna Height</i><br>"
                        "Understand the importance of antenna height for overcoming obstacles.</li>"
                        "<li><b>Level 4:</b> <i>HF Skywave Propagation</i><br>"
                        "Discover how HF signals reflect off the ionosphere for long-distance communication.</li>"
                        "<li><b>Level 5:</b> <i>Navigating Obstacles</i><br>"
                        "Analyze environments and adapt strategies to overcome signal interference.</li>"
                        "</ul>"

                        "<h4 style='color: #ffa500;'>General Concepts:</h4>"
                        "<p><b>Antenna Types:</b></p>"
                        "<ul>"
                        "<li><b>Dish Antenna:</b> Highly focused, ideal for long-range communication.</li>"
                        "<li><b>Yagi Antenna:</b> Balanced focus, suitable for mid-range communication.</li>"
                        "<li><b>Dipole Antenna:</b> Omnidirectional, effective for short-range communication.</li>"
                        "</ul>"
                        "<p><b>Frequency Bands:</b></p>"
                        "<ul>"
                        "<li><b>HF:</b> Reflects off the ionosphere for global reach.</li>"
                        "<li><b>VHF:</b> Ideal for line-of-sight communication with moderate penetration.</li>"
                        "<li><b>UHF:</b> Short-range, high-capacity communication.</li>"
                        "<li><b>SHF:</b> Highly directional, used for satellite links.</li>"
                        "</ul>"

                        "<h4 style='color: #ffa500;'>Course Information:</h4>"
                        "<p>Course: <b>CS3505</b><br>"
                        "Assignment: <b>A9 Educational Game</b></p>"

                        "<h4 style='color: #ffa500;'>Authors:</h4>"
                        "<p>Thu Ha<br>"
                        "Chanphone Visathip<br>"
                        "Phuc Hoang<br>"
                        "Trenton Stratton</p>"
                        "</div>";

    QDialog aboutDialog(this);
    aboutDialog.setWindowTitle("About the Game");
    aboutDialog.setStyleSheet("background-color: #1e1e1e; color: #f0f0f0; border-radius: 10px;");

    QVBoxLayout* layout = new QVBoxLayout(&aboutDialog);
    QLabel* label = new QLabel(aboutText);
    label->setWordWrap(true); // Ensure text wraps to fit the dialog
    label->setStyleSheet("background-color: #1e1e1e; color: #f0f0f0; padding: 10px;");
    layout->addWidget(label);

    QPushButton* closeButton = new QPushButton("Close");
    closeButton->setStyleSheet(
        "background-color: #555555; color: #f0f0f0; border: 1px solid #888888; padding: 5px 10px; border-radius: 5px;"
        "font-size: 14px;"
        "QPushButton:hover { background-color: #777777; }"
        "QPushButton:pressed { background-color: #444444; }"
        );
    closeButton->setFixedSize(100, 30);
    layout->addWidget(closeButton, 0, Qt::AlignCenter);

    connect(closeButton, &QPushButton::clicked, &aboutDialog, &QDialog::accept);

    aboutDialog.exec();
}

void GameMenuPage::chooseLevel()
{
    // Show a level selection dialog
    QStringList levels = {"Level 1", "Level 2", "Level 3", "Level 4", "Level 5"};
    bool ok;
    QString selectedLevel = QInputDialog::getItem(this, "Choose Level", "Select a level to play:", levels, 0, false, &ok);

    if (ok && !selectedLevel.isEmpty()) {
        level = levels.indexOf(selectedLevel)+1;
        emit levelSelected(level);
        qDebug() << "menu level select: " << level << Qt::endl;
    }
}
