// #define NUM_PARTICLES 1
#define NUM_RIGID_BODIES 1
#include <stdio.h>
#include <windows.h>
#include <stdlib.h> 
#include <math.h>
#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif
// Two dimensional vector.
typedef struct {
    float x;
    float y;
} Vector2;

typedef struct {
    float width;
    float height;
    float mass;
    float momentOfInertia;
} BoxShape;

// Calculates the inertia of a box shape and stores it in the momentOfInertia variable.
void CalculateBoxInertia(BoxShape *boxShape) {
    float m = boxShape->mass;
    float w = boxShape->width;
    float h = boxShape->height;
    boxShape->momentOfInertia = m * (w * w + h * h) / 12;
}

// Two dimensional rigid body
typedef struct {
    Vector2 position;
    Vector2 linearVelocity;
    float angle;
    float angularVelocity;
    Vector2 force;
    float torque;
    BoxShape shape;
} RigidBody;

typedef struct {
    Vector2 min;
    Vector2 max;
} AABB;
// Global array of rigid bodies.
RigidBody rigidBodies[NUM_RIGID_BODIES];

// Prints the position and angle of each body on the output.
// We could instead draw them on screen.
void PrintRigidBodies() {
    for (int i = 0; i < NUM_RIGID_BODIES; ++i) {
        RigidBody *rigidBody = &rigidBodies[i];
        printf("body[%i] p = (%.2f, %.2f), a = %.2f\n", i, rigidBody->position.x, rigidBody->position.y, rigidBody->angle);
    }
}

// Initializes rigid bodies with random positions and angles and zero linear and angular velocities.
// They're all initialized with a box shape of random dimensions.
void InitializeRigidBodies() {
    for (int i = 0; i < NUM_RIGID_BODIES; ++i) {
        RigidBody *rigidBody = &rigidBodies[i];
        rigidBody->position = (Vector2){rand() %50, rand() %50 };
        rigidBody->angle = (rand() %360)/360.f * M_PI * 2;
        rigidBody->linearVelocity = (Vector2){0, 0};
        rigidBody->angularVelocity = 0;
        
        BoxShape shape;
        shape.mass = 10;
        shape.width = 1 + rand() % 2;
        shape.height = 1 + rand() % 2;
        CalculateBoxInertia(&shape);
        rigidBody->shape = shape;
    }
}

// Applies a force at a point in the body, inducing some torque.
void ComputeForceAndTorque(RigidBody *rigidBody) {
    Vector2 f = (Vector2){0, 100};
    rigidBody->force = f;
    // r is the 'arm vector' that goes from the center of mass to the point of force application
    Vector2 r = (Vector2){rigidBody->shape.width / 2, rigidBody->shape.height / 2};
    rigidBody->torque = r.x * f.y - r.y * f.x;
}

BOOL TestAABBOverlap(AABB* a, AABB* b)
{
    float d1x = b->min.x - a->max.x;
    float d1y = b->min.y - a->max.y;
    float d2x = a->min.x - b->max.x;
    float d2y = a->min.y - b->max.y;

    if (d1x > 0.0f || d1y > 0.0f)
        return FALSE;

    if (d2x > 0.0f || d2y > 0.0f)
        return FALSE;

    return TRUE;
}

void RunRigidBodySimulation() {
    float totalSimulationTime = 10; // The simulation will run for 10 seconds.
    float currentTime = 0; // This accumulates the time that has passed.
    float dt = 1; // Each step will take one second.
    
    InitializeRigidBodies();
    PrintRigidBodies();
    
    while (currentTime < totalSimulationTime) {
        Sleep(dt);
        
        for (int i = 0; i < NUM_RIGID_BODIES; ++i) {
            RigidBody *rigidBody = &rigidBodies[i];
            ComputeForceAndTorque(rigidBody);
            Vector2 linearAcceleration = (Vector2){rigidBody->force.x / rigidBody->shape.mass, rigidBody->force.y / rigidBody->shape.mass};
            rigidBody->linearVelocity.x += linearAcceleration.x * dt;
            rigidBody->linearVelocity.y += linearAcceleration.y * dt;
            rigidBody->position.x += rigidBody->linearVelocity.x * dt;
            rigidBody->position.y += rigidBody->linearVelocity.y * dt;
            float angularAcceleration = rigidBody->torque / rigidBody->shape.momentOfInertia;
            rigidBody->angularVelocity += angularAcceleration * dt;
            rigidBody->angle += rigidBody->angularVelocity * dt;
        }
        
        PrintRigidBodies();
        currentTime += dt;
    }
}




int main(){
    RunRigidBodySimulation();
    return 0;
}