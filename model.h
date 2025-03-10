/**
 * @file Model.h
 * @brief This class is the model component of the MVC architecture for the app.
 * It manages the game elements, simulating their dynamics and interactions according to
 * user inputs and game rules. This includes creating elements like avatars, ground,
 * sky, obstacles, and a particle mesh for wave propagation visualization.
 *
 * @author: Trentton Stratton, Phuc Hoang, Chanphone Visathip, Thu Ha.
 * @date: 12/12/2024
 */

#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include "math.h"
#include "Box2D/Box2D.h"

// Enumeration for different types of game objects.
enum ObjectType
{
    Rock,
    Tree,
    Hill,
    Human
};

// Structure to store object data including position and type.
struct ObjectData
{
    b2Vec2 objPos;
    ObjectType type;
    ObjectData(b2Vec2 position, ObjectType objType) : objPos(position), type(objType) {}
};

// The Model class manages the game's physical simulation and state.
class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(int levelNumber);
    ~Model();

    // Simulation properties
    float deltaTime;
    int windowWidth;
    int windowHeight;

    // Radio transmission settings
    b2Vec2 transmitLocation;
    b2Vec2 transmitDirection;
    b2Vec2 playerLocation;
    int antennaHeight;
    int transmitPower;
    float beamWidth = 360.0f;  // Default to omnidirectional
    float waveSpeed = 1.0f;    // Default wave speed
    QString frequencyBand;

    // Methods for particle mesh and game object management
    void addParticleMesh(int particleSpacing, int particleSize, int windowWidth, int windowHeight);
    void createParticleMeshJoint(b2Body* bodyA, b2Body* bodyB, int particleSpacing);
    b2Body* addParticle(b2Vec2 position, int particleSize);
    b2Body* addGround(b2Vec2 position);
    b2Body* addSky(b2Vec2 position);
    b2Body* addRock(b2Vec2 position);
    b2Body* addTree(b2Vec2 position);
    b2Body* addHill(b2Vec2 position);
    b2Body* addHuman(b2Vec2 position);
    void getPosition();
    void step();
    void deleteAddedObjects();
    QVector<int> calculateClosestParticles();
    float calculateScalingFactor();
    void emitWave();
    void getObjectPosition();

    // Radio settings adjustments
    void setAntennaHeight(int height);
    void setAntennaLocation(int antennaHeight);
    void setTransmitPower(int powerLevel);
    void setFrequencyBand(QString frequency);
    void setAntennaType(QString antenna);
    void setAntennaOrientation(int angleDegrees);
    bool isHumanTouch(const std::vector<b2Body*>& queriedBodies);

    // Nested class for managing level-specific worlds.
    class LevelWorlds
    {
    public:
        LevelWorlds(const QVector<ObjectData> &obj);
        QString getBackgroundPath() const;
        QVector<ObjectData> getObjects() const;

        // Static methods to create specific level configurations.
        static LevelWorlds createLevel1();
        static LevelWorlds createLevel2();
        static LevelWorlds createLevel3();
        static LevelWorlds createLevel4();
        static LevelWorlds createLevel5();

    private:
        QString backgroundPath;
        QVector<ObjectData> obj;
    };

    // Callback class for querying nearby particle bodies.
    class ParticleQueryCallback : public b2QueryCallback
    {
    public:
        std::vector<b2Body*> nearbyBodies;

        bool ReportFixture(b2Fixture* fixture) override
        {
            b2Body* body = fixture->GetBody();
            nearbyBodies.push_back(body);
            return true;  // Keep going to the next fixture.
        }
    };

    // Setup method for initializing levels.
    void setUpLevel(Model::LevelWorlds level);

private:
    b2World* world;  // The Box2D world for the simulation
    QVector<b2Body*> levelObjects;  // Bodies representing game objects
    QVector<b2Body*> particleMesh;  // Bodies representing the particle mesh
    QVector<ObjectData> levelItems;  // Data about the objects in the current level
    int m_levelNumber;  // The current level number

signals:
    void stateChanged();  // Signal to indicate that the model state has changed
    void updateParticlePositions(QVector<b2Vec2> positions);
    void updateObjectsPositions(QVector<ObjectData> positions);
    void humanTouched(int);

public slots:
    void onSetupNextLevel(int levelNumber);  // Slot to handle setting up the next level

};

#endif // MODEL_H
