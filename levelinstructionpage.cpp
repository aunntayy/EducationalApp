/**
 * This class is a window that appears upon the initiation of
 * each game level.
 *
 * It introduces the player to radio communication basics relevant to
 * the level, provides guidance on game objectives, and grants access
 * to the main game menu.
 *
 * This window may be dismissed and recalled during gameplay.
 */

#include "levelinstructionpage.h"

LevelInstructionPage::LevelInstructionPage(QWidget *parent, int levelNumber) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Create a text edit widget for displaying instructions
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);

    // Customize the font and style
    QFont font;
    font.setFamily("Arial");       // Set font family
    font.setPointSize(20);         // Set font size
    font.setBold(true);            // Set bold style
    textEdit->setFont(font);

    // Set additional styles using stylesheets
    textEdit->setStyleSheet(
        "QTextEdit {"
        "   color: #333333;"      // Set text color
        "   background-color: #f9f9f9;" // Set background color
        "   border: 2px solid #cccccc;" // Add a border
        "   border-radius: 10px;"       // Round the corners
        "   padding: 10px;"             // Add padding
        "}"
        );

    changeInstructions(levelNumber);
    layout->addWidget(textEdit);
    layout->setContentsMargins(10, 10, 10, 10);

    QHBoxLayout* continueBtnLayout = new QHBoxLayout;
    QPushButton* continueButton = new QPushButton("CONTINUE", this);
    continueButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;" // Green button background
        "   color: white;"              // White text
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 10px 20px;"
        "   font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;" // Darker green on hover
        "}"
        "QPushButton:pressed {"
        "   background-color: #3e8e41;" // Even darker green on press
        "}"
        );
    continueButton->setFixedWidth(150);
    continueBtnLayout->addWidget(continueButton, 0, Qt::AlignCenter);
    layout->addLayout(continueBtnLayout);
    connect(continueButton, &QPushButton::clicked, this, &LevelInstructionPage::onContinueButtonClicked);
}

void LevelInstructionPage::changeInstructions(int levelNumber)
{
    QString headerStyle = "color: red; font-size: 18px; font-weight: bold; text-align: center;";
    QString bodyStyle = "font-size: 14px; font-style: italic; color: black;";
    QString listStyle = "font-size: 14px; color: black;";

    QString commonInfo = QString(
        "<div style='%1'>Antenna Types</div>"
        "<ul style='%2'>"
        "<li><b>Dish Antenna:</b> Highly focused beam, ideal for precise, long-range communication.</li>"
        "<li><b>Yagi Antenna:</b> Moderately focused, suitable for mid-range communication with balanced reach.</li>"
        "<li><b>Dipole Antenna:</b> Omnidirectional, great for short-range communication.</li>"
        "</ul>"
        "<div style='%1'>Frequency Bands</div>"
        "<ul style='%2'>"
        "<li><b>HF (High Frequency):</b> Reflects off the ionosphere for long-distance communication.</li>"
        "<li><b>VHF (Very High Frequency):</b> Line-of-sight communication; can penetrate moderate obstacles.</li>"
        "<li><b>UHF (Ultra High Frequency):</b> Short-range, high-bandwidth communication.</li>"
        "<li><b>SHF (Super High Frequency):</b> Highly directional, often used for satellite links.</li>"
        "</ul>"
    ).arg(headerStyle, listStyle);

    switch (levelNumber)
    {
    case 1:
        textEdit->setHtml(
            QString("<div style='%1'>Level 1: Understanding Transmit Power</div>"
                    "<p style='%2'>"
                    "In this level, there are no obstacles. Adjust the transmit power and antenna height to ensure your signal travels across the screen "
                    "to reach your friend. Use the minimum power necessary for optimal efficiency."
                    "</p>"
                    "%3")
                .arg(headerStyle, bodyStyle, commonInfo)
        );
        break;
    case 2:
        textEdit->setHtml(
            QString("<div style='%1'>Level 2: Antenna Types</div>"
                    "<p style='%2'>"
                    "Experiment with different antenna types (dish, yagi, and dipole) to see their effects. "
                    "Select the one with the highest gain to reach your target efficiently."
                    "</p>"
                    "%3")
                .arg(headerStyle, bodyStyle, commonInfo)
        );
        break;
    case 3:
        textEdit->setHtml(
            QString("<div style='%1'>Level 3: Antenna Height</div>"
                    "<p style='%2'>"
                    "Adjust the antenna height to ensure your signal clears obstacles like hills and reaches the target."
                    " Use the minimum height necessary to conserve resources."
                    "</p>"
                    "%3")
                .arg(headerStyle, bodyStyle, commonInfo)
        );
        break;
    case 4:
        textEdit->setHtml(
            QString("<div style='%1'>Level 4: HF Skywave Propagation</div>"
                    "<p style='%2'>"
                    "Learn to use HF frequencies for long-distance communication by bouncing signals off the ionosphere. "
                    "Adjust angles and frequencies for atmospheric reflection."
                    "</p>"
                    "%3")
                .arg(headerStyle, bodyStyle, commonInfo)
        );
        break;
    case 5:
        textEdit->setHtml(
            QString("<div style='%1'>Level 5: Navigating Obstacles</div>"
                    "<p style='%2'>"
                    "Analyze the environment to select the appropriate frequency and antenna type for overcoming obstacles."
                    " Balance efficiency and signal strength."
                    "</p>"
                    "%3")
                .arg(headerStyle, bodyStyle, commonInfo)
        );
        break;
    default:
        break;
    }
}

void LevelInstructionPage::onContinueButtonClicked()
{
    emit continueButtonClicked();
    qDebug() << "Instruction window continue signal triggered." << Qt::endl;
}
