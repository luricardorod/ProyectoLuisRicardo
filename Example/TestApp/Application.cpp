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
	rotationCam = 0;
	worldLights.dirGlobal = CVector4D(0, 0, -1, 0);
	worldLights.colorGlobal = CVector4D(.2, 0.2, 0.2, 0);
	worldLights.colorPoint = CVector4D(.7, .7, .7, 0);
	worldLights.posPoint = CVector4D(0, -1, 1, 0);
	worldLights.posCamera = &PositionCamera;
	worldLights.flagWireFrame = false;
}

void TestApp::CreateAssets() {	
	PrimitiveMgr.SetVP(&VP);


	int index = PrimitiveMgr.CreateSprite("riu.png", 48, 80, 600, 720, 7, 70, 80);

	primitiveFigs[0].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateSprite("lol.png", 20, 20, 10, 705, 0, 256, 256);

	primitiveFigs[1].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);

	index = PrimitiveMgr.CreateSprite("radar.png", 294, 298, 0, 720, 0, 294, 298);

	primitiveFigs[2].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	index = PrimitiveMgr.CreateObject3D("Models/CerdoNuevo.X");
	primitiveFigs[3].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	index = PrimitiveMgr.CreateObject3D("Models/Scene.X");
	primitiveFigs[4].CreateInstance(PrimitiveMgr.GetPrimitive(index), &VP);
	
	index = PrimitiveMgr.CreateObject3D("Models/NuBatman.X");
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
	//primitiveFigs[2].TranslateAbsolute(0, -0.5, 0);
	//primitiveFigs[2].Update();
	primitiveFigs[3].TranslateAbsolute(100.0f, 0, -20);
	primitiveFigs[3].Update();
	primitiveFigs[8].TranslateAbsolute(100.0f, 0, -20);
	primitiveFigs[8].Update();

	primitiveFigs[9].TranslateAbsolute(-100.0f, 0, 20);
	primitiveFigs[9].Update();
	/*primitiveFigs[CROC].TranslateAbsolute(-43.871941f, 0.064795f, -58.153839f);
	primitiveFigs[CROC].RotateXAbsolute(0.0f);
	primitiveFigs[CROC].RotateYAbsolute(207.731613f);
	primitiveFigs[CROC].RotateXAbsolute(0.0f);
	primitiveFigs[CROC].ScaleAbsolute(0.151794f);*/
	/*primitiveFigs[CROC].Update();

	primitiveFigs[LINK].TranslateAbsolute(12.499269f, -0.079694f, -63.019135f);
	primitiveFigs[LINK].RotateXAbsolute(0.0f);
	primitiveFigs[LINK].RotateYAbsolute(-410.563721f);
	primitiveFigs[LINK].RotateXAbsolute(0.0f);
	primitiveFigs[LINK].ScaleAbsolute(0.114174f);
	primitiveFigs[LINK].Update();
	primitiveFigs[HOUSE_L].TranslateAbsolute(56.104416f, 0.574123f, 29.808973f);
	primitiveFigs[HOUSE_L].RotateXAbsolute(0.0f);
	primitiveFigs[HOUSE_L].RotateYAbsolute(141.33f);
	primitiveFigs[HOUSE_L].RotateXAbsolute(0.0f);
	primitiveFigs[HOUSE_L].ScaleAbsolute(0.670580f);
	primitiveFigs[HOUSE_L].Update();
	primitiveFigs[HOUSE_R].TranslateAbsolute(-82.823868f, 0.443788f, 34.599747f);
	primitiveFigs[HOUSE_R].RotateXAbsolute(0.0f);
	primitiveFigs[HOUSE_R].RotateYAbsolute(51.43101f);
	primitiveFigs[HOUSE_R].RotateXAbsolute(0.0f);
	primitiveFigs[HOUSE_R].ScaleAbsolute(0.670580f);
	primitiveFigs[HOUSE_R].Update();
	primitiveFigs[CERDO].TranslateAbsolute(-14.064236f, 50, -29.351925f);
	primitiveFigs[CERDO].RotateXAbsolute(100);
	primitiveFigs[CERDO].RotateYAbsolute(0.0f);
	primitiveFigs[CERDO].RotateZAbsolute(0.0f);
	primitiveFigs[CERDO].ScaleAbsolute(27.208776f);
	primitiveFigs[CERDO].Update();*/
	worldLights.posPoint = CVector4D(primitiveFigs[7].position.m30, primitiveFigs[7].position.m31, primitiveFigs[7].position.m32,0);
	PositionLight = worldLights.posPoint;
	InitWorldBullet();
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
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -56, 0));

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

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar	mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(2, 10, 0));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

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
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);

		}
		else
		{
			trans = obj->getWorldTransform();
		}
		printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		
	}
}

void TestApp::OnUpdate() {
	UpdateWorldBullet();
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
	
	for (int i = 0; i < TOTAL_INSTANCES; i++) {
		primitiveFigs[i].Draw();
	}

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

	}
}

void TestApp::OnPause() {

}

void TestApp::OnResume() {

}

void TestApp::OnReset() {

}