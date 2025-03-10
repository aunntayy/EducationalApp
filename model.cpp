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

#include "model.h"

void Model::setUpLevel(Model::LevelWorlds level)
{
    for (const ObjectData& obj : level.getObjects())
    {
        switch (obj.type)
        {
        case ObjectType::Rock:
            addRock(obj.objPos);
            break;
        case ObjectType::Tree:
            addTree(obj.objPos);
            break;
        case ObjectType::Hill:
            addHill(obj.objPos);
            break;
        case ObjectType::Human:
            addHuman(obj.objPos);
            if (playerLocation.x == 0.0f) {  // Set playerLocation.x to the first human's position
                playerLocation.x = obj.objPos.x;
                playerLocation.y = obj.objPos.y;
            }
            break;
        default:
            break;
        }
    }
}

Model::Model(int levelNumber)
{
    windowWidth = 1200;
    windowHeight = 800;

    b2Vec2 gravity(0.0, 0.0);
    world = new b2World(gravity);
    deltaTime = 1.0f / 60.0f; // 60 FPS

    addGround(b2Vec2(0.0f, 850.0f));
    addParticleMesh(20, 1, windowWidth, windowHeight); // 10, 1

    // switch statement for level objects to add
    qDebug() <<"in model: " <<levelNumber;
    m_levelNumber = levelNumber;
    switch(levelNumber)
    {
        case 1:
        {
            Model::LevelWorlds level = LevelWorlds::createLevel1();
            setUpLevel(level);
            break;
        }
        case 2:
        {
            Model::LevelWorlds level = LevelWorlds::createLevel2();
            setUpLevel(level);
            break;
        }
        case 3:
        {
            Model::LevelWorlds level = LevelWorlds::createLevel3();
            setUpLevel(level);
            break;
        }
        case 4:
        {
            Model::LevelWorlds level = LevelWorlds::createLevel4();
            setUpLevel(level);
            break;
        }
        case 5:
        {
            Model::LevelWorlds level = LevelWorlds::createLevel5();
            setUpLevel(level);
            break;
        }
    }
    playerLocation = b2Vec2(400.0f, 500.0f); // kind of arbitrary player position
    transmitLocation = b2Vec2(playerLocation.x, playerLocation.y - 5 * 10);
}


Model::~Model()
{
    delete world;
}

void Model::getPosition()
{
    QVector<b2Vec2> position;
    for (auto body : particleMesh)
    {
        position.append(body->GetPosition());
    }
    emit updateParticlePositions(position);
}

void Model::getObjectPosition()
{
    emit updateObjectsPositions(levelItems);
}


void Model::addParticleMesh(int particleSpacing, int particleSize, int windowWidth, int windowHeight)
{

    for (int y = 0; y <= windowHeight; y += particleSpacing)
    {
        for (int x = -200; x <= windowWidth + 200; x += particleSpacing)
        {
            b2Vec2 position(x,y);
            b2Body* particle = addParticle(position, particleSize);

            // create distance joints to particle to left (in -x direction)
            if (x > -200)
            {
                b2Body* particleLeft = particleMesh.back();
                createParticleMeshJoint(particleLeft, particle, particleSpacing);
            }

            // create distance joints to particle in row above (in -y direction)
            if (y > 0)
            {
                b2Body* particleAbove = particleMesh[particleMesh.size() - ((windowWidth+400) / particleSpacing) - 1];
                createParticleMeshJoint(particleAbove, particle, particleSpacing);
            }

            // create distance joints to particle above and to left (in -y direction and -x direction)
            if (x > 1 && y > 1)
            {
                b2Body* particleDiagonal = particleMesh[particleMesh.size() - ((windowWidth+400) / particleSpacing) - 2];

                int diagParticleSpacing = sqrt(2 * (particleSpacing * particleSpacing)); // pythag thrm
                createParticleMeshJoint(particleDiagonal, particle, diagParticleSpacing);
            }

            particleMesh.append(particle);
        }
    }
    qDebug() << "Width:" << windowWidth << "height" << windowHeight;
    qDebug() << particleMesh.size();
    getPosition();
}


