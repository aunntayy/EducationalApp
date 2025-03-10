/**
 * This class is a helper class for the view (GUI.cpp).
 * It is responsible for drawing the game objects based on their positions, which are determined by the model (model.cpp).
 * @author: Chanphone Visathip, Phuc Hoang
 * @date: 12/12/2024
 */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QWidget>
#include <QPainter>
#include <Box2D/Box2D.h>
#include <QTimer>
#include <QDebug>

// The Environment class inherits from QWidget and manages the game's visual representation.
class Environment : public QWidget
{
    Q_OBJECT

public:
    // Constructor: Initializes a new instance of the Environment class with an optional parent widget.
    explicit Environment(QWidget* parent = nullptr);

    // Nested class for particle management in the game environment.
    class Particle
    {
    public:
        // Constructor: Creates a particle with a specific size and initial position.
        Particle(int size, b2Vec2 pos)
            :particleSize(size), originalPos(pos) {}

        b2Body* body; // Box2D body associated with the particle
        int particleSize; // Size of the particle
        b2Vec2 originalPos; // Original position of the particle
    };

    /**
     * Draws the specified object at the given position.
     * @param object The position of the object to draw.
     * @param img The image (pixmap) used to represent the object.
     */
    void drawObjects(b2Vec2 object, QPixmap img);

    /**
     * Draws particles at specified locations.
     * @param particles Vector of positions where particles should be drawn.
     */
    void drawParticles(const QVector<b2Vec2> particles);

    // Vector containing all particles in the environment.
    QVector<Particle> particles;
    // Box2D body representing a rock in the environment.
    b2Body* rock;
    // Queue of pairs consisting of positions and images to be drawn in the current frame.
    QVector<std::pair<b2Vec2, QPixmap>> drawQueue;
    // Background image for level 1 of the game.
    QPixmap backGround = QPixmap(":/img/backgroundLv1.jpg");

protected:
    // Overridden paint event to handle custom drawing of the widget.
    void paintEvent(QPaintEvent* event) override;
};

#endif // ENVIRONMENT_H
