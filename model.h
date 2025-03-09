/**
 * This class is the model for the app.
 *
 * It is responsible for creating the elements of the
 * game and determining their position at each step in
 * time. Additionally, it responds to user controls to
 * modify the properties of some objects in order to
 * alter how the radio wave propagates through the medium.
 *
 * Game elements include the player avatar, target avatar,
 * ground, sky, obstacles, and a mesh of particles (which is
 * the medium by which the wave propagation is visualized).
 */

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include "math.h"

#include "Box2D/Box2D.h"

enum ObjectType
{
    Rock,
    Tree,
    Hill,
    Human
};

struct ObjectData
{
    b2Vec2 objPos;
    ObjectType type;

    ObjectData(b2Vec2 position, ObjectType objType)
        : objPos(position), type(objType) {}
};


class Model : public QObject
{
    Q_OBJECT

public:
    Model(int levelNumber);
    ~Model();

    float deltaTime;
    int windowWidth;
    int windowHeight;

    // radio settings
    b2Vec2 transmitLocation;
    b2Vec2 transmitDirection;
    b2Vec2 playerLocation;
    int antennaHeight;
    int transmitPower;
    float beamWidth = 360.0f; // Default to omnidirectional
    float waveSpeed = 1.0f;   // Default wave speed
    QString frequencyBand;


    /**
    * @brief Model::addParticleMesh
    * Creates a soft body medium through which a wave can propagate. This
    * is composed of a mesh of particle bodies where each particle is
    * connected to adjacent particles by a distance joint.
    *
    * @param particleSpacing
    * @param particleSize
    * @param windowWidth
    * @param windowHeight
    */
    void addParticleMesh(int particleSpacing, int particleSize, int windowWidth, int windowHeight);

    /**
    * @brief Joins two particle bodies with a distance joint.
    *
    * @param bodyA
    * @param bodyB
    * @param particleSpacing
    */
    void createParticleMeshJoint(b2Body* bodyA, b2Body* bodyB, int particleSpacing);

    /**
    * @brief Creates a particle.
    *
    * @param position
    * @param particleSize
    * @return
    */
    b2Body* addParticle(b2Vec2 position, int particleSize);


    /**
    * @brief Creates the ground.
    * @param position
    * @return
    */
    b2Body* addGround(b2Vec2 position);

    /**
    * @brief Creates the sky.
    * @param position
    * @return
    */
    b2Body* addSky(b2Vec2 position);

    // level specific obstacles
    /**
    * @brief Creates the rock obstacle.
    * @param position
    * @return
    */
    b2Body* addRock(b2Vec2 position);

    /**
    * @brief Creates the tree obstacle.
    * @param position
    * @return
    */
    b2Body* addTree(b2Vec2 position);

    /**
    * @brief Creates the hill obstacle.
    * @param position
    * @return
    */
    b2Body* addHill(b2Vec2 position);

    /**
    * @brief Creates a human.
    * @param position
    * @return
    */
    b2Body* addHuman(b2Vec2 position);

    /**
    * @brief Creates a vector of particle positions and emits a
    * signal, sending the vector so that the display can be
    * updated to reflect the changed positions.
    *
    */
    void getPosition();

    /**
    * @brief Advances the model forward one step in time.
    */
    void step();

    /**
    * @brief Model::deleteAddedObjects
    */
    void deleteAddedObjects();

    QVector<int> calculateClosestParticles();
    float calculateScalingFactor();


    /**
    * @brief Model::emitWave
    */
    void emitWave(); // need to add direction(s), strength, etc

    void getObjectPosition();

    // radio setting adjustment

    /**
    * @brief Setter for antenna height.
    *
    * @param height
    */
    void setAntennaHeight(int height);

    void setAntennaLocation(int antennaHeight);

    /**
    * @brief Setter for transmit power.
    *
    * @param powerLevel
    */
    void setTransmitPower(int powerLevel);

    /**
    * @brief Setter for transmit frequency.
    *
    * @param frequency
    */
    void setFrequencyBand(QString frequency);

    /**
    * @brief Setter for antenna type.
    *
    * @param antenna
    */
    void setAntennaType(QString antenna);

    /**
    * @brief Setter for the direction the antenna is pointed.
    * It converts the angular direction (from -90 to 90 degrees, where
    * 0 is straight up) to a cartesian vector.
    *
    * @param antenna
    */
    void setAntennaOrientation(int angleDegrees);

    /**
     * @brief Check if human is touched by radio wave
     */
    bool isHumanTouch(const std::vector<b2Body*> &queriedBodies);


    class LevelWorlds
    {

    public:
        LevelWorlds(const QVector<ObjectData> &obj);
        QString getBackgroundPath() const;
        QVector<ObjectData> getObjects() const;

        /**
         * @brief Create level objects
         * @return LevelWorlds
         */
        static LevelWorlds createLevel1();

        /**
         * @brief Create level objects
         * @return LevelWorlds
         */
        static LevelWorlds createLevel2();

        /**
         * @brief Create level objects
         * @return LevelWorlds
         */
        static LevelWorlds createLevel3();

        /**
         * @brief Create level objects
         * @return LevelWorlds
         */
        static LevelWorlds createLevel4();

        /**
         * @brief Create level objects
         * @return LevelWorlds
         */
        static LevelWorlds createLevel5();

    private:
        QString backgroundPath;
        QVector<ObjectData> obj;
    };

    class ParticleQueryCallback : public b2QueryCallback
    {
    public:
        std::vector<b2Body*> nearbyBodies;

        bool ReportFixture(b2Fixture* fixture) override
        {
            b2Body* body = fixture->GetBody();
            if (body->GetType() == b2_dynamicBody)
            {
                nearbyBodies.push_back(body);
            }else if (body->GetType() == b2_staticBody)
            {
                nearbyBodies.push_back(body);
            }
            return true;
        }
    };


    void setUpLevel(Model::LevelWorlds level);

private:
    b2World* world;
    QVector<b2Body*> levelObjects;
    QVector<b2Body*> particleMesh;
    QVector<ObjectData> levelItems;
    int m_levelNumber;

signals:
    void stateChanged(); // model changed, gui needs to update
    void updateParticlePositions(QVector<b2Vec2> positions);
    void updateObjectsPositions(QVector<ObjectData> positions);
    void humanTouched(int);

public slots:
    void onSetupNextLevel(int levelNumber); // Slot to set up the next level

};

#endif // MODEL_H
