#include "MyRigidBody.h"
using namespace BTX;
//Allocation
uint MyRigidBody::SAT(MyRigidBody* const a_pOther)
{
	// Projected radii
	float radiusA;
	float radiusB;
	// Rotation matrix to align objects
	matrix3 rotationMatrix;
	matrix3 absRotationMatrix;

	// Axes of both objects
	std::vector<vector3> axesA;
	std::vector<vector3> axesB;
	axesA.push_back(GetModelMatrix()[0]);
	axesA.push_back(GetModelMatrix()[1]);
	axesA.push_back(GetModelMatrix()[2]);
	axesB.push_back(a_pOther->GetModelMatrix()[0]);
	axesB.push_back(a_pOther->GetModelMatrix()[1]);
	axesB.push_back(a_pOther->GetModelMatrix()[2]);

	// Convert centers to global space
	vector3 globalCenterA = vector3(GetModelMatrix() * vector4(m_v3Center, 1.0f));
	vector3 globalCenterB = vector3(a_pOther->GetModelMatrix() * vector4(a_pOther->m_v3Center, 1.0f));

	// Create rotation matrix
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rotationMatrix[i][j] = glm::dot(axesA[i], axesB[j]);
		}
	}

	// Calculate translation between centers
	vector3 translation = globalCenterA - globalCenterB;
	// Express translation in terms of the first object's axes
	translation = vector3(glm::dot(translation, axesA[0]), glm::dot(translation, axesA[1]), glm::dot(translation, axesA[2]));

	// Compute absolute value of the rotation matrix
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			absRotationMatrix[i][j] = glm::abs(rotationMatrix[i][j] + FLT_EPSILON); // Add epsilon for instances where the value is close to zero or zero
		}
	}

	// Test axes AX, AY, and AZ
	for (int i = 0; i < 3; i++)
	{
		radiusA = m_v3HalfWidth[i];
		radiusB = a_pOther->m_v3HalfWidth[0] * absRotationMatrix[i][0] + a_pOther->m_v3HalfWidth[1] * absRotationMatrix[i][1] + a_pOther->m_v3HalfWidth[2] * absRotationMatrix[i][2];
		if (glm::abs(translation[i]) > radiusA + radiusB)
		{
			return i + 1;
		}
	}

	// Test axes BX, BY, and BZ
	for (int i = 0; i < 3; i++)
	{
		radiusA = m_v3HalfWidth[0] * absRotationMatrix[0][i] + m_v3HalfWidth[1] * absRotationMatrix[1][i] + m_v3HalfWidth[2] * absRotationMatrix[2][i];
		radiusB = a_pOther->m_v3HalfWidth[i];
		if (glm::abs(translation[0] * rotationMatrix[0][i] + translation[1] * rotationMatrix[1][i] + translation[2] * rotationMatrix[2][i]) > radiusA + radiusB)
		{
			return i + 4;
		}
	}

	// Test cross product axes combinations
	// (AX, BX), (AX, BY), (AX, BZ), (AY, BX), (AY, BY), (AY, BZ), (AZ, BX), (AZ, BY), (AZ, BZ)

	// Test axis AXxBX
	radiusA = m_v3HalfWidth[1] * absRotationMatrix[2][0] + m_v3HalfWidth[2] * absRotationMatrix[1][0];
	radiusB = a_pOther->m_v3HalfWidth[1] * absRotationMatrix[0][2] + a_pOther->m_v3HalfWidth[2] * absRotationMatrix[0][1];
	if (glm::abs(translation[2] * rotationMatrix[1][0] - translation[1] * rotationMatrix[2][0]) > radiusA + radiusB)
	{
		return BTXs::eSATResults::SAT_AXxBX;
	}

	// Test axis AXxBY
	radiusA = m_v3HalfWidth[1] * absRotationMatrix[2][1] + m_v3HalfWidth[2] * absRotationMatrix[1][1];
	radiusB = a_pOther->m_v3HalfWidth[0] * absRotationMatrix[0][2] + a_pOther->m_v3HalfWidth[2] * absRotationMatrix[0][0];
	if (glm::abs(translation[2] * rotationMatrix[1][1] - translation[1] * rotationMatrix[2][1]) > radiusA + radiusB)
	{
		return BTXs::eSATResults::SAT_AXxBY;
	}

	// Test axis AXxBZ
	radiusA = m_v3HalfWidth[1] * absRotationMatrix[2][2] + m_v3HalfWidth[2] * absRotationMatrix[1][2];
	radiusB = a_pOther->m_v3HalfWidth[0] * absRotationMatrix[0][1] + a_pOther->m_v3HalfWidth[1] * absRotationMatrix[0][0];
	if (glm::abs(translation[2] * rotationMatrix[1][2] - translation[1] * rotationMatrix[2][2]) > radiusA + radiusB)
	{
		return BTXs::eSATResults::SAT_AXxBZ;
	}

	// Test axis AYxBX
	radiusA = m_v3HalfWidth[0] * absRotationMatrix[2][0] + m_v3HalfWidth[2] * absRotationMatrix[0][0];
	radiusB = a_pOther->m_v3HalfWidth[1] * absRotationMatrix[1][2] + a_pOther->m_v3HalfWidth[2] * absRotationMatrix[1][1];

	if (glm::abs(translation[0] * rotationMatrix[2][0] - translation[2] * rotationMatrix[0][0]) > radiusA + radiusB)
	{
		return BTXs::eSATResults::SAT_AYxBX;
	}

	// Test axis AYxBY
	radiusA = m_v3HalfWidth[0] * absRotationMatrix[2][1] + m_v3HalfWidth[2] * absRotationMatrix[0][1];
	radiusB = a_pOther->m_v3HalfWidth[0] * absRotationMatrix[1][2] + a_pOther->m_v3HalfWidth[2] * absRotationMatrix[1][0];
	if (glm::abs(translation[0] * rotationMatrix[2][1] - translation[2] * rotationMatrix[0][1]) > radiusA + radiusB)
	{
		return BTXs::eSATResults::SAT_AYxBY;
	}

	// Test axis AYxBZ
	radiusA = m_v3HalfWidth[0] * absRotationMatrix[2][2] + m_v3HalfWidth[2] * absRotationMatrix[0][2];
	radiusB = a_pOther->m_v3HalfWidth[0] * absRotationMatrix[1][1] + a_pOther->m_v3HalfWidth[1] * absRotationMatrix[1][0];
	if (glm::abs(translation[0] * rotationMatrix[2][2] - translation[2] * rotationMatrix[0][2]) > radiusA + radiusB)
	{
		return BTXs::eSATResults::SAT_AYxBZ;
	}

	// Test axis AZxBX
	radiusA = m_v3HalfWidth[0] * absRotationMatrix[1][0] + m_v3HalfWidth[1] * absRotationMatrix[0][0];
	radiusB = a_pOther->m_v3HalfWidth[1] * absRotationMatrix[2][2] + a_pOther->m_v3HalfWidth[2] * absRotationMatrix[2][1];
	if (glm::abs(translation[1] * rotationMatrix[0][0] - translation[0] * rotationMatrix[1][0]) > radiusA + radiusB)
	{
		return BTXs::eSATResults::SAT_AZxBX;
	}

	// Test axis AZxBY
	radiusA = m_v3HalfWidth[0] * absRotationMatrix[1][1] + m_v3HalfWidth[1] * absRotationMatrix[0][1];
	radiusB = a_pOther->m_v3HalfWidth[0] * absRotationMatrix[2][2] + a_pOther->m_v3HalfWidth[2] * absRotationMatrix[2][0];
	if (glm::abs(translation[1] * rotationMatrix[0][1] - translation[0] * rotationMatrix[1][1]) > radiusA + radiusB)
	{
		return BTXs::eSATResults::SAT_AZxBY;
	}

	// Test axis AZxBZ
	radiusA = m_v3HalfWidth[0] * absRotationMatrix[1][2] + m_v3HalfWidth[1] * absRotationMatrix[0][2];
	radiusB = a_pOther->m_v3HalfWidth[0] * absRotationMatrix[2][1] + a_pOther->m_v3HalfWidth[1] * absRotationMatrix[2][0];
	if (glm::abs(translation[1] * rotationMatrix[0][2] - translation[0] * rotationMatrix[1][2]) > radiusA + radiusB)
	{
		return BTXs::eSATResults::SAT_AZxBZ;
	}

	// No axis test separates the objects, so they are intersecting
	return BTXs::eSATResults::SAT_NONE;
}
bool MyRigidBody::IsColliding(MyRigidBody* const a_pOther)
{
	//check if spheres are colliding
	bool bColliding = true;
	/*
	* We use Bounding Spheres or ARBB as a pre-test to avoid expensive calculations (SAT)
	* we default bColliding to true here to always fall in the need of calculating
	* SAT for the sake of the assignment.
	*/
	if (bColliding) //they are colliding with bounding sphere
	{
		uint nResult = SAT(a_pOther);

		if (nResult == BTXs::eSATResults::SAT_NONE) //The SAT shown they are colliding
		{
			this->AddCollisionWith(a_pOther);
			a_pOther->AddCollisionWith(this);
		}
		else //they are not colliding
		{
			// Plane colors
			std::vector<vector3> planeColors = {
				vector3(1, 0, 0), vector3(0, 1, 0), vector3(0, 0, 1),
				vector3(1, 1, 0), vector3(1, 0, 1), vector3(0, 1, 1),
				vector3(0.5, 0, 0), vector3(0, 0.5, 0), vector3(0, 0, 0.5),
				vector3(0.5, 0.5, 0), vector3(0.5, 0, 0.5), vector3(0, 0.5, 0.5),
				vector3(0.3, 0.5, 0.2), vector3(0.5, 0.2, 0.3), vector3(0.2, 0.3, 0.5)
			};

			matrix4 mat4;
			vector3 color = planeColors[nResult - 1]; // result: 1~15 => index: 0~14

			// Compute the mat4 based on the specific case of nResult.
			vector3 planePosition;
			vector3 planeNormal;
			float planeSize = 5.0f; // Adjust the plane size based on your requirements.

			matrix4 m4This = GetModelMatrix();
			matrix4 m4Other = a_pOther->GetModelMatrix();

			vector3 v3ThisX = vector3(m4This[0]);
			vector3 v3ThisY = vector3(m4This[1]);
			vector3 v3ThisZ = vector3(m4This[2]);

			vector3 v3OtherX = vector3(m4Other[0]);
			vector3 v3OtherY = vector3(m4Other[1]);
			vector3 v3OtherZ = vector3(m4Other[2]);

			// Compute the plane normal and position based on the separating axis (nResult).
			switch (nResult)
			{
			case BTXs::eSATResults::SAT_AX:
				planeNormal = v3ThisX;
				break;
			case BTXs::eSATResults::SAT_AY:
				planeNormal = v3ThisY;
				break;
			case BTXs::eSATResults::SAT_AZ:
				planeNormal = v3ThisZ;
				break;
			case BTXs::eSATResults::SAT_BX:
				planeNormal = v3OtherX;
				break;
			case BTXs::eSATResults::SAT_BY:
				planeNormal = v3OtherY;
				break;
			case BTXs::eSATResults::SAT_BZ:
				planeNormal = v3OtherZ;
				break;
			case BTXs::eSATResults::SAT_AXxBX:
				planeNormal = glm::cross(v3ThisX, v3OtherX);
				break;
			case BTXs::eSATResults::SAT_AXxBY:
				planeNormal = glm::cross(v3ThisX, v3OtherY);
				break;
			case BTXs::eSATResults::SAT_AXxBZ:
				planeNormal = glm::cross(v3ThisX, v3OtherZ);
				break;
			case BTXs::eSATResults::SAT_AYxBX:
				planeNormal = glm::cross(v3ThisY, v3OtherX);
				break;
			case BTXs::eSATResults::SAT_AYxBY:
				planeNormal = glm::cross(v3ThisY, v3OtherY);
				break;
			case BTXs::eSATResults::SAT_AYxBZ:
				planeNormal = glm::cross(v3ThisY, v3OtherZ);
				break;
			case BTXs::eSATResults::SAT_AZxBX:
				planeNormal = glm::cross(v3ThisZ, v3OtherX);
				break;
			case BTXs::eSATResults::SAT_AZxBY:
				planeNormal = glm::cross(v3ThisZ, v3OtherY);
				break;
			case BTXs::eSATResults::SAT_AZxBZ:
				planeNormal = glm::cross(v3ThisZ, v3OtherZ);
				break;
			default:
				planeNormal = vector3(0, 0, 1);
				break;
			}

			// Compute the average position between the two objects.
			planePosition = (GetCenterGlobal() + a_pOther->GetCenterGlobal()) * 0.5f;
			// Compute the mat4 transformation matrix.
			quaternion qRotation = glm::rotation(vector3(0, 0, 1), planeNormal);
			mat4 = glm::translate(planePosition) * glm::mat4_cast(qRotation) * glm::scale(vector3(planeSize));
			// Render the plane
			m_pModelMngr->AddPlaneToRenderList(mat4, color);

			// Render a copy of the plane with the opposite normal so we can see it on both sides
			qRotation = glm::rotation(vector3(0, 0, 1), -planeNormal);
			mat4 = glm::translate(planePosition) * glm::mat4_cast(qRotation) * glm::scale(vector3(planeSize));
			m_pModelMngr->AddPlaneToRenderList(mat4, color);

			this->RemoveCollisionWith(a_pOther);
			a_pOther->RemoveCollisionWith(this);
		}
	}
	else //they are not colliding with bounding sphere
	{
		this->RemoveCollisionWith(a_pOther);
		a_pOther->RemoveCollisionWith(this);
	}
	return bColliding;
}
void MyRigidBody::Init(void)
{
	m_pModelMngr = ModelManager::GetInstance();
	m_bVisibleBS = false;
	m_bVisibleOBB = true;
	m_bVisibleARBB = false;

	m_fRadius = 0.0f;

	m_v3ColorColliding = C_RED;
	m_v3ColorNotColliding = C_WHITE;

	m_v3Center = ZERO_V3;
	m_v3MinL = ZERO_V3;
	m_v3MaxL = ZERO_V3;

	m_v3MinG = ZERO_V3;
	m_v3MaxG = ZERO_V3;

	m_v3HalfWidth = ZERO_V3;
	m_v3ARBBSize = ZERO_V3;

	m_m4ToWorld = IDENTITY_M4;
}
void MyRigidBody::Swap(MyRigidBody& a_pOther)
{
	std::swap(m_pModelMngr, a_pOther.m_pModelMngr);
	std::swap(m_bVisibleBS, a_pOther.m_bVisibleBS);
	std::swap(m_bVisibleOBB, a_pOther.m_bVisibleOBB);
	std::swap(m_bVisibleARBB, a_pOther.m_bVisibleARBB);

	std::swap(m_fRadius, a_pOther.m_fRadius);

	std::swap(m_v3ColorColliding, a_pOther.m_v3ColorColliding);
	std::swap(m_v3ColorNotColliding, a_pOther.m_v3ColorNotColliding);

	std::swap(m_v3Center, a_pOther.m_v3Center);
	std::swap(m_v3MinL, a_pOther.m_v3MinL);
	std::swap(m_v3MaxL, a_pOther.m_v3MaxL);

	std::swap(m_v3MinG, a_pOther.m_v3MinG);
	std::swap(m_v3MaxG, a_pOther.m_v3MaxG);

	std::swap(m_v3HalfWidth, a_pOther.m_v3HalfWidth);
	std::swap(m_v3ARBBSize, a_pOther.m_v3ARBBSize);

	std::swap(m_m4ToWorld, a_pOther.m_m4ToWorld);

	std::swap(m_CollidingRBSet, a_pOther.m_CollidingRBSet);
}
void MyRigidBody::Release(void)
{
	m_pModelMngr = nullptr;
	ClearCollidingList();
}
//Accessors
bool MyRigidBody::GetVisibleBS(void) { return m_bVisibleBS; }
void MyRigidBody::SetVisibleBS(bool a_bVisible) { m_bVisibleBS = a_bVisible; }
bool MyRigidBody::GetVisibleOBB(void) { return m_bVisibleOBB; }
void MyRigidBody::SetVisibleOBB(bool a_bVisible) { m_bVisibleOBB = a_bVisible; }
bool MyRigidBody::GetVisibleARBB(void) { return m_bVisibleARBB; }
void MyRigidBody::SetVisibleARBB(bool a_bVisible) { m_bVisibleARBB = a_bVisible; }
float MyRigidBody::GetRadius(void) { return m_fRadius; }
vector3 MyRigidBody::GetColorColliding(void) { return m_v3ColorColliding; }
vector3 MyRigidBody::GetColorNotColliding(void) { return m_v3ColorNotColliding; }
void MyRigidBody::SetColorColliding(vector3 a_v3Color) { m_v3ColorColliding = a_v3Color; }
void MyRigidBody::SetColorNotColliding(vector3 a_v3Color) { m_v3ColorNotColliding = a_v3Color; }
vector3 MyRigidBody::GetCenterLocal(void) { return m_v3Center; }
vector3 MyRigidBody::GetMinLocal(void) { return m_v3MinL; }
vector3 MyRigidBody::GetMaxLocal(void) { return m_v3MaxL; }
vector3 MyRigidBody::GetCenterGlobal(void) { return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
vector3 MyRigidBody::GetMinGlobal(void) { return m_v3MinG; }
vector3 MyRigidBody::GetMaxGlobal(void) { return m_v3MaxG; }
vector3 MyRigidBody::GetHalfWidth(void) { return m_v3HalfWidth; }
matrix4 MyRigidBody::GetModelMatrix(void) { return m_m4ToWorld; }
void MyRigidBody::SetModelMatrix(matrix4 a_m4ModelMatrix)
{
	//to save some calculations if the model matrix is the same there is nothing to do here
	if (a_m4ModelMatrix == m_m4ToWorld)
		return;

	//Assign the model matrix
	m_m4ToWorld = a_m4ModelMatrix;

	//Calculate the 8 corners of the cube
	vector3 v3Corner[8];
	//Back square
	v3Corner[0] = m_v3MinL;
	v3Corner[1] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MinL.z);
	v3Corner[2] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MinL.z);
	v3Corner[3] = vector3(m_v3MaxL.x, m_v3MaxL.y, m_v3MinL.z);

	//Front square
	v3Corner[4] = vector3(m_v3MinL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[5] = vector3(m_v3MaxL.x, m_v3MinL.y, m_v3MaxL.z);
	v3Corner[6] = vector3(m_v3MinL.x, m_v3MaxL.y, m_v3MaxL.z);
	v3Corner[7] = m_v3MaxL;

	//Place them in world space
	for (uint uIndex = 0; uIndex < 8; ++uIndex)
	{
		v3Corner[uIndex] = vector3(m_m4ToWorld * vector4(v3Corner[uIndex], 1.0f));
	}

	//Identify the max and min as the first corner
	m_v3MaxG = m_v3MinG = v3Corner[0];

	//get the new max and min for the global box
	for (uint i = 1; i < 8; ++i)
	{
		if (m_v3MaxG.x < v3Corner[i].x) m_v3MaxG.x = v3Corner[i].x;
		else if (m_v3MinG.x > v3Corner[i].x) m_v3MinG.x = v3Corner[i].x;

		if (m_v3MaxG.y < v3Corner[i].y) m_v3MaxG.y = v3Corner[i].y;
		else if (m_v3MinG.y > v3Corner[i].y) m_v3MinG.y = v3Corner[i].y;

		if (m_v3MaxG.z < v3Corner[i].z) m_v3MaxG.z = v3Corner[i].z;
		else if (m_v3MinG.z > v3Corner[i].z) m_v3MinG.z = v3Corner[i].z;
	}

	//we calculate the distance between min and max vectors
	m_v3ARBBSize = m_v3MaxG - m_v3MinG;
}
//The big 3
MyRigidBody::MyRigidBody(std::vector<vector3> a_pointList)
{
	Init();
	//Count the points of the incoming list
	uint uVertexCount = a_pointList.size();

	//If there are none just return, we have no information to create the BS from
	if (uVertexCount == 0)
		return;

	//Max and min as the first vector of the list
	m_v3MaxL = m_v3MinL = a_pointList[0];

	//Get the max and min out of the list
	for (uint i = 1; i < uVertexCount; ++i)
	{
		if (m_v3MaxL.x < a_pointList[i].x) m_v3MaxL.x = a_pointList[i].x;
		else if (m_v3MinL.x > a_pointList[i].x) m_v3MinL.x = a_pointList[i].x;

		if (m_v3MaxL.y < a_pointList[i].y) m_v3MaxL.y = a_pointList[i].y;
		else if (m_v3MinL.y > a_pointList[i].y) m_v3MinL.y = a_pointList[i].y;

		if (m_v3MaxL.z < a_pointList[i].z) m_v3MaxL.z = a_pointList[i].z;
		else if (m_v3MinL.z > a_pointList[i].z) m_v3MinL.z = a_pointList[i].z;
	}

	//with model matrix being the identity, local and global are the same
	m_v3MinG = m_v3MinL;
	m_v3MaxG = m_v3MaxL;

	//with the max and the min we calculate the center
	m_v3Center = (m_v3MaxL + m_v3MinL) / 2.0f;

	//we calculate the distance between min and max vectors
	m_v3HalfWidth = (m_v3MaxL - m_v3MinL) / 2.0f;

	//Get the distance between the center and either the min or the max
	m_fRadius = glm::distance(m_v3Center, m_v3MinL);
}
MyRigidBody::MyRigidBody(MyRigidBody const& a_pOther)
{
	m_pModelMngr = a_pOther.m_pModelMngr;

	m_bVisibleBS = a_pOther.m_bVisibleBS;
	m_bVisibleOBB = a_pOther.m_bVisibleOBB;
	m_bVisibleARBB = a_pOther.m_bVisibleARBB;

	m_fRadius = a_pOther.m_fRadius;

	m_v3ColorColliding = a_pOther.m_v3ColorColliding;
	m_v3ColorNotColliding = a_pOther.m_v3ColorNotColliding;

	m_v3Center = a_pOther.m_v3Center;
	m_v3MinL = a_pOther.m_v3MinL;
	m_v3MaxL = a_pOther.m_v3MaxL;

	m_v3MinG = a_pOther.m_v3MinG;
	m_v3MaxG = a_pOther.m_v3MaxG;

	m_v3HalfWidth = a_pOther.m_v3HalfWidth;
	m_v3ARBBSize = a_pOther.m_v3ARBBSize;

	m_m4ToWorld = a_pOther.m_m4ToWorld;

	m_CollidingRBSet = a_pOther.m_CollidingRBSet;
}
MyRigidBody& MyRigidBody::operator=(MyRigidBody const& a_pOther)
{
	if (this != &a_pOther)
	{
		Release();
		Init();
		MyRigidBody temp(a_pOther);
		Swap(temp);
	}
	return *this;
}
MyRigidBody::~MyRigidBody() { Release(); };

//--- a_pOther Methods
void MyRigidBody::AddCollisionWith(MyRigidBody* a_pOther)
{
	/*
		check if the object is already in the colliding set, if
		the object is already there return with no changes
	*/
	auto element = m_CollidingRBSet.find(a_pOther);
	if (element != m_CollidingRBSet.end())
		return;
	// we couldn't find the object so add it
	m_CollidingRBSet.insert(a_pOther);
}
void MyRigidBody::RemoveCollisionWith(MyRigidBody* a_pOther)
{
	m_CollidingRBSet.erase(a_pOther);
}
void MyRigidBody::ClearCollidingList(void)
{
	m_CollidingRBSet.clear();
}

void MyRigidBody::AddToRenderList(void)
{
	if (m_bVisibleBS)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
		else
			m_pModelMngr->AddWireSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(vector3(m_fRadius)), C_BLUE_CORNFLOWER);
	}
	if (m_bVisibleOBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorColliding);
		else
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) * glm::scale(m_v3HalfWidth * 2.0f), m_v3ColorNotColliding);
	}
	if (m_bVisibleARBB)
	{
		if (m_CollidingRBSet.size() > 0)
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
		else
			m_pModelMngr->AddWireCubeToRenderList(glm::translate(GetCenterGlobal()) * glm::scale(m_v3ARBBSize), C_YELLOW);
	}
}