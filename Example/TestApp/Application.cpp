#include "Application.h"
#include <SDL\SDL_mouse.h>
#include "Sprite.h"


enum {
	SCENE = 0,
	CROC,
	LINK,
	HOUSE_L,
	HOUSE_R,
	DRONE,
	CERDO,
	CERDOLIGHT,
	HOL,
	veno,
	TOTAL_INSTANCES
};
void TestApp::InitVars() {
	DtTimer.Init();
	Position	= CVector4D(0.0f, 0.0f, 0.0f, 0);
	PositionLight = CVector4D(0.0f, 0.0f, 0.0f, 0);
	Orientation = CVector4D(0.0f, 0.0f, 0.0f, 0);
	Scaling		= CVector4D(1.0f, 1.0f, 1.0f, 0);
	rotationCam = -3.11;
	worldLights.dirGlobal = CVector4D(0, 0, -1, 0);
	worldLights.colorGlobal = CVector4D(.2, 0.2, 0.2, 0);
	worldLights.colorPoint = CVector4D(.7, .7, .7, 0);
	worldLights.posPoint = CVector4D(0, -1, 1, 0);
	worldLights.posCamera = &PositionCamera;
	worldLights.flagWireFrame = false;
	worldLights.flagShadowMap = false;
	worldLights.LigthView = Identity();
	RTIndex = -1;

}

void TestApp::CreateAssets() {	

	GBufferPass = pFramework->pVideoDriver->CreateRT(4, BaseRT::RGBA8, BaseRT::F32, 0, 0);
	DeferredPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA8, BaseRT::F32, 0, 0);
	ShadowMapPass = pFramework->pVideoDriver->CreateRT(1, BaseRT::RGBA8, BaseRT::F32, 0, 0);


	PrimitiveMgr.SetVP(&VP);


	int index = PrimitiveMgr.CreateSprite("riu.png", 48, 80, 600, 720, 7, 70, 80);

	primitiveFigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateSprite("lol.png", 20, 20, 10, 705, 0, 256, 256);

	primitiveFigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateSprite("radar.png", 294, 298, 0, 720, 0, 294, 298);

	primitiveFigs[2].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateObject3D("Models/NuBatman.X");
	primitiveFigs[3].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	index = PrimitiveMgr.CreateObject3D("Models/Scene.X");
	//index = PrimitiveMgr.CreateObject3D("Models/NuBatman.X");
	//index = PrimitiveMgr.CreateObject3D("Models/cube2.X");

	primitiveFigs[4].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	
	index = PrimitiveMgr.CreateObject3D("Models/NuVenomJok.X");
	primitiveFigs[5].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	index = PrimitiveMgr.CreateObject3D("Models/CerdoNuevo.X");
	primitiveFigs[6].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	primitiveFigs[7].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	index = PrimitiveMgr.CreateObject3D("Models/NuCroc.X");
	primitiveFigs[8].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	index = PrimitiveMgr.CreateObject3D("Models/NuCroc.X");
	primitiveFigs[9].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	for (int i = 0; i < TOTAL_INSTANCES; i++) {
		primitiveFigs[i].lights = &worldLights;
	}
	int countertemp = 0;
	index = PrimitiveMgr.CreateObject3D("Models/cube2.X");
	for (int z = 0; z < 3; z++)
	{
		for (int  j = 0; j < 3; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				figsFisics[countertemp].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
				figsFisics[countertemp].TranslateAbsolute(i*2, j*2+20, z*2 - 50);
				figsFisics[countertemp].Update();
				figsFisics[countertemp].lights = &worldLights;
				countertemp++;
			}
		}
	}

	figsFisics[countertemp].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	figsFisics[countertemp].TranslateAbsolute(.5, 0, .5 - 50);
	figsFisics[countertemp].Update();
	figsFisics[countertemp].lights = &worldLights;
	countertemp++;
	figsFisics[countertemp].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	figsFisics[countertemp].TranslateAbsolute(3.5, 0, .5 - 50);
	figsFisics[countertemp].Update();
	figsFisics[countertemp].lights = &worldLights;
	countertemp++;
	figsFisics[countertemp].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	figsFisics[countertemp].TranslateAbsolute(.5, 0, 3 - 50);
	figsFisics[countertemp].Update();
	figsFisics[countertemp].lights = &worldLights;
	countertemp++;
	figsFisics[countertemp].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	figsFisics[countertemp].TranslateAbsolute(3.5, 0, 3 - 50);
	figsFisics[countertemp].Update();
	figsFisics[countertemp].lights = &worldLights;
	countertemp++;
	CMatrix4D View;
	PositionCamera = CVector4D(0.0f, -10.0f, 100.0f, 0);
	CVector4D Up = CVector4D(0.0f, 1.0f, 0.0f, 0);
	CVector4D LookAt = PositionCamera - Normalize(Orientation) * 10;

	View = LookAtRH(PositionCamera, LookAt, Up);
	CMatrix4D proj = PerspectiveFovRH(45*3.1416/180, 1280.0f / 720.0f, 0.1f, 10000.0f);
	VP = View * proj;

	//primitiveFigs[0].TranslateAbsolute(-50.0f,0,-10);
	primitiveFigs[0].Update();
	primitiveFigs[1].TranslateAbsolute(0, 0, 0);
	primitiveFigs[1].Update();
	primitiveFigs[5].ScaleAbsolute(.5);
	primitiveFigs[5].Update();
	primitiveFigs[2].TranslateAbsolute(25, -20, 0);
	primitiveFigs[2].Update();
	primitiveFigs[3].TranslateAbsolute(10.0f, 0, -20);
	primitiveFigs[3].Update();
	//primitiveFigs[4].TranslateAbsolute(0, -1000, -500);
	//primitiveFigs[4].ScaleAbsolute(1000);

	primitiveFigs[4].Update();
	primitiveFigs[7].TranslateAbsolute(-2, 10, 5);
	primitiveFigs[7].Update();
	primitiveFigs[8].TranslateAbsolute(100.0f, 0, -20);
	primitiveFigs[8].Update();

	primitiveFigs[9].TranslateAbsolute(-100.0f, 0, 20);
	primitiveFigs[9].Update();
	
	worldLights.posPoint = CVector4D(primitiveFigs[7].position.m30, primitiveFigs[7].position.m31, primitiveFigs[7].position.m32,0);
	PositionLight = worldLights.posPoint;
	InitWorldBullet();


	QuadIndex = PrimitiveMgr.CreateQuad();
	rendertargets[0].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[0], 0);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[1], 1);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[2], 2);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->vColorTextures[3], 3);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[0]->pDepthTexture, 4);

	rendertargets[0].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	rendertargets[1].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	rendertargets[2].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	rendertargets[3].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	rendertargets[4].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	rendertargets[5].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	rendertargets[6].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	rendertargets[7].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	rendertargets[8].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	rendertargets[9].CreateInstance(PrimitiveMgr.GetPrimitive(QuadIndex), &VP);
	
	rendertargets[0].ScaleAbsolute(.2);
	rendertargets[0].TranslateAbsolute(-.8, .8, 0);
	rendertargets[0].Update();

	rendertargets[1].ScaleAbsolute(.2);
	rendertargets[1].TranslateAbsolute(-.8, .4, 0);
	rendertargets[1].Update();

	rendertargets[2].ScaleAbsolute(.2);
	rendertargets[2].TranslateAbsolute(-.8, 0, 0);
	rendertargets[2].Update();

	rendertargets[3].ScaleAbsolute(.2);
	rendertargets[3].TranslateAbsolute(-.8, -.4, 0);
	rendertargets[3].Update();

	rendertargets[4].ScaleAbsolute(.2);
	rendertargets[4].TranslateAbsolute(-.8, -.8, 0);
	rendertargets[4].Update();

	rendertargets[5].ScaleAbsolute(.2);
	rendertargets[5].TranslateAbsolute(.8, .8, 0);
	rendertargets[5].Update();

	rendertargets[6].ScaleAbsolute(.2);
	rendertargets[6].TranslateAbsolute(.8, .4, 0);
	rendertargets[6].Update();

	rendertargets[7].ScaleAbsolute(.2);
	rendertargets[7].TranslateAbsolute(.8, 0, 0);
	rendertargets[7].Update();

	rendertargets[8].ScaleAbsolute(.2);
	rendertargets[8].TranslateAbsolute(.8, -.4, 0);
	rendertargets[8].Update();

	rendertargets[9].ScaleAbsolute(.2);
	rendertargets[9].TranslateAbsolute(.8, -.8, 0);
	rendertargets[9].Update();

	for (int i = 0; i < 10; i++) {
		rendertargets[i].lights = &worldLights;
	}
}

void TestApp::DestroyAssets() {
	PrimitiveMgr.DestroyPrimitives();
	DestroyWorldBullet();
}

void TestApp::InitWorldBullet()
{
	int i;
	/////-----initialization_start-----

	/////collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	/////use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	///-----initialization_end-----

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	

	///create a few basic rigid bodies

	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5
	{
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(500.), btScalar(50.), btScalar(500.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -49.8, 0));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}
	{
		//create a dynamic rigidbody

		btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(10.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		for (int z = 0; z < 3; z++)
		{
			for (int j = 0; j < 3; j++)
			{
				for (int i = 0; i < 3; i++)
				{
					startTransform.setOrigin(btVector3(i * 2, j * 2 + 20, z * 2 - 50));

					//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
					btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
					btRigidBody* body = new btRigidBody(rbInfo);

					dynamicsWorld->addRigidBody(body);
				}
			}
		}
		startTransform.setOrigin(btVector3(.5, 10, .5 - 50));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(body);
		startTransform.setOrigin(btVector3(3.5, 10, .5 - 50));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo1(mass, myMotionState, colShape, localInertia);
		body = new btRigidBody(rbInfo1);

		dynamicsWorld->addRigidBody(body);
		startTransform.setOrigin(btVector3(.5, 10, 3 - 50));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo2(mass, myMotionState, colShape, localInertia);
		body = new btRigidBody(rbInfo2);

		dynamicsWorld->addRigidBody(body);
		startTransform.setOrigin(btVector3(3.5, 10, 3 - 50));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo3(mass, myMotionState, colShape, localInertia);
		body = new btRigidBody(rbInfo3);
		dynamicsWorld->addRigidBody(body);

	}
}

void TestApp::DestroyWorldBullet()
{
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j<collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}

void TestApp::UpdateWorldBullet()
{
	dynamicsWorld->stepSimulation(1.f / 60.f, 10);

	//print positions of all objects
	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody* body = btRigidBody::upcast(obj);
		CMatrix4D pMatrix;
		CVector4D Q;
		
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);

		}
		else
		{
			trans = obj->getWorldTransform();
		}
		//printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		
		if (j != 0)
		{
			btQuaternion qua = trans.getRotation();

			Q.x = qua.getX();
			Q.y = qua.getY();
			Q.z = qua.getZ();
			Q.w = qua.getW();

			pMatrix.m00 = 1.0f - 2.0f*Q.y*Q.y - 2.0f*Q.z*Q.z;
			pMatrix.m01 = 2.0f*Q.x*Q.y - 2.0f * Q.z*Q.w;
			pMatrix.m02 = 2.0f*Q.x*Q.z + 2.0f * Q.w*Q.y;
			pMatrix.m03 = 0.0f;

			pMatrix.m10 = 2.0f*Q.x*Q.y + 2.0f*Q.z*Q.w;
			pMatrix.m11 = 1.0f - 2.0f*Q.x*Q.x - 2.0f*Q.z*Q.z;
			pMatrix.m12 = 2.0f*Q.y*Q.z - 2.0f*Q.x*Q.w;
			pMatrix.m13 = 0.0f;

			pMatrix.m20 = 2.0f*Q.x*Q.z - 2.0f*Q.y*Q.w;
			pMatrix.m21 = 2.0f*Q.y*Q.z + 2.0f*Q.x*Q.w;
			pMatrix.m22 = 1.0f - 2.0f * Q.x*Q.x - 2.0f * Q.y*Q.y;
			pMatrix.m23 = pMatrix.m30 = pMatrix.m31 = pMatrix.m32 = 0.0f;

			pMatrix.m33 = 1.0f;


			figsFisics[j-1].TranslateAbsolute(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
			figsFisics[j - 1].final = pMatrix * figsFisics[j - 1].position;
			//figsFisics[j - 1].Update();
		}
	}
}

void TestApp::OnUpdate() {
	if (runBullet)
	{
		UpdateWorldBullet();
	}
	DtTimer.Update();
	OnInput();
	Orientation = RotationY(rotationCam) * Orientation;
	PositionCamera = PositionCamera + Position.z * Normalize(Orientation)*30;
	CVector4D proyeccion = Normalize(Orientation) * Position.x * 30;
	proyeccion.y = 0;
	float temp = proyeccion.x;
	proyeccion.x = proyeccion.z;
	proyeccion.z = -temp;
	PositionCamera = PositionCamera + proyeccion;
	CMatrix4D View;
	CVector4D Up = CVector4D(0.0f, 1.0f, 0.0f, 0);

	CVector4D LookAt = PositionCamera - Normalize(Orientation) * 10;

	View = LookAtRH(PositionCamera, LookAt, Up);
	CMatrix4D proj = PerspectiveFovRH(45 * 3.1416 / 180, 1280.0f / 720.0f, 0.1f, 10000.0f);
	VP = View * proj;

	primitiveFigs[0].Update();

	primitiveFigs[7].TranslateAbsolute(PositionLight.x, PositionLight.y, PositionLight.z);
	primitiveFigs[7].Update();
	worldLights.posPoint = CVector4D(primitiveFigs[7].position.m30, primitiveFigs[7].position.m31, primitiveFigs[7].position.m32,0);

	float posx = (400 + PositionCamera.x)*0.4f / 800.0f;
	float posz = (400 + PositionCamera.z)*-0.7f / 800.0f;

	primitiveFigs[1].TranslateAbsolute(posx, posz, 0);
	primitiveFigs[1].Update();

	OnDraw();
	Orientation = CVector4D(0.0f, 0.0f, 10.0f, 0);
	Position = CVector4D(0.0f, 0.0f, 0.0f, 0);
}

void TestApp::OnDraw() {
	pFramework->pVideoDriver->Clear();

	
	CVector4D shadowLight = CVector4D(-PositionLight.x, -PositionLight.y + 1, -PositionLight.z, 0);
	CVector4D Up = CVector4D(0.0f, 1.0f, 0.0f, 0);
	CVector4D OrientationLigth = CVector4D(-10.0f, 10.0f, 10.0f, 0);
	CVector4D LookAt = shadowLight - Normalize(OrientationLigth) * 10;

	View = LookAtRH(shadowLight, LookAt, Up);
	CMatrix4D proj = PerspectiveFovRH(45 * 3.1416 / 180, 1280.0f / 720.0f,1, 10000.0f);
	CMatrix4D camerVP = VP;
	VP = View * proj;

	pFramework->pVideoDriver->PushRT(ShadowMapPass);
	pFramework->pVideoDriver->Clear();


	for (int i = 3; i < TOTAL_INSTANCES; i++) {
		primitiveFigs[i].SetSignature(Signature::LIGTHSHADOWMAP);
		primitiveFigs[i].Draw();
		primitiveFigs[i].SetSignature(Signature::FORWARD_PASS);
	}

	worldLights.LigthView = VP;
	VP = camerVP;

	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->PushRT(GBufferPass);
	pFramework->pVideoDriver->Clear();


	for (int i = 3; i < TOTAL_INSTANCES; i++) {
		primitiveFigs[i].SetSignature(Signature::GBUFF_PASS);
		primitiveFigs[i].Draw();
		primitiveFigs[i].SetSignature(Signature::FORWARD_PASS);
	}

	pFramework->pVideoDriver->PopRT();

	pFramework->pVideoDriver->Clear();
	for (int i = 0; i < PrimitiveMgr.primitives.size(); i++)
	{
		PrimitiveMgr.GetPrimitive(i)->SetTexture(pFramework->pVideoDriver->RTs[ShadowMapPass]->pDepthTexture, 0);
	}

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[ShadowMapPass]->vColorTextures[0], 0);
	rendertargets[0].SetSignature(Signature::LIGTHSHADOWMAP);
	rendertargets[0].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[0], 0);
	rendertargets[1].SetSignature(Signature::LIGTHSHADOWMAP);
	rendertargets[1].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[1], 0);
	rendertargets[2].SetSignature(Signature::LIGTHSHADOWMAP);
	rendertargets[2].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[2], 0);
	rendertargets[3].SetSignature(Signature::LIGTHSHADOWMAP);
	rendertargets[3].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[3], 0);
	rendertargets[4].SetSignature(Signature::LIGTHSHADOWMAP);
	rendertargets[4].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->pDepthTexture, 0);
	rendertargets[5].SetSignature(Signature::LIGTHSHADOWMAP);
	rendertargets[5].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[0], 0);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[1], 1);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[2], 2);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[3], 3);
	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->pDepthTexture, 4);
	rendertargets[6].SetSignature(Signature::DEFERRED_PASS);
	rendertargets[6].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[3], 0);
	rendertargets[7].SetSignature(Signature::LIGTHSHADOWMAP);
	rendertargets[7].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[3], 0);
	rendertargets[8].SetSignature(Signature::LIGTHSHADOWMAP);
	rendertargets[8].Draw();

	PrimitiveMgr.GetPrimitive(QuadIndex)->SetTexture(pFramework->pVideoDriver->RTs[GBufferPass]->vColorTextures[3], 0);
	rendertargets[9].SetSignature(Signature::LIGTHSHADOWMAP);
	rendertargets[9].Draw();

	worldLights.flagShadowMap = true;
	for (int i = 0; i < TOTAL_INSTANCES; i++) {
		primitiveFigs[i].SetSignature(Signature::FORWARD_PASS);
		primitiveFigs[i].Draw();
	}
	worldLights.flagShadowMap = false;

	
	pFramework->pVideoDriver->SwapBuffers();
}

void TestApp::OnInput() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	if (x < 100)
	{
		rotationCam += 0.5f*DtTimer.GetDTSecs();
	}
	if (x > 1180)
	{
		rotationCam -= 0.5f*DtTimer.GetDTSecs();
	}
	Orientation.x = (640 - x)/50.0f;
	Orientation.y = (y - 360)/20.0f;

	if (IManager.PressedKey(SDLK_UP)) {
		Position.z -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_DOWN)) {
		Position.z += 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_LEFT)) {
		Position.x -= 1.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_RIGHT)) {
		Position.x += 1.0f*DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_w)) {
		PositionLight.z -= 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_s)) {
		PositionLight.z += 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_a)) {
		PositionLight.x -= 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_d)) {
		PositionLight.x += 20.0f*DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_z)) {
		PositionLight.y += 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_x)) {
		PositionLight.y -= 20.0f*DtTimer.GetDTSecs();
	}

	if (IManager.PressedKey(SDLK_x)) {
		PositionLight.y -= 20.0f*DtTimer.GetDTSecs();
	}
	if (IManager.PressedKey(SDLK_m)) {
		worldLights.flagWireFrame = true;
	}
	if (IManager.PressedKey(SDLK_n)) {
		worldLights.flagWireFrame = false;
	}
	if (IManager.PressedKey(SDLK_p)) {
		printf("posx:%f\n", PositionCamera.x);
		printf("posy:%f\n", PositionCamera.y);
		printf("posz:%f\n", PositionCamera.z);
		printf("rot:%f\n", rotationCam);
	}
	if (IManager.PressedKey(SDLK_k)) {
		runBullet = true;
	}
	if (IManager.PressedKey(SDLK_l)) {
		runBullet = false;
	}
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}