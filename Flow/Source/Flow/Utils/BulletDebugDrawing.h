#pragma once
#include "ThirdParty\Bullet\LinearMath\btIDebugDraw.h"
#include "Flow/Rendering/Core/Bindables/IndexBuffer.h"

#include "Flow\Rendering\Core\Line.h"


//m_activeObject(1, 1, 1),
//m_deactivatedObject(0, 1, 0),
//m_wantsDeactivationObject(0, 1, 1),
//m_disabledDeactivationObject(1, 0, 0),
//m_disabledSimulationObject(1, 1, 0),
//m_aabb(1, 0, 0),
//m_contactPoint(1, 1, 0)

enum class BulletDebugColour
{
	Active,
	Deactivated,
	WantsDeactivation,
	DisabledDeactivation,
	DisabledSimulation,
	AABB,
	ContactPoint
};

class BulletDebugDraw : public btIDebugDraw
{
public:

	void Init();

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;

	virtual void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	virtual void reportErrorWarning(const char* warningString);

	virtual void draw3dText(const btVector3& location, const char* textString);

	virtual void setDebugMode(int debugMode);

	virtual int getDebugMode() const;

	virtual btIDebugDraw::DefaultColors getDefaultColors() const override;
	void SetDebugColour(BulletDebugColour Option, Vector3 Colour);
	btIDebugDraw::DefaultColors& GetDebugColours();

	int _DebugMode;


	DefaultColors _DebugColours;
};