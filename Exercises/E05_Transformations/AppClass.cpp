#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Zhao Jin - zj5148@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	//Allocate the memory for the Meshes
	// First row
	positions.emplace_back(3.0f, 0.0f, 0.0f);
	positions.emplace_back(4.0f, 0.0f, 0.0f);
	positions.emplace_back(6.0f, 0.0f, 0.0f);
	positions.emplace_back(7.0f, 0.0f, 0.0f);

	// Second row
	positions.emplace_back(0.0f, 1.0f, 0.0f);
	positions.emplace_back(2.0f, 1.0f, 0.0f);
	positions.emplace_back(8.0f, 1.0f, 0.0f);
	positions.emplace_back(10.0f, 1.0f, 0.0f);

	// Third row
	positions.emplace_back(0.0f, 2.0f, 0.0f);
	for (uint i = 2; i <= 8; i++) {
		positions.emplace_back(i, 2.0f, 0.0f);
	}
	positions.emplace_back(10.0f, 2.0f, 0.0f);

	// Fourth row
	for (uint i = 0; i <= 10; i++) {
		positions.emplace_back(i, 3.0f, 0.0f);
	}

	// Fifth row
	positions.emplace_back(1.0f, 4.0f, 0.0f);
	positions.emplace_back(2.0f, 4.0f, 0.0f);
	positions.emplace_back(4.0f, 4.0f, 0.0f);
	positions.emplace_back(5.0f, 4.0f, 0.0f);
	positions.emplace_back(6.0f, 4.0f, 0.0f);
	positions.emplace_back(8.0f, 4.0f, 0.0f);
	positions.emplace_back(9.0f, 4.0f, 0.0f);

	// Sixth row
	for (uint i = 2; i <= 8; i++) {
		positions.emplace_back(i, 5.0f, 0.0f);
	}

	// Seventh row
	positions.emplace_back(3.0f, 6.0f, 0.0f);
	positions.emplace_back(7.0f, 6.0f, 0.0f);

	// Eighth row
	positions.emplace_back(2.0f, 7.0f, 0.0f);
	positions.emplace_back(8.0f, 7.0f, 0.0f);

	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_BLACK);
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

	// Get a timer
	static float fTimer = 0; // Store the new timer
	static uint uClock = m_pSystem->GenClock(); // Generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); // Get the delta time for that timer

	// Draw Meshes
	uint i = 0;
	static float fHorizontal = -10.0f;
	static vector3 v3Position(fHorizontal, 0.0f, 0.0f);

	v3Position.x = fHorizontal + fTimer;
	v3Position.y = cos(glm::radians(fTimer * 60.0f));

	//Calculate the model, view and projection matrix
	matrix4 m4Model = glm::translate(vector3(-5.0f, -3.0f, -15.0f)) * glm::translate(v3Position);

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	// Using one mesh, render it at all positions
	m_uMeshCount = positions.size();
	for (uint i = 0; i < m_uMeshCount; i++) {
		m_pMesh->Render(m4Projection, m4View, glm::translate(m4Model, positions[i]));
	}

	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

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
	//Release meshes
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}