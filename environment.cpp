/**
 * This class is a helper class for the view (GUI.cpp).
 *
 * It draws the objects of the game according to the
 * positions determined by the model (model.cpp).
 */

#include "environment.h"

Environment::Environment(QWidget* parent) : QWidget(parent){}

void Environment::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.drawPixmap(rect(), backGround);

    painter.setPen(QPen(qRgb(0, 0, 0)));
    painter.setBrush(QBrush(QColor(66, 200, 245, 100)));

    for(auto particle : particles) {
        painter.drawEllipse(QPoint(particle.originalPos.x, particle.originalPos.y),
                            particle.particleSize,particle.particleSize);
    }

    // Draw objects
    for (const auto& item : drawQueue) {
        const b2Vec2& pos = item.first;
        const QPixmap& img = item.second;
        QRect targetRect(pos.x - img.width() / 2,
                         pos.y - img.height() / 2,
                         img.width(), img.height());
        painter.drawPixmap(targetRect, img);
    }
}

void Environment::drawParticles(QVector<b2Vec2> particlesPos) {
    int size = 3; //2
    for(auto particlePos : particlesPos) {
        particles.append(Particle(size, particlePos));
    }
    update();
}

void Environment::drawObjects(b2Vec2 objectsPos, QPixmap img)
{
    // Add object to the drawing queue
    drawQueue.append({objectsPos, img});
    update();
}






