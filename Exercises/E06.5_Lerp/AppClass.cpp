#include "AppClass.h"
#include <cstdlib>

void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Zhao Jin - zj5148@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	//Make MyMesh object
	m_pMesh = new MyMesh();

	//Generate a circle
	m_pMesh->GenerateCube(1.0f, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 view = m_pCameraMngr->GetViewMatrix(); // view Matrix
	matrix4 projection = m_pCameraMngr->GetProjectionMatrix(); // projection matrix

	/*
	// Sol 1
	// Matrix of the model, its position in the world
	float startPos = -5.0f;
	float endPos = 5.0f;
	static DWORD startTime = GetTickCount64();
	DWORD currentTime = GetTickCount64();
	DWORD timeElapsedMS = currentTime - startTime;
	float timeElapsedS = timeElapsedMS / 1000.0f; // In seconds
	float totalAnimationTime = 10.0f;

	float proportion = timeElapsedS / totalAnimationTime;
	float currentPos = glm::lerp(startPos, endPos, proportion);
	if (currentPos > endPos) currentPos = endPos;

	matrix4 transform = glm::translate(IDENTITY_M4, vector3(currentPos, 0.0f, 0.0f));
	*/

	// Sol 2
	// Time variables
	static float time = 0; // Store the new timer
	static uint uClock = m_pSystem->GenClock(); // Generate a new clock for that timer
	time += m_pSystem->GetDeltaTime(uClock); // Add the delta time to that 
	static float lerpTime = 0.2f; // How long it takes for each displacement
	// Position variables
	static std::vector<vector3> positions = {
		vector3(-4.0f,-2.0f, 5.0f),
		vector3(1.0f,-2.0f, 5.0f),
		vector3(-3.0f,-1.0f, 3.0f),
		vector3(2.0f,-1.0f, 3.0f),
		vector3(-2.0f, 0.0f, 0.0f),
		vector3(3.0f, 0.0f, 0.0f),
		vector3(-1.0f, 1.0f,-3.0f),
		vector3(4.0f, 1.0f,-3.0f),
		vector3(0.0f, 2.0f,-5.0f),
		vector3(5.0f, 2.0f,-5.0f),
		vector3(1.0f, 3.0f,-5.0f)
	};
	static int positionCount = positions.size();
	static int lastIndex = positionCount - 1;
	static int posIndex = 0; // Current position index
	int startPosIndex = abs(posIndex);
	int endPosIndex = abs(posIndex + 1);

	// Moves by lerping from current position to the next
	vector3 startPos = positions[startPosIndex];
	vector3 endPos = positions[endPosIndex];
	// Calculates the lerp progress
	float proportion = MapValue(time, 0.0f, lerpTime, 0.0f, 1.0f);
	// Target reached, updates data and go to the next position
	if (proportion >= 1.0f) {
		// Do not lerp beyond end point
		proportion = 1.0f;
		// Resets timer
		time = 0;
		posIndex = (posIndex + 1 + lastIndex) % (2 * lastIndex) - lastIndex;
		// std::cout << abs(posIndex) << std::endl; // abs(posIndex) = 0->10->0
	}
	// Calculates the current position
	vector3 currentPos = glm::lerp(startPos, endPos, proportion);

	matrix4 transform = glm::translate(IDENTITY_M4, vector3(currentPos)); // Transform matrix / model

	//Render the mesh
	m_pMesh->Render(projection, view, transform);

	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	for (uint i = 0; i < positionCount; i++) {
		m_pModelMngr->AddSphereToRenderList(glm::translate(positions[i]) * glm::scale(vector3(0.15f)), C_RED, RENDER_WIRE);
	}

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Safely release the memory
	SafeDelete(m_pMesh);
	//release GUI
	ShutdownGUI();
}