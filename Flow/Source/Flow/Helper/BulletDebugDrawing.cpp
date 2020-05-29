#include "Flowpch.h"
#include "BulletDebugDrawing.h"
#include "Flow\Rendering\Core\DebugDrawing\LineBatcher.h"
#include "Flow\GameFramework\World.h"

void BulletDebugDraw::Init()
{
}

void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	World::GetLineBatcher_S().AddLine(Vector(from.x(), from.y(), from.z()), Vector(to.x(), to.y(), to.z()), Vector(color.x(), color.y(), color.z()));
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