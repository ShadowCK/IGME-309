#include "AppClass.h"

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
	pentagonMesh = new MyMesh();
	quadMesh = new MyMesh();

	//Generate a circle
	m_pMesh->GenerateCircle(2.0f, 100, vector3(0.60, 0.62, 1));
	//Generate a pentagon
	pentagonMesh->GenerateCircle(2.0f, 5, vector3(-5, 0, 0), vector3(0.99, 0.55, 0.33));
	//Generate a quad
	quadMesh->GenerateCircle(2.0f, 4, vector3(5, 0, 0), vector3(0.44, 0.88, 0.33));
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

	//Render the mesh
	m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	pentagonMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	quadMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));

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
	//Safely release the memory
	SafeDelete(m_pMesh);
	SafeDelete(pentagonMesh);
	SafeDelete(quadMesh);
	//release GUI
	ShutdownGUI();
}