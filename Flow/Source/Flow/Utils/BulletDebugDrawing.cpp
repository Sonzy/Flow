#include "Flowpch.h"
#include "BulletDebugDrawing.h"
#include "Flow\Rendering\Core\DebugDrawing\LineBatcher.h"
#include "Flow\GameFramework\World.h"

void BulletDebugDraw::Init()
{
}

void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	World::GetLineBatcher_S().AddLine(Vector3(from.x(), from.y(), from.z()), Vector3(to.x(), to.y(), to.z()), Vector3(color.x(), color.y(), color.z()));
}
void BulletDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}
void BulletDebugDraw::reportErrorWarning(const char* warningString)
{
	FLOW_ENGINE_WARNING(warningString);
}
void BulletDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
}
void BulletDebugDraw::setDebugMode(int debugMode)
{
	_DebugMode |= debugMode;
}
int BulletDebugDraw::getDebugMode() const
{
	return _DebugMode;
}

btIDebugDraw::DefaultColors BulletDebugDraw::getDefaultColors() const
{
	return _DebugColours;
}

void BulletDebugDraw::SetDebugColour(BulletDebugColour Option, Vector3 Colour)
{
	switch (Option)
	{
	case BulletDebugColour::Active:
		_DebugColours.m_activeObject = btVector3(Colour.x, Colour.y, Colour.z);
		break;
	case BulletDebugColour::Deactivated:
		_DebugColours.m_activeObject = btVector3(Colour.x, Colour.y, Colour.z);
		break;
	case BulletDebugColour::WantsDeactivation:
		_DebugColours.m_activeObject = btVector3(Colour.x, Colour.y, Colour.z);
		break;
	case BulletDebugColour::DisabledDeactivation:
		_DebugColours.m_activeObject = btVector3(Colour.x, Colour.y, Colour.z);
		break;
	case BulletDebugColour::DisabledSimulation:
		_DebugColours.m_activeObject = btVector3(Colour.x, Colour.y, Colour.z);
		break;
	case BulletDebugColour::AABB:
		_DebugColours.m_activeObject = btVector3(Colour.x, Colour.y, Colour.z);
		break;
	case BulletDebugColour::ContactPoint:
		_DebugColours.m_activeObject = btVector3(Colour.x, Colour.y, Colour.z);
		break;
	default:
		FLOW_ENGINE_ERROR("BulletDebugDraw::SetDebugColour: Case Error");
		break;
	}
}

btIDebugDraw::DefaultColors& BulletDebugDraw::GetDebugColours()
{
	return _DebugColours;
}
