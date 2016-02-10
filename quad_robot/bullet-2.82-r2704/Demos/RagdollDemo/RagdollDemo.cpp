/*
Bullet Continuous Collision Detection and Physics Library
Ragdoll Demo
Copyright (c) 2007 Starbreeze Studios

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Written by: Marten Svanfeldt
*/

#define CONSTRAINT_DEBUG_SIZE 0.2f


#include "btBulletDynamicsCommon.h"
#include "GlutStuff.h"
#include "GL_ShapeDrawer.h"

#include "LinearMath/btIDebugDraw.h"

#include "GLDebugDrawer.h"
#include "RagdollDemo.h"

#include "BulletDynamics/ConstraintSolver/btHingeConstraint.h"

#include "GLDebugDrawer.h" 


// Enrico: Shouldn't these three variables be real constants and not defines?

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4     0.785398163397448309616
#endif



class RagDoll
{
	enum
	{
		BODYPART_PELVIS = 0,
		BODYPART_SPINE,
		BODYPART_HEAD,

		BODYPART_LEFT_UPPER_LEG,
		BODYPART_LEFT_LOWER_LEG,

		BODYPART_RIGHT_UPPER_LEG,
		BODYPART_RIGHT_LOWER_LEG,

		BODYPART_LEFT_UPPER_ARM,
		BODYPART_LEFT_LOWER_ARM,

		BODYPART_RIGHT_UPPER_ARM,
		BODYPART_RIGHT_LOWER_ARM,

		BODYPART_COUNT
	};

	enum
	{
		JOINT_PELVIS_SPINE = 0,
		JOINT_SPINE_HEAD,

		JOINT_LEFT_HIP,
		JOINT_LEFT_KNEE,

		JOINT_RIGHT_HIP,
		JOINT_RIGHT_KNEE,

		JOINT_LEFT_SHOULDER,
		JOINT_LEFT_ELBOW,

		JOINT_RIGHT_SHOULDER,
		JOINT_RIGHT_ELBOW,

		JOINT_COUNT
	};

	btDynamicsWorld* m_ownerWorld;
	btCollisionShape* m_shapes[BODYPART_COUNT];
	btRigidBody* m_bodies[BODYPART_COUNT];
	btTypedConstraint* m_joints[JOINT_COUNT];

	btRigidBody* localCreateRigidBody (btScalar mass, const btTransform& startTransform, btCollisionShape* shape)
	{
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			shape->calculateLocalInertia(mass,localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		m_ownerWorld->addRigidBody(body);

		return body;
	}

public:
	RagDoll (btDynamicsWorld* ownerWorld, const btVector3& positionOffset)
		: m_ownerWorld (ownerWorld)
	{

		// Setup the geometry
		m_shapes[BODYPART_PELVIS] = new btCapsuleShape(btScalar(0.15), btScalar(0.20));
		m_shapes[BODYPART_SPINE] = new btCapsuleShape(btScalar(0.15), btScalar(0.28));
		m_shapes[BODYPART_HEAD] = new btCapsuleShape(btScalar(0.10*4), btScalar(0.05));
		m_shapes[BODYPART_LEFT_UPPER_LEG] = new btCapsuleShape(btScalar(0.07), btScalar(0.45));
		m_shapes[BODYPART_LEFT_LOWER_LEG] = new btCapsuleShape(btScalar(0.05), btScalar(0.37));
		m_shapes[BODYPART_RIGHT_UPPER_LEG] = new btCapsuleShape(btScalar(0.07), btScalar(0.45));
		m_shapes[BODYPART_RIGHT_LOWER_LEG] = new btCapsuleShape(btScalar(0.05), btScalar(0.37));
		m_shapes[BODYPART_LEFT_UPPER_ARM] = new btCapsuleShape(btScalar(0.05), btScalar(0.33));
		m_shapes[BODYPART_LEFT_LOWER_ARM] = new btCapsuleShape(btScalar(0.04), btScalar(0.25));
		m_shapes[BODYPART_RIGHT_UPPER_ARM] = new btCapsuleShape(btScalar(0.05), btScalar(0.33));
		m_shapes[BODYPART_RIGHT_LOWER_ARM] = new btCapsuleShape(btScalar(0.04), btScalar(0.25));

		// Setup all the rigid bodies
		btTransform offset; offset.setIdentity();
		offset.setOrigin(positionOffset);

		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(0.), btScalar(1.), btScalar(0.)));
		m_bodies[BODYPART_PELVIS] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_PELVIS]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(0.), btScalar(1.2), btScalar(0.)));
		m_bodies[BODYPART_SPINE] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_SPINE]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(0.), btScalar(1.6), btScalar(0.)));
		m_bodies[BODYPART_HEAD] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_HEAD]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(-0.18), btScalar(0.65), btScalar(0.)));
		m_bodies[BODYPART_LEFT_UPPER_LEG] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_UPPER_LEG]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(-0.18), btScalar(0.2), btScalar(0.)));
		m_bodies[BODYPART_LEFT_LOWER_LEG] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_LOWER_LEG]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(0.18), btScalar(0.65), btScalar(0.)));
		m_bodies[BODYPART_RIGHT_UPPER_LEG] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_UPPER_LEG]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(0.18), btScalar(0.2), btScalar(0.)));
		m_bodies[BODYPART_RIGHT_LOWER_LEG] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_LOWER_LEG]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(-0.35), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,M_PI_2);
		m_bodies[BODYPART_LEFT_UPPER_ARM] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_UPPER_ARM]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(-0.7), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,M_PI_2);
		m_bodies[BODYPART_LEFT_LOWER_ARM] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_LEFT_LOWER_ARM]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(0.35), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,-M_PI_2);
		m_bodies[BODYPART_RIGHT_UPPER_ARM] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_UPPER_ARM]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(0.7), btScalar(1.45), btScalar(0.)));
		transform.getBasis().setEulerZYX(0,0,-M_PI_2);
		m_bodies[BODYPART_RIGHT_LOWER_ARM] = localCreateRigidBody(btScalar(1.), offset*transform, m_shapes[BODYPART_RIGHT_LOWER_ARM]);

		// Setup some damping on the m_bodies
		for (int i = 0; i < BODYPART_COUNT; ++i)
		{
			m_bodies[i]->setDamping(0.05, 0.85);
			m_bodies[i]->setDeactivationTime(0.8);
			m_bodies[i]->setSleepingThresholds(1.6, 2.5);
		}

		// Now setup the constraints
		btHingeConstraint* hingeC;
		btConeTwistConstraint* coneC;

		btTransform localA, localB;

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(btVector3(btScalar(0.), btScalar(0.15), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.15), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_SPINE], localA, localB);
		hingeC->setLimit(btScalar(-M_PI_4), btScalar(M_PI_2));
		m_joints[JOINT_PELVIS_SPINE] = hingeC;
		hingeC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_PELVIS_SPINE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,M_PI_2); localA.setOrigin(btVector3(btScalar(0.), btScalar(0.30), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_2); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_HEAD], localA, localB);
		coneC->setLimit(M_PI_4, M_PI_4, M_PI_2);
		m_joints[JOINT_SPINE_HEAD] = coneC;
		coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_SPINE_HEAD], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,-M_PI_4*5); localA.setOrigin(btVector3(btScalar(-0.18), btScalar(-0.10), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,-M_PI_4*5); localB.setOrigin(btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_LEFT_UPPER_LEG], localA, localB);
		coneC->setLimit(M_PI_4, M_PI_4, 0);
		m_joints[JOINT_LEFT_HIP] = coneC;
		coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_HIP], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_LEFT_UPPER_LEG], *m_bodies[BODYPART_LEFT_LOWER_LEG], localA, localB);
		hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
		m_joints[JOINT_LEFT_KNEE] = hingeC;
		hingeC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_KNEE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,M_PI_4); localA.setOrigin(btVector3(btScalar(0.18), btScalar(-0.10), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_4); localB.setOrigin(btVector3(btScalar(0.), btScalar(0.225), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_PELVIS], *m_bodies[BODYPART_RIGHT_UPPER_LEG], localA, localB);
		coneC->setLimit(M_PI_4, M_PI_4, 0);
		m_joints[JOINT_RIGHT_HIP] = coneC;
		coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_HIP], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(btVector3(btScalar(0.), btScalar(-0.225), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(btVector3(btScalar(0.), btScalar(0.185), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_RIGHT_UPPER_LEG], *m_bodies[BODYPART_RIGHT_LOWER_LEG], localA, localB);
		hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
		m_joints[JOINT_RIGHT_KNEE] = hingeC;
		hingeC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_KNEE], true);


		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,M_PI); localA.setOrigin(btVector3(btScalar(-0.2), btScalar(0.15), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_2); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_LEFT_UPPER_ARM], localA, localB);
		coneC->setLimit(M_PI_2, M_PI_2, 0);
		coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_joints[JOINT_LEFT_SHOULDER] = coneC;
		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_SHOULDER], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_LEFT_UPPER_ARM], *m_bodies[BODYPART_LEFT_LOWER_ARM], localA, localB);
//		hingeC->setLimit(btScalar(-M_PI_2), btScalar(0));
		hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
		m_joints[JOINT_LEFT_ELBOW] = hingeC;
		hingeC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_LEFT_ELBOW], true);



		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,0,0); localA.setOrigin(btVector3(btScalar(0.2), btScalar(0.15), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,0,M_PI_2); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.18), btScalar(0.)));
		coneC = new btConeTwistConstraint(*m_bodies[BODYPART_SPINE], *m_bodies[BODYPART_RIGHT_UPPER_ARM], localA, localB);
		coneC->setLimit(M_PI_2, M_PI_2, 0);
		m_joints[JOINT_RIGHT_SHOULDER] = coneC;
		coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_SHOULDER], true);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0,M_PI_2,0); localA.setOrigin(btVector3(btScalar(0.), btScalar(0.18), btScalar(0.)));
		localB.getBasis().setEulerZYX(0,M_PI_2,0); localB.setOrigin(btVector3(btScalar(0.), btScalar(-0.14), btScalar(0.)));
		hingeC =  new btHingeConstraint(*m_bodies[BODYPART_RIGHT_UPPER_ARM], *m_bodies[BODYPART_RIGHT_LOWER_ARM], localA, localB);
//		hingeC->setLimit(btScalar(-M_PI_2), btScalar(0));
		hingeC->setLimit(btScalar(0), btScalar(M_PI_2));
		m_joints[JOINT_RIGHT_ELBOW] = hingeC;
		hingeC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

		m_ownerWorld->addConstraint(m_joints[JOINT_RIGHT_ELBOW], true);
	}

	virtual	~RagDoll ()
	{
		int i;

		// Remove all constraints
		for ( i = 0; i < JOINT_COUNT; ++i)
		{
			m_ownerWorld->removeConstraint(m_joints[i]);
			delete m_joints[i]; m_joints[i] = 0;
		}

		// Remove all bodies and shapes
		for ( i = 0; i < BODYPART_COUNT; ++i)
		{
			m_ownerWorld->removeRigidBody(m_bodies[i]);
			
			delete m_bodies[i]->getMotionState();

			delete m_bodies[i]; m_bodies[i] = 0;
			delete m_shapes[i]; m_shapes[i] = 0;
		}
	}
};

static RagdollDemo* ragdollDemo; 

bool myContactProcessedCallback(btManifoldPoint& cp, 
                                void* body0, void* body1)
{
    int *ID1, *ID2; 
    btCollisionObject* o1 = static_cast<btCollisionObject*>(body0); 
    btCollisionObject* o2 = static_cast<btCollisionObject*>(body1);
    int groundID = 9;

    ID1 = static_cast<int*>(o1->getUserPointer()); 
    ID2 = static_cast<int*>(o2->getUserPointer());
    ragdollDemo->touches[*ID1] = 1;
    ragdollDemo->touches[*ID2] = 1;
    ragdollDemo->touchPoints[*ID1] = cp.m_positionWorldOnB; 
 	ragdollDemo->touchPoints[*ID2] = cp.m_positionWorldOnB;
    //printf("ID1 = %d, ID2 = %d\n", *ID1, *ID2);

    return false;
}


void RagdollDemo::initPhysics()
{
	// Setup the basic world
	pause = false;
	ragdollDemo = this;
	gContactProcessedCallback = myContactProcessedCallback;


	setTexturing(true);
	setShadows(true);

	setCameraDistance(btScalar(5.));

	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	btVector3 worldAabbMin(-10000,-10000,-10000);
	btVector3 worldAabbMax(10000,10000,10000);
	m_broadphase = new btAxisSweep3 (worldAabbMin, worldAabbMax);

	m_solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	//m_dynamicsWorld->getDispatchInfo().m_useConvexConservativeDistanceUtil = true;
	//m_dynamicsWorld->getDispatchInfo().m_convexConservativeDistanceThreshold = 0.01f;



	// Setup a big ground box
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(200.),btScalar(10.),btScalar(200.)));
		m_collisionShapes.push_back(groundShape);
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0,-10,0));

#define CREATE_GROUND_COLLISION_OBJECT 1
#ifdef CREATE_GROUND_COLLISION_OBJECT
		btCollisionObject* fixedGround = new btCollisionObject();
		fixedGround->setCollisionShape(groundShape);
		fixedGround->setWorldTransform(groundTransform);

		(fixedGround)->setUserPointer( &(IDs[9]) );

		m_dynamicsWorld->addCollisionObject(fixedGround);
#else
		localCreateRigidBody(btScalar(0.),groundTransform,groundShape);
#endif //CREATE_GROUND_COLLISION_OBJECT

	}

	// Spawn one ragdoll
	//btVector3 startOffset(1,0.5,0);
	//spawnRagdoll(startOffset);
	//startOffset.setValue(-1,0.5,0);
	//spawnRagdoll(startOffset);

	for(int i = 0; i < 10; i ++)
	{
		IDs[i] = i;
	}

	CreateBox(0, 0., 2.2, 0., .9, .9, 0.2); // Create the box 

	CreateCylinder(1, 0., 2.2, 2. , 0.2, 1., 0., M_PI_2, 0.);
	CreateCylinder(2, 0., 2.2, -2. , 0.2, 1., 0., M_PI_2, 0.);
	CreateCylinder(3, 2., 2.2, 0. , 0.2, 1., 0., 0., M_PI_2);
	CreateCylinder(4, -2., 2.2, 0. , 0.2, 1., 0., 0., M_PI_2);
	
	CreateCylinder(5, 0., 1., 3.2 , 0.2, 1, 0., 0., 0.);
	CreateCylinder(6, 0., 1., -3.2 , 0.2, 1, 0., 0., 0.);
	CreateCylinder(7, 3.2, 1., 0. , 0.2, 1, 0.0, 0., 0.);
	CreateCylinder(8, -3.2, 1., 0. , 0.2, 1, 0., 0., 0.);

	CreateHinge(0,    4, 8,    -3.2, 2.2, 0.0,    0, 0, -1);
	CreateHinge(1,    3, 7,    3.2, 2.2, 0.0,    0, 0, 1);
	CreateHinge(2,    2, 6,    0., 2.2, -3.2,    -1, 0, 0);
	CreateHinge(3,    1, 5,    0., 2.2, 3.2,    1, 0, 0);

	CreateHinge(4,    1, 0,    0., 2.2, 1.,    1, 0, 0);
	CreateHinge(5,    2, 0,    0., 2.2, -1.,    -1, 0, 0);
	CreateHinge(6,    3, 0,    1., 2.2, 0.0,    0, 0, 1);
	CreateHinge(7,    4, 0,    -1., 2.2, 0.0,    0, 0, -1);





	clientResetScene();		
}

void RagdollDemo::spawnRagdoll(const btVector3& startOffset)
{
	RagDoll* ragDoll = new RagDoll (m_dynamicsWorld, startOffset);
	m_ragdolls.push_back(ragDoll);
}	


int numcalls = 0;
void RagdollDemo::clientMoveAndDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	//simple dynamics world doesn't handle fixed-time-stepping
	float ms = getDeltaTimeMicroseconds();

	float minFPS = 1000000.f/60.f;
	if (ms > minFPS)
		ms = minFPS;

	if (m_dynamicsWorld)
	{
		if (!pause || (pause && oneStep)) { 
			numcalls ++;

			if(numcalls % 20 == 0)
			{
				ActuateJoint(0, (rand()/double(RAND_MAX))*90.-45., -90., ms / 1000000.f); 
				ActuateJoint(1, (rand()/double(RAND_MAX))*90.-45, 90., ms / 1000000.f); 
				ActuateJoint(2, (rand()/double(RAND_MAX))*90.-45, 90., ms / 1000000.f); 
				ActuateJoint(3, (rand()/double(RAND_MAX))*90.-45, -90., ms / 1000000.f);

				ActuateJoint(4, (rand()/double(RAND_MAX))*90.-45, -90., ms / 1000000.f); 
				ActuateJoint(5, (rand()/double(RAND_MAX))*90.-45, 90., ms / 1000000.f); 
				ActuateJoint(6, (rand()/double(RAND_MAX))*90.-45, 90., ms / 1000000.f); 
				ActuateJoint(7, (rand()/double(RAND_MAX))*90.-45, -90., ms / 1000000.f); 
			}
			for(int i = 0; i < 10; i ++)
			{
				touches[i] = 0;
			}
		    m_dynamicsWorld->stepSimulation(ms / 1000000.f);

		    for(int i = 0; i < 10; i++)
		    {
		    	printf("%d", touches[i]);
		    }
		    printf("\n");
		    oneStep = false;
		}
		
		//optional but useful: debug drawing
		m_dynamicsWorld->debugDrawWorld();


	}

	renderme(); 

	glFlush();

	glutSwapBuffers();
}

void RagdollDemo::displayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	renderme();

	//optional but useful: debug drawing
	if (m_dynamicsWorld)
		m_dynamicsWorld->debugDrawWorld();

	glFlush();
	glutSwapBuffers();
}

void RagdollDemo::keyboardCallback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'e':
		{
		btVector3 startOffset(0,2,0);
		spawnRagdoll(startOffset);
		break;
		}
	case 'p':
		{
			pause = !pause;
		}
	case 'i':
		{
			if(pause)
			{
				oneStep = true;
			}
		}
	default:
		DemoApplication::keyboardCallback(key, x, y);
	}

	
}



void	RagdollDemo::exitPhysics()
{
	for(int i = 0; i < 9; i ++)
	{
		DeleteObject(i);
	}
	DestroyHinge(0);
}



/*****START CUSTOM RAGDOLL CODE********/


void RagdollDemo::CreateBox( int index, 
 double x, double y, double z,
 double length, double width, double height)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(btScalar(x), btScalar(y), btScalar(z)));

	geom[index] = new btBoxShape(btVector3(btScalar(length),btScalar(height),btScalar(width)));


	body[index] = localCreateRigidBody(1, transform, geom[index]);
	(body[index])->setUserPointer( &(IDs[index]) );

	m_dynamicsWorld->addRigidBody(body[index]);	
}

void RagdollDemo::CreateCylinder( int index, 
 double x, double y, double z,
 double radius, double length,
 double yaw, double pitch, double roll) 
{ 
 
 	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(btScalar(x), btScalar(y), btScalar(z)));
	transform.setRotation(btQuaternion(btScalar(yaw),btScalar(pitch),btScalar(roll)));

	geom[index] = new btCylinderShape(btVector3(btScalar(radius),btScalar(length),btScalar(radius)));


	body[index] = localCreateRigidBody(1, transform, geom[index]);
	(body[index])->setUserPointer( &(IDs[index]) );

	m_dynamicsWorld->addRigidBody(body[index]);	
 }

 void RagdollDemo::DeleteObject( int index ) 
 {
        m_dynamicsWorld->removeRigidBody( body[index] ); 

        delete body[index];
        delete geom[index];
 } 


void RagdollDemo::CreateHinge(int index, int body1, int body2, double x, double y, double z, double ax, double ay, double az)
{
	btVector3 p(x, y, z);
  	btVector3 a(ax, ay, az);

  	//p = flipZY(p);
  	//a = flipZY(a);

  	btVector3 p1 = PointWorldToLocal(body1, p);
  	btVector3 p2 = PointWorldToLocal(body2, p);

  	btVector3 a1 = AxisWorldToLocal(body1, a);
  	btVector3 a2 = AxisWorldToLocal(body2, a);

  	joints[index] = new btHingeConstraint(*body[body1], *body[body2],
                                                   p1, p2,
                                                   a1, a2, false); 

  	


  	if ( index==4 )
      	  joints[index]->setLimit( (-45. + 90.)*3.14159/180., (45. + 90.)*3.14159/180.); 
	else if ( index==5 )
	      joints[index]->setLimit( (-45. - 90.)*3.14159/180., (45. - 90.)*3.14159/180.); 
	else if ( index==6 )
	      joints[index]->setLimit( (-45. + 90.)*3.14159/180., (45. + 90.)*3.14159/180.); 
	else if ( index==7 )
	      joints[index]->setLimit( (-45. - 90.)*3.14159/180., (45. - 90.)*3.14159/180.);
	else if ( index==0 )
	      joints[index]->setLimit( (-45. - 90.)*3.14159/180., (45. - 90.)*3.14159/180.); 
	else if ( index==1 )
	      joints[index]->setLimit( (-45. + 90.)*3.14159/180., (45. + 90.)*3.14159/180.); 
	else if ( index==2 )
	      joints[index]->setLimit( (-45. - 90.)*3.14159/180., (45. - 90.)*3.14159/180.); 
	else if ( index==3 )
	      joints[index]->setLimit( (-45. + 90.)*3.14159/180., (45. + 90.)*3.14159/180.); 

	

  	m_dynamicsWorld->addConstraint( joints[index] , true );
}

void RagdollDemo::DestroyHinge(int index) {
	m_dynamicsWorld->removeConstraint( joints[index] );
	delete joints[index];
}


btVector3 RagdollDemo::PointWorldToLocal(int index, btVector3 &p) {
  btTransform local1 = body[index]->getCenterOfMassTransform().inverse();
  return local1 * p;
}

btVector3 RagdollDemo::AxisWorldToLocal(int index, btVector3 &a) {
  btTransform local1 = body[index]->getCenterOfMassTransform().inverse();
  btVector3 zero(0,0,0);
  local1.setOrigin(zero);
  return local1 * a;
}

btVector3 RagdollDemo::flipZY(btVector3 input) {
  btScalar temp;
  temp = input[1];
  input[1] = input[2];
  input[2] = temp;
  return input;
}

void RagdollDemo::ActuateJoint(int jointIndex, double desiredAngle, 
                  double jointOffset, double timeStep) {

	joints[jointIndex]->enableMotor(true);
  	joints[jointIndex]->setMaxMotorImpulse(btScalar(1.));

	btScalar angle((desiredAngle + jointOffset)*3.14159/180.);
	btScalar dt(timeStep);
	joints[jointIndex]->setMotorTarget(angle, dt); 
    
}

void RagdollDemo::ActuateJoint2(int jointIndex, double desiredAngle, 
                  double jointOffset, double timeStep) {

	btScalar curAngle = joints[jointIndex]->getHingeAngle();
	btScalar angDif = desiredAngle - curAngle;
	joints[jointIndex]->enableAngularMotor(true, angDif, 10.0);


	//joints[index]->setMotorTarget(btScalar((desiredAngle + jointOffset)*3.14159/180.), btScalar(timeStep)); 
    
}