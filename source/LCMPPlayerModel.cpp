//
//  LCMPPlayerModel.cpp
//  Low Control Mall Patrol
//
//  Author: Kevin Games
//  Version: 2/20/22
//

#include "LCMPPlayerModel.h"

/** The density of this player */
#define DEFAULT_DENSITY 1.0f
/** The friction of this player */
#define DEFAULT_FRICTION 0.4f
/** The restitution of this player */
#define DEFAULT_RESTITUTION 0.4f

using namespace cugl;

/**
 * initializes a Player Model
 */
bool PlayerModel::init(const Vec2 pos, const Size size, float scale,
                       const std::shared_ptr<cugl::scene2::SceneNode>& node,
                       std::shared_ptr<cugl::scene2::ActionManager>& actions) {
    // Call the parent's initializer
    physics2::CapsuleObstacle::init(pos, size);
    
    // Set physics properties for the body
    setName("player");
    setBodyType(b2_dynamicBody);
    setDensity(DEFAULT_DENSITY);
    setFriction(DEFAULT_FRICTION);
    setRestitution(DEFAULT_RESTITUTION);
    setFixedRotation(true);
    setDebugColor(Color4::RED);
    
    // Set collision sound
    _collisionSound  = "";
    
    // Save the scale (SCREEN UNITS / WORLD UNITS)
    _scale = scale;
    
    // Save player's top-level node
    _node = node;
    
    // Save action manager
    _actions = actions;
    
    // Add a dropshadow node
    PolyFactory pf;
    Poly2 shadow = pf.makeCapsule(pos * scale, size * scale);
    _dropshadow = scene2::PolygonNode::allocWithPoly(shadow);
    _dropshadow->setAnchor(Vec2::ANCHOR_CENTER);
    _dropshadow->setPosition(Vec2::ZERO);
    _dropshadow->setColor(Color4(Vec4(0,0,0,0.25f)));
    _node->addChild(_dropshadow);
    
    // Create animations
    for (int i = 0; i < _animFrames.size(); i++) {
        std::vector<int> vec;
        for(int ii = 0; ii < _animFrames[i]; ii++) {
            vec.push_back(ii);
        }
        _animations.push_back(scene2::Animate::alloc(vec,DURATION));
        _cycles.push_back(false);
    }
    
    // We're gonna assume this always works appropriately
    return true;
}

/**
 * Disposes of all resources in this instance of Player Model
 */
void PlayerModel::dispose() {
    _node = nullptr;
    _dropshadow = nullptr;
    
    for (int i = 0; i < _spriteNodes.size(); i++) {
        _spriteNodes[i] = nullptr;
        _animations[i] = nullptr;
    }
}

/**
 * Applies an acceleration to the player (most likely for local updates)
 */
void PlayerModel::applyForce(cugl::Vec2 force) {
    // Push the player in the direction they want to go
    b2Vec2 b2force(force.x * getAcceleration(), force.y * getAcceleration());
    _body->ApplyForceToCenter(b2force, true);
    
    // Dampen the movement
    b2Vec2 b2velocity = _body->GetLinearVelocity();
    b2Vec2 b2damping(b2velocity.x * -getDamping(), b2velocity.y * -getDamping());
    _body->ApplyForceToCenter(b2damping, true);
    
    // If the player has reached max speed
    //b2Vec2 b2velocity = _body->GetLinearVelocity();
    if (b2velocity.LengthSquared() >= getMaxSpeed() * getMaxSpeed()) {
        b2velocity.Normalize();
        b2velocity *= getMaxSpeed();
        _body->SetLinearVelocity(b2velocity);
    }
}

/**
 * Updates the position and velocity of the player (most likely for network updates)
 */
void PlayerModel::applyNetwork(cugl::Vec2 position, cugl::Vec2 velocity, cugl::Vec2 force) {
    setPosition(position);
    playAnimation(velocity);
}

/**
 * Updates the player node based on this player's body
 */
void PlayerModel::update(float timestep) {
    cugl::physics2::SimpleObstacle::update(timestep);
    
    if (_node != nullptr) {
        Vec2 position(_body->GetPosition().x, _body->GetPosition().y);
        _node->setPosition(position * _scale);
    }
}

int PlayerModel::findDirection(Vec2 movement) {
    float angle = movement.getAngle(Vec2(1,0)) * 180 / 3.14;
    int key = (angle < 45 || angle >= -45) ? 0 : -1;
    if (angle < 135 && angle >= 45) key = 1;
    else if (angle >= 135 || angle < -135 || (movement.y == 0 && movement.x < 0)) key = 2;
    else if (angle < -45 && angle >= -135) key = 3;
    return key;
}

/**
 * Performs a film strip action
 */
void PlayerModel::playAnimation(Vec2 movement) {
    // Figure out which animation direction to use
    int key = findDirection(movement);
    if (movement.length() == 0) {
        key = STILL_ANIM_KEY;
    }
    
    for (shared_ptr<cugl::scene2::SpriteNode> s : _spriteNodes) {
        s->setVisible(false);
    }

    // Animate different direction
    bool cycle;
    std::shared_ptr<scene2::SpriteNode> node;
    // Set still animation if no movement
    _actions->activate(ACT_KEY, _animations[key], _spriteNodes[key]);
    _spriteNodes[key]->setVisible(true);
    node = _spriteNodes[key];
    cycle = _cycles[key];

    if (movement.length() > 0) {
        // Turn on the flames and go back and forth
        if (node->getFrame() == node->getSize()-1) {
            _cycles[key] = false;
        } else {
            _cycles[key] = true;
        }

        // Increment
        if (_cycles[key]) {
            node->setFrame(node->getFrame()+1);
        } else {
            node->setFrame(node->getFrame()-1);
        }
    } else {
        node->setFrame(0);
    }
}

void PlayerModel::setSpriteNodes(float width) {
    for (int i = 0; i < _spriteSheets.size(); i++) {
        _spriteNodes.push_back(scene2::SpriteNode::alloc(_spriteSheets[i], 1, _animFrames[i]));
        _spriteNodes[i]->setScale(CHAR_SCALE);
        _spriteNodes[i]->setAnchor(Vec2::ANCHOR_CENTER);
        _spriteNodes[i]->setPosition(Vec2(0, width / 2.5f));
        _spriteNodes[i]->setVisible(false);
        _node->addChild(_spriteNodes[i]);
    }
    _spriteNodes[STILL_ANIM_KEY]->setVisible(true);
}