b2Body* Model::addParticle(b2Vec2 position, int particleSize)
{

    // define body
    b2BodyDef particleDef;
    particleDef.position.Set(position.x, position.y);

    if ((position.y > 0 && position.y < windowHeight) && (position.x > -200 && position.x < windowWidth + 200))
    {
        particleDef.type = b2_dynamicBody;
    }
    else
    {
        particleDef.type = b2_staticBody;
    }

    // create body
    b2Body* particleBody = world->CreateBody(&particleDef);

    // shape
    b2CircleShape particleShape;
    particleShape.m_radius = particleSize / 2.0f;


    // fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &particleShape;

    if (particleDef.type == b2_dynamicBody)
    {
        // dynamic fixture properties
        fixtureDef.density = 1.0f;     // weight
        fixtureDef.friction =0.01f;    //.1
        fixtureDef.restitution = 10000.0f; //.5
    }

    particleBody->CreateFixture(&fixtureDef);
    return particleBody;
}


void Model::createParticleMeshJoint(b2Body* bodyA, b2Body* bodyB, int particleSpacing)
{
    b2DistanceJointDef jointDef;
    jointDef.bodyA = bodyA;
    jointDef.bodyB = bodyB;
    jointDef.collideConnected = false; // prevent collision between connected bodies
    jointDef.length = particleSpacing;
    jointDef.frequencyHz = 2.5f;       // stiffness 10
    jointDef.dampingRatio = .1f;       // damping   5

    world->CreateJoint(&jointDef);
}


b2Body* Model::addSky(b2Vec2 position)
{
    b2BodyDef skyDef;
    skyDef.type = b2_staticBody;
    skyDef.position.Set(position.x, position.y);

    b2Body* skyBody = world->CreateBody(&skyDef);

    b2PolygonShape skyBox;
    skyBox.SetAsBox(1200.0f, 50.0f);

    skyBody->CreateFixture(&skyBox, 0.0f);

    return skyBody;
}


b2Body* Model::addGround(b2Vec2 position)
{
    b2BodyDef groundDef;
    groundDef.type = b2_staticBody;
    groundDef.position.Set(position.x, position.y);

    b2Body* groundBody = world->CreateBody(&groundDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(1200.0f, 50.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    return groundBody;
}

b2Body* Model::addRock(b2Vec2 position)
{
    b2BodyDef rockBody;
    rockBody.type = b2_staticBody;
    rockBody.position.Set(position.x, position.y);

    b2Body* rockBd = world->CreateBody(&rockBody);

    b2PolygonShape rockBox;
    rockBox.SetAsBox(90.0f, 50.0f);

    b2FixtureDef fixture;
    fixture.shape = &rockBox;
    fixture.density = 0.0f;
    fixture.friction = 0.8f;
    fixture.restitution = 0.1f;
    rockBd->CreateFixture(&fixture);

    b2Vec2 rockPos = rockBd->GetPosition();
    levelItems.push_back(ObjectData(rockPos,ObjectType::Rock));

    levelObjects.push_back(rockBd);

    return rockBd;
}

b2Body* Model::addTree(b2Vec2 position)
{
    b2BodyDef treeBody;
    treeBody.type = b2_staticBody;
    treeBody.position.Set(position.x, position.y);

    b2Body* treeBd = world->CreateBody(&treeBody);

    b2PolygonShape treeBox;
    treeBox.SetAsBox(90.0f, 70.0f);

    b2FixtureDef fixture;
    fixture.shape = &treeBox;
    fixture.density = 0.0f;
    fixture.friction = 0.8f;
    fixture.restitution = 0.1f;
    treeBd->CreateFixture(&fixture);

    b2Vec2 treePos = treeBd->GetPosition();
    levelItems.push_back(ObjectData(treePos,ObjectType::Tree));

    levelObjects.push_back(treeBd);

    return treeBd;
}

b2Body* Model::addHill(b2Vec2 position)
{
    b2BodyDef hillBody;
    hillBody.type = b2_staticBody;
    hillBody.position.Set(position.x, position.y);

    b2Body* hillBd = world->CreateBody(&hillBody);

    b2PolygonShape hillBox;
    hillBox.SetAsBox(0.0f, 0.0f);

    b2FixtureDef fixture;
    fixture.shape = &hillBox;
    fixture.density = 0.0f;
    fixture.friction = 0.8f;
    fixture.restitution = 0.1f;
    hillBd->CreateFixture(&fixture);

    b2Vec2 hillPos = hillBd->GetPosition();
    levelItems.push_back(ObjectData(hillPos,ObjectType::Hill));

    levelObjects.push_back(hillBd);
    return hillBd;
}

b2Body* Model::addHuman(b2Vec2 position)
{
    b2BodyDef humanBody;
    humanBody.type = b2_staticBody;
    humanBody.position.Set(position.x, position.y);

    b2Body* humanBd = world->CreateBody(&humanBody);

    b2PolygonShape humanBox;
    humanBox.SetAsBox(50.0f, 40.0f);

    b2FixtureDef fixture;
    fixture.shape = &humanBox;
    fixture.density = 0.0f;
    fixture.friction = 0.8f;
    fixture.restitution = 0.1f;
    humanBd->CreateFixture(&fixture);

    b2Vec2 humanPos = humanBd->GetPosition();
    levelItems.push_back(ObjectData(humanPos,ObjectType::Human));

    levelObjects.push_back(humanBd);

    return humanBd;
}

void Model::step()
{
    world->Step(deltaTime, 6, 2);
    getPosition();
    getObjectPosition();
}

void Model::deleteAddedObjects()
{
    for (b2Body* body : levelObjects)
    {
        world->DestroyBody(body);
    }
    levelItems.clear();
}

void Model::setAntennaHeight(int height)
{
    antennaHeight = height;
    setAntennaLocation(antennaHeight);
}

void Model::setAntennaLocation(int antennaHeight)
{
    transmitLocation = b2Vec2(playerLocation.x, playerLocation.y - antennaHeight * 10);
}

void Model::setTransmitPower(int powerLevel)
{
    transmitPower = powerLevel;
}

void Model::setFrequencyBand(QString frequency)
{
    if (frequency == "HF") {
        waveSpeed = 1.0;  // Low frequency, slower wave propagation
    } else if (frequency == "VHF") {
        waveSpeed = 2.0;  // Moderate speed
    } else if (frequency == "UHF") {
        waveSpeed = 3.0;  // High speed
    } else if (frequency == "SHF") {
        waveSpeed = 4.0;  // Very high speed
    }

    qDebug() << "Frequency band changed. Wave speed is: " << waveSpeed;
}

void Model::setAntennaType(QString antenna)
{
    if (antenna == "dish") {
        beamWidth = 10;   // Narrow beam
        waveSpeed = 30.0; // High speed for long-range
    } else if (antenna == "yagi") {
        beamWidth = 45;   // Moderate beam
        waveSpeed = 20.0; // Medium speed for mid-range
    } else if (antenna == "dipole") {
        beamWidth = 360;  // Omnidirectional
        waveSpeed = 10.0; // Low speed for short-range
    }

    qDebug() << "Antenna type changed to:" << antenna << ". Beam width:" << beamWidth << ", Wave speed:" << waveSpeed;
}

void Model::setAntennaOrientation(int angleDegrees)
{
    float angleRadians = angleDegrees * (M_PI / 180.0f);

    float x = std::sin(angleRadians);
    float y = std::cos(angleRadians);

    transmitDirection = b2Vec2(x, y);
}

void Model::emitWave()
{
    // Adjust radius based on power level
    int baseRadius = 50; // Base range of the wave
    int radius = baseRadius + (transmitPower * 2); // Increase range with power

    b2AABB aabb;
    aabb.lowerBound = transmitLocation - b2Vec2(radius, radius);
    aabb.upperBound = transmitLocation + b2Vec2(radius, radius);

    Model::ParticleQueryCallback callback;
    this->world->QueryAABB(&callback, aabb);

    for (b2Body* particle : callback.nearbyBodies)
    {
        b2Vec2 particlePosition = particle->GetPosition();
        b2Vec2 direction = particlePosition - transmitLocation;

        if (direction.LengthSquared() <= radius * radius) {
            direction.Normalize();

            // Check if the particle is within the beam width
            float angleCos = b2Dot(transmitDirection, direction);
            float halfBeamWidthCos = std::cos(beamWidth / 2.0f);

            if (angleCos >= halfBeamWidthCos) {
                b2Vec2 velocity = direction;

                // Modify velocity based on power and scaling factor
                float scalingFactor = calculateScalingFactor();
                velocity *= waveSpeed * scalingFactor * (transmitPower / 100.0f);

                particle->SetLinearVelocity(velocity);

                qDebug() << "Particle velocity: " << velocity.x << ", " << velocity.y;
            }
        }

        if (isHumanTouch(callback.nearbyBodies)) {
            emit humanTouched(m_levelNumber);
        }
    }

    qDebug() << "Wave emitted with beam width:" << beamWidth << "and speed:" << waveSpeed << "and power:" << transmitPower;
}

bool Model::isHumanTouch(const std::vector<b2Body*> &queriedBodies)
{

    for (b2Body* body : queriedBodies)
    {
        if (std::find(levelObjects.begin(), levelObjects.end(), body) != levelObjects.end())
        {
            for (const ObjectData& obj : levelItems)
            {
                if (obj.type == ObjectType::Human &&
                    (obj.objPos - body->GetPosition()).LengthSquared() < 1e-6f)
                {
                    return true; // Human is within the aabb
                }
            }
        }
    }
    return false;
}

float Model::calculateScalingFactor() {
    int p_min = 1;
    int p_max = 100;

    float s_min = 100000.0f;
    float s_max = 1000000000.0f; // max scaling factor

    float n = 1.00001;
    float alpha = 1000.0f;

    if (transmitPower < p_min) transmitPower = p_min;
    if (transmitPower > p_max) transmitPower = p_max;

    // exponential mapping (power to scaling factor)
    //float scalingFactor = s_min + (s_max - s_min) * pow((transmitPower - p_min) / (float)(p_max - p_min), n);
    //float scalingFactor = (s_max - s_min) * pow((transmitPower - p_min) / (float)(p_max - p_min), n);

    // logarithmic mapping
    //float scalingFactor = s_min + (s_max - s_min) * (log(transmitPower + 1) / log(p_max + 1)) / alpha;
    //float scalingFactor = s_min + (s_max - s_min) * (log((transmitPower + alpha) / alpha) / log((p_max + alpha) / alpha));

    // linear mapping
    float scalingFactor = (s_min / 100) + (s_max - s_min) * ((transmitPower - p_min) / (float)(p_max - p_min));

    return scalingFactor;
}

void Model::onSetupNextLevel(int levelNumber)
{
    switch (levelNumber) {
    case 1:
        setUpLevel(LevelWorlds::createLevel1());
        break;
    case 2:
        setUpLevel(LevelWorlds::createLevel2());
        break;
    case 3:
        setUpLevel(LevelWorlds::createLevel3());
        break;
    case 4:
        setUpLevel(LevelWorlds::createLevel4());
        break;
    case 5:
        setUpLevel(LevelWorlds::createLevel5());
        break;
    default:
        qDebug() << "Invalid level number: " << levelNumber;
        return;
    }

    // Update the environment after setting up the level
    step(); // Start the simulation for the new level
}

Model::LevelWorlds::LevelWorlds(const QVector<ObjectData> &obj)
    : obj(obj) {}

QString Model::LevelWorlds::getBackgroundPath() const {
    return backgroundPath;
}

QVector<ObjectData> Model::LevelWorlds::getObjects() const {
    return obj;
}

// Level World
Model::LevelWorlds Model::LevelWorlds::createLevel1() {
    QVector<ObjectData> objects= {
         {{b2Vec2(150.0f, 700.0f)},  ObjectType::Human},
         {{b2Vec2(420.0f, 300.0f)},  ObjectType::Human}
    };
    return LevelWorlds(objects);
}

Model::LevelWorlds Model::LevelWorlds::createLevel2() {
    QVector<ObjectData> objects= {
        {{b2Vec2(150.0f, 700.0f)},  ObjectType::Human},
        {{b2Vec2(1000.0f, 700.0f)},  ObjectType::Human}
    };
    return LevelWorlds(objects);
}


Model::LevelWorlds Model::LevelWorlds::createLevel3() {
    QVector<ObjectData> objects = {
        {{b2Vec2(740.0f,300.0f)} , ObjectType::Hill},
        {{b2Vec2(150.0f, 700.0f)},  ObjectType::Human},
        {{b2Vec2(630.0f, 350.0f)},  ObjectType::Human}
        //{{b2Vec2(320.0f, 750.0f)},  ObjectType::Human}
    };
    return LevelWorlds(objects);
}

Model::LevelWorlds Model::LevelWorlds::createLevel4() {
    QVector<ObjectData> objects= {
        {{b2Vec2(800.0f,300.0f)} , ObjectType::Hill},
        {{b2Vec2(150.0f, 700.0f)},  ObjectType::Human},
        {{b2Vec2(1000.0f, 700.0f)},  ObjectType::Human}
    };
    return LevelWorlds(objects);
}

Model::LevelWorlds Model::LevelWorlds::createLevel5() {
    QVector<ObjectData> objects= {
        {{b2Vec2(150.0f, 700.0f)},  ObjectType::Human},
        {{b2Vec2(1000.0f, 700.0f)},  ObjectType::Human},
        //{{b2Vec2(320.0f, 700.0f)},  ObjectType::Human},
        {{b2Vec2(500.0f, 750.0f)}, ObjectType::Rock},
        {{b2Vec2(550.0f, 570.0f)}, ObjectType::Tree}
    };
    return LevelWorlds(objects);
}
