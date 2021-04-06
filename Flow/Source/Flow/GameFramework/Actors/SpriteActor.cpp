#include "pch.h"
#include "SpriteActor.h"
#include "Flow/GameFramework/Components/SpriteComponent.h"

SpriteActor::SpriteActor()
	: SpriteActor("Sprite Actor")
{
}

SpriteActor::SpriteActor(const std::string& NewName)
	: Actor(NewName)
{
	m_SpriteComponent = CreateComponent<SpriteComponent>("Sprite Component");
	m_SpriteComponent->SetOwned(true);
	m_RootComponent = m_SpriteComponent;
}

SpriteActor::~SpriteActor()
{
	delete m_SpriteComponent;
}

SpriteComponent* SpriteActor::GetSpriteComponent() const
{
	return m_SpriteComponent;
}