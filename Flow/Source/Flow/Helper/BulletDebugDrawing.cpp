#include "Flowpch.h"
#include "BulletDebugDrawing.h"
#include "Flow\Rendering\Core\Line.h"

namespace Flow
{
	Line* BulletDebugDraw::Line_ = new Line();

	void BulletDebugDraw::Init()
	{
		Line_->Initialise();
	}

	void BulletDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		Line::DrawLine(Vector(from.x(), from.y(), from.z()), Vector(to.x(), to.y(), to.z()), Vector(color.x(), color.y(), color.z()));
	}
	void BulletDebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
	}
	void BulletDebugDraw::reportErrorWarning(const char* warningString)
	{
	}
	void BulletDebugDraw::draw3dText(const btVector3& location, const char* textString)
	{
	}
	void BulletDebugDraw::setDebugMode(int debugMode)
	{
		DebugMode |= debugMode;
	}
	int BulletDebugDraw::getDebugMode() const
	{
		return DebugMode;
	}

	Line* BulletDebugDraw::Get()
	{
		return Line_;
	}
}