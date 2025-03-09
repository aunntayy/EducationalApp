/**
 * This class is a helper class for the view (GUI.cpp).
 *
 * It draws the objects of the game according to the
 * positions determined by the model (model.cpp).
 */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QWidget>
#include <QPainter>
#include <Box2D/Box2D.h>
#include <QTimer>
#include <QDebug>

class Environment : public QWidget
{
    Q_OBJECT

public:

    explicit Environment(QWidget* parent = nullptr);

    class Particle
    {
    public:
        Particle(int size, b2Vec2 pos)
            :particleSize(size) , originalPos(pos) {}
        b2Body* body;
        int particleSize;
        b2Vec2 originalPos;
    };

    /**
     * @brief Recieve object location from GUI and draw it on screen
     * @param object
     * @param img
     */
    void drawObjects(b2Vec2 object, QPixmap img);
    //void resetDotPos();

    /**
     * @brief Recieve particles location and draw it on screen
     * @param particles
     */
    void drawParticles(const QVector<b2Vec2> particles);

    QVector<Particle> particles;
    b2Body* rock;
    QVector<std::pair<b2Vec2, QPixmap>> drawQueue;
    QPixmap backGround = QPixmap(":/img/backgroundLv1.jpg");

protected:
    void paintEvent(QPaintEvent* event) override;

};

#endif // ENVIRONMENT_H
