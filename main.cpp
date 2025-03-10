#include <QApplication>
#include <QTimer>
#include "GUI.h"
#include "model.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow gui;
    Model* model = nullptr;
    QTimer timer;

    // Display the game menu initially
    gui.displayGameMenu();

    // Start Level
    QObject::connect(&gui, &MainWindow::startLevelClicked, [&](int levelNumber) {
        // Clean up the previous model
        if (model) {
            delete model;
            model = nullptr;
        }

        // Create and set up a new model
        model = new Model(levelNumber);

        // Set up the GUI for the new level
        gui.setupLevel(levelNumber);

        // Start the timer for updating the model
        timer.start(16);

        // Connect timer to model's step function
        QObject::connect(&timer, &QTimer::timeout, [&]() {
            if (model) {
                model->step();
            }
        });

        // Connect model updates to GUI
        QObject::connect(model, &Model::updateParticlePositions, [&gui](const QVector<b2Vec2>& positions) {
            gui.displayLevel(positions);
        });

        QObject::connect(model, &Model::updateObjectsPositions, [&gui](const QVector<ObjectData>& objects) {
            gui.displayLevelObjects(objects);
        });

        // Connect GUI controls to the model
        QObject::connect(&gui, &MainWindow::antennaTypeSelected, [=](const QString& selectedAntenna) {
            model->setAntennaType(selectedAntenna);
        });

        QObject::connect(&gui, &MainWindow::antennaOrientationAdjusted, [=](int selectedAntennaOrientation) {
            model->setAntennaOrientation(selectedAntennaOrientation);
        });

        QObject::connect(&gui, &MainWindow::frequencyBandSelected, [=](const QString& selectedFrequencyBand) {
            model->setFrequencyBand(selectedFrequencyBand);
        });

        QObject::connect(&gui, &MainWindow::powerLevelAdjusted, [=](int selectedPowerLevel) {
            model->setTransmitPower(selectedPowerLevel);
        });

        QObject::connect(&gui, &MainWindow::antennaHeightAdjusted, [=](int selectedAntennaHeight) {
            model->setAntennaHeight(selectedAntennaHeight);
        });

        QObject::connect(&gui, &MainWindow::transmitButtonClicked, model, &Model::emitWave);

        // Handle human touched event
        QObject::connect(model, &Model::humanTouched, [&](int levelNumber) {
            gui.displayLevelCompleteWindow(levelNumber);
            timer.stop();
        });
    });

    // Handle level complete transition
    QObject::connect(&gui, &MainWindow::levelComplete, [&](int currentLevel) {
        // Move to the next level
        int nextLevel = currentLevel + 1;
        if (nextLevel <= 5) {
            gui.displayInstructionWindow(nextLevel);
            emit gui.startLevelClicked(nextLevel); // Trigger startLevelClicked for the next level
        } else {
            // Game completed, return to main menu
            gui.displayGameMenu();
        }
    });

    // Reset to the main menu on exit
    QObject::connect(&gui, &MainWindow::exitToMenu, [&]() {
        // Stop the timer
        timer.stop();

        // Delete the current model
        if (model) {
            delete model;
            model = nullptr;
        }

        // Reset the GUI to the main menu
        gui.displayGameMenu();
    });

    gui.show();
    return app.exec();
}
