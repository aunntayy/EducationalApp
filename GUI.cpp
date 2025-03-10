/**
 * This class is the view for the app.
 *
 * It is responsible for displaying different game windows and
 * as well as the level being played.
 */

#include "GUI.h"
#include "ui_GUI.h"
#include "environment.h"
#include "gamemenupage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // app window setup
    this->setWindowTitle("Waveform Adventures");
    this->setFixedSize(1200, 900);
    this->move(250, 0);
    this->setStatusBar(nullptr);

    // setup stacked widget and secondary gui windows
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    gameMenuWindow = new GameMenuPage(this);
    stackedWidget->addWidget(gameMenuWindow);

    levelCompleteWindow = new LevelCompletePage(this);
    stackedWidget->addWidget(levelCompleteWindow);

    levelInstructionWindow = new LevelInstructionPage(this);

    // signals/slots for secondary gui windows
    connect(levelInstructionWindow, &LevelInstructionPage::continueButtonClicked, this, &MainWindow::dismissInstructions);
    connect(gameMenuWindow, &GameMenuPage::levelSelected, this, &MainWindow::dismissInstructions);

    connect(levelCompleteWindow, &LevelCompletePage::gameMenuButtonClicked , this, &MainWindow::displayGameMenu);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createControlPanel(){
    if (controlPanel) {return;}
    controlPanel = new QDockWidget("", this);
    controlPanel->setTitleBarWidget(new QWidget());

    controlPanel->setStyleSheet("background-color: rgb(100, 100, 100);");
    controlPanel->setFixedHeight(100);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    controlPanel->setSizePolicy(sizePolicy);
    controlPanel->setFeatures(QDockWidget::NoDockWidgetFeatures);
    controlPanel->setFeatures(QDockWidget::DockWidgetMovable);
    controlPanel->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);

    // panel container
    QWidget *panelContainer = new QWidget();
    panelContainer->setStyleSheet("background-color: rgb(50, 50, 50);");
    controlPanel->setWidget(panelContainer);

    // panel - layout to hold controls
    QHBoxLayout* layout = new QHBoxLayout();

    QPushButton* transmitButton = new QPushButton("TRANSMIT", this);
    transmitButton->setStyleSheet(
        "QPushButton {"
        "  width: 100px;"
        "  height: 80px;"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #d3d3d3, stop:1 #696969);"
        "  border: 1px solid #DDDDDD;"
        "  border-radius: 2px;"
        "  box-shadow: 0px 4px 8px rgba(0, 0, 0, 0.2);"
        "  color: #333333;"
        "  padding: 10px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  box-shadow: 0px 8px 16px rgba(0, 0, 0, 0.3);"
        "}"
        "QPushButton:pressed {"
        "  padding-bottom: 4px;"
        "  box-shadow: 0px 2px 4px rgba(0, 0, 0, 0.4);"
        "  transform: translateY(2px);"
        "}"
        );
    connect(transmitButton, &QPushButton::clicked, this, [=]() {
        emit transmitButtonClicked();
    });

    QVBoxLayout* powerLayout = new QVBoxLayout();
    QLabel* powerLabel = new QLabel("Transmit Power");
    powerLabel->setAlignment(Qt::AlignHCenter);
    QDial* powerDial = new QDial(this);
    powerDial->setRange(1,100);
    powerDial->setWrapping(false);
    powerDial->setNotchesVisible(true);
    connect(powerDial, &QDial::valueChanged, this, [=](int selectedPowerLevel)
                            {
                                emit powerLevelAdjusted(selectedPowerLevel); // Notify Model of the new power level
                            });

    powerLayout->addWidget(powerLabel);
    powerLayout->addWidget(powerDial);

    QVBoxLayout* heightLayout = new QVBoxLayout();
    QLabel* heightLabel = new QLabel("Antenna Height");
    heightLabel->setAlignment(Qt::AlignHCenter);
    QSlider* heightSlider = new QSlider(Qt::Vertical);
    heightSlider->setMinimum(0);
    heightSlider->setMaximum(30);
    heightSlider->setValue(5);
    heightSlider->setTickPosition(QSlider::TicksLeft);
    heightSlider->setTickInterval(5);
    heightSlider->setFixedWidth(100);
    //heightSlider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(heightSlider, &QSlider::valueChanged, this, [=](int selectedAntennaHeight)
    {
        emit antennaHeightAdjusted(selectedAntennaHeight);
    });
    heightLayout->addWidget(heightLabel);
    heightLayout->addWidget(heightSlider);

    QVBoxLayout* orientationLayout = new QVBoxLayout();
    QLabel* orientationLabel = new QLabel("Antenna Orientation");
    orientationLabel->setAlignment(Qt::AlignHCenter);
    QDial* orientationDial = new QDial(this);
    orientationDial->setRange(-90,90); // -x direction, +x direction
    orientationDial->setWrapping(false);
    orientationDial->setNotchesVisible(true);
    connect(orientationDial, &QDial::valueChanged, this, [=](int selectedAntennaOrientation)
    {
        emit antennaOrientationAdjusted(selectedAntennaOrientation);
    });
    orientationLayout->addWidget(orientationLabel);
    orientationLayout->addWidget(orientationDial);

    QVBoxLayout* antennaTypeLayout = new QVBoxLayout();
    QLabel* antennaTypeLabel = new QLabel("Antenna Type");
    antennaTypeLabel->setAlignment(Qt::AlignHCenter);
    QComboBox* antennaSelector = new QComboBox(this);
    antennaSelector->setPlaceholderText("-");
    antennaSelector->addItems({"dish", "yagi", "dipole"});
    connect(antennaSelector, &QComboBox::currentIndexChanged, this, [=](int index)
    {
        QString selectedAntenna = antennaSelector->itemText(index);
        emit antennaTypeSelected(selectedAntenna);
    });
    antennaTypeLayout->addWidget(antennaTypeLabel);
    antennaTypeLayout->addWidget(antennaSelector);

    QVBoxLayout* freqBandLayout = new QVBoxLayout();
    QLabel* freqBandLabel = new QLabel("Frequency Band");
    freqBandLabel->setAlignment(Qt::AlignCenter);
    QComboBox* frequencyBandSelector = new QComboBox(this);
    frequencyBandSelector->setPlaceholderText("-");
    frequencyBandSelector->addItems({"HF", "VHF", "UHF", "SHF"});
    connect(frequencyBandSelector, &QComboBox::currentIndexChanged, this, [=](int index)
    {
        QString selectedFrequencyBand = frequencyBandSelector->itemText(index);
        emit frequencyBandSelected(selectedFrequencyBand);
    });
    freqBandLayout->addWidget(freqBandLabel);
    freqBandLayout->addWidget(frequencyBandSelector);

    QVBoxLayout* btnLayout = new QVBoxLayout();
    QPushButton* instructionBtn = new QPushButton("INSTRUCTIONS", this);
    instructionBtn->setStyleSheet(
        "QPushButton {"
        "  width: 120px; height: 44px;"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #d3d3d3, stop:1 #696969);"
        "  border: 1px solid #DDDDDD;"
        "  border-radius: 2px;"
        "  color: #333333;"
        "  padding: 8px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "  box-shadow: 0px 8px 16px rgba(0, 0, 0, 0.3);"
        "}"
        "QPushButton:pressed {"
        "  padding-bottom: 4px;"
        "  transform: translateY(2px);"
        "}"
        );
    connect(instructionBtn, &QPushButton::clicked, this, [=]()
    {
        emit instructionButtonClicked();
    });
    QPushButton* gameMenuButton = new QPushButton("GAME MENU", this);

    gameMenuButton->setStyleSheet(
        "QPushButton {"
        "  width: 120px; height: 44px;"
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #d3d3d3, stop:1 #696969);"
        "  border: 1px solid #DDDDDD;"
        "  border-radius: 2px;"
        "  color: #333333;"
        "  padding: 8px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:pressed {"
        "  padding-bottom: 4px;"
        "  transform: translateY(2px);"
        "}"
    );
    connect(gameMenuButton, &QPushButton::clicked, this, [=](){
        emit exitToMenu();
    });

    btnLayout->addWidget(instructionBtn);
    btnLayout->addWidget(gameMenuButton);

    layout->addLayout(antennaTypeLayout);
    layout->addLayout(freqBandLayout);
    layout->addLayout(powerLayout);
    layout->addLayout(heightLayout);
    layout->addLayout(orientationLayout);
    layout->addWidget(transmitButton);
    layout->addSpacerItem(new QSpacerItem(50, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addLayout(btnLayout);
    panelContainer->setLayout(layout);

    // Add the panel as a dock widget
    addDockWidget(Qt::BottomDockWidgetArea, controlPanel);

    connect(instructionBtn , &QPushButton::clicked , this , &MainWindow::displayInstructionWindow);
}

void MainWindow::displayGameMenu()
{
    stackedWidget->setCurrentWidget(gameMenuWindow);

    if (controlPanel) {
        controlPanel->hide();
    }

    // Connect the level selection from GameMenuPage to startLevelClicked
    connect(gameMenuWindow, &GameMenuPage::levelSelected, this, [this](int levelNumber) {
        if (!controlPanel) {
            createControlPanel();
        }

        // Show the control panel when a level starts
        controlPanel->show();
        emit startLevelClicked(levelNumber);
    });

    connect(gameMenuWindow, &GameMenuPage::startButtonClicked, this, [this]() {
        int levelNumber = 1; // Default level
        if (!controlPanel) {
            createControlPanel();
        }

        // Show the control panel when the game starts
        controlPanel->show();
        emit startLevelClicked(levelNumber);
    });
}

void MainWindow::setupLevel(int levelNumber)
{
    level = new Environment(this);
    displayInstructionWindow(levelNumber);
}

void MainWindow::displayInstructionWindow(int levelNumber)
{
    levelInstructionWindow->changeInstructions(levelNumber);
    levelInstructionWindow->setParent(nullptr);
    levelInstructionWindow->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    levelInstructionWindow->resize(900,600);
    levelInstructionWindow->setWindowModality(Qt::ApplicationModal);
    levelInstructionWindow->setAttribute(Qt::WA_DeleteOnClose, false);

    QRect mainWindowGeometry = this->geometry();
    int x = mainWindowGeometry.left() + (mainWindowGeometry.width() - levelInstructionWindow->width()) / 2;
    int y = (mainWindowGeometry.top() + (mainWindowGeometry.height() - levelInstructionWindow->height()) / 2) - 60;
    levelInstructionWindow->move(x, y);

    levelInstructionWindow->show();
}

void MainWindow::dismissInstructions()
{
    //stackedWidget->setCurrentWidget(levelWorldWindow); // shouldn't show game menu, should show the level world
    levelInstructionWindow->hide();
    qDebug() << "dismissing instruction window" << Qt::endl;
}

void MainWindow::displayLevel(QVector<b2Vec2> position)
{
    level = new Environment(this);
    level -> drawParticles(position);
}

void MainWindow::displayLevelObjects(QVector<ObjectData> objects){
    level->drawQueue.clear();
    stackedWidget->addWidget(level);
    stackedWidget->setCurrentWidget(level);
    for(ObjectData &obj : objects) {
         switch (obj.type) {
         case ObjectType::Rock:{
             QPixmap rockImg = QPixmap(":/img/rock.png");

             rockImg = rockImg.scaled(rockImg.width() * 0.1f,
                                      rockImg.height()* 0.1f,
                                      Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);
             level -> drawObjects(obj.objPos, rockImg);
             break;
         }
         case ObjectType::Tree:{
             QPixmap treeImg = QPixmap(":/img/tree.png");
             treeImg = treeImg.scaled(treeImg.width() * 0.15f,
                                      treeImg.height()* 0.15f,
                                      Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);
                level -> drawObjects(obj.objPos, treeImg);
             break;
         }
         case ObjectType::Hill:{
             QPixmap hillImg = QPixmap(":/img/hills.png");
             hillImg = hillImg.scaled(hillImg.width() * 0.4f,
                                      hillImg.height()* 0.4f,
                                      Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);
             level -> drawObjects(obj.objPos, hillImg);
             break;
         }
         case ObjectType::Human:{
             QPixmap humanImg = QPixmap(":/img/human.png");
             humanImg = humanImg.scaled(humanImg.width() * 0.15f,
                                      humanImg.height()* 0.15f,
                                      Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);
             level -> drawObjects(obj.objPos, humanImg);
             break;
         }
        }
     }

}

void MainWindow::displayLevelCompleteWindow(int currentLevel)
{
    // Update score for the completed levellevel
    updateScore(10 * currentLevel);

    levelCompleteWindow->updateScoreDisplay(score);
    stackedWidget->setCurrentWidget(levelCompleteWindow);

    // Disconnect previous connections to avoid duplicate signals
    disconnect(levelCompleteWindow, &LevelCompletePage::gameMenuButtonClicked, nullptr, nullptr);
    disconnect(levelCompleteWindow, &LevelCompletePage::nextLevelButtonClicked, nullptr, nullptr);

    // Back to game menu
    connect(levelCompleteWindow, &LevelCompletePage::gameMenuButtonClicked, this, [&]() {
        resetScore();
        stackedWidget->setCurrentWidget(gameMenuWindow);
    });

    // Proceed to the next level
    connect(levelCompleteWindow, &LevelCompletePage::nextLevelButtonClicked, this, [=]() {
        int nextLevel = currentLevel + 1;
        if (nextLevel <= 5) {
            emit startLevelClicked(nextLevel); // Emit startLevelClicked signal for the next level
        } else {
            resetScore();
            // End of the game or loop back to the menu
            stackedWidget->setCurrentWidget(gameMenuWindow);
        }
    });
}

void MainWindow::updateScore(int points)
{
    score += points;
    qDebug() << "Score updated to:" << score;
}

void MainWindow::resetScore()
{
    score = 0;
    qDebug() << "Score reset.";
}




