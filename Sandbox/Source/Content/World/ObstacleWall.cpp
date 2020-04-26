#include "ObstacleWall.h"

#include "Flow\GameFramework\Components\StaticMeshComponent.h"

#include "Flow\Assets\AssetSystem.h"
#include "Flow\Assets\Materials\MaterialAsset.h"
#include "Flow\Assets\Meshes\MeshAsset.h"
#include "Flow\Rendering\Core\Materials\Material.h"

ObstacleWall::ObstacleWall()
	: ObstacleWall("ObstacleWall")
{

}

ObstacleWall::ObstacleWall(const std::string& Name)
	: WorldObject(Name)
{
	//CreateWall(WallLayout(100, 100, 100, 4));

	WallLayoutContainer Layout1 = WallLayoutContainer(40.0f, 50.0f);
	Layout1.AddCube(WallLayout2(Vector(-200.0f, 0.0f, 0.0f), Vector(50.0f, 20.0f, 20.0f)));
	Layout1.AddCube(WallLayout2(Vector(-100.0f, 0.0f, 0.0f), Vector(50.0f, 20.0f, 20.0f)));
	Layout1.AddCube(WallLayout2(Vector(0.0f, 0.0f, 0.0f), Vector(50.0f, 20.0f, 20.0f)));
	Layout1.AddCube(WallLayout2(Vector(100.0f, 0.0f, 0.0f), Vector(50.0f, 20.0f, 20.0f)));
	Layout1.AddCube(WallLayout2(Vector(200.0f, 0.0f, 0.0f), Vector(50.0f, 20.0f, 20.0f)));

	WallLayoutContainer Layout2 = WallLayoutContainer(30.0f);
	Layout2.AddCube(WallLayout2(Vector(-170.0f, 0.0f, 0.0f), Vector(100.0f, 20.0f, 20.0f)));
	Layout2.AddCube(WallLayout2(Vector(-50.0f, 0.0f, 0.0f), Vector(100.0f, 20.0f, 20.0f)));
	Layout2.AddCube(WallLayout2(Vector(20.0f, 0.0f, 0.0f), Vector(30.0f, 20.0f, 20.0f)));
	Layout2.AddCube(WallLayout2(Vector(120.0f, 0.0f, 0.0f), Vector(50.0f, 20.0f, 20.0f)));

	WallLayoutContainer Layout3 = WallLayoutContainer(600.0f);
	Layout3.AddCube(WallLayout2(Vector(-170.0f, 0.0f, 0.0f), Vector(100.0f, 20.0f, 20.0f)));
	Layout3.AddCube(WallLayout2(Vector(-50.0f, 0.0f, 0.0f), Vector(100.0f, 20.0f, 20.0f)));
	Layout3.AddCube(WallLayout2(Vector(20.0f, 0.0f, 0.0f), Vector(30.0f, 20.0f, 20.0f)));
	Layout3.AddCube(WallLayout2(Vector(120.0f, 0.0f, 0.0f), Vector(50.0f, 20.0f, 20.0f)));

	WallLayoutContainer Layout4 = WallLayoutContainer(100.0f);
	Layout4.AddCube(WallLayout2(Vector(-170.0f, 0.0f, 0.0f), Vector(100.0f, 20.0f, 20.0f)));
	Layout4.AddCube(WallLayout2(Vector(-50.0f, 0.0f, 0.0f), Vector(100.0f, 20.0f, 20.0f)));
	Layout4.AddCube(WallLayout2(Vector(20.0f, 0.0f, 0.0f), Vector(30.0f, 20.0f, 20.0f)));
	Layout4.AddCube(WallLayout2(Vector(120.0f, 0.0f, 0.0f), Vector(50.0f, 20.0f, 20.0f)));

	Layouts_.push_back(Layout1);
	Layouts_.push_back(Layout2);
	Layouts_.push_back(Layout3);
	Layouts_.push_back(Layout4);

	std::random_device RD_;
	Random_ = std::mt19937(RD_());
	Distribution_ = std::uniform_int_distribution<int>(0, Layouts_.size() - 1);
}

void ObstacleWall::Initialise(Vector StartPosition)
{
	CreateWall();
	RootComponent_->SetRelativePosition(StartPosition);
}


void ObstacleWall::CreateWall(WallLayout NewLayout)
{
	int HorizontalBoxNum = NewLayout.HorizontalGaps_ + 1;
	int HalfBoxes = trunc(HorizontalBoxNum / 2);
	float Gap = (NewLayout.CubeWidth_ + NewLayout.GapWidth_) / 2;
	float InitialOffset = (((HorizontalBoxNum % 2 == 0) ? -(Gap / 2) : 0) + (Gap * HalfBoxes));

	auto BoxMesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Box");
	auto WallMaterial = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_LitGrey")->GetMaterial();

	for (int i = 0; i < HorizontalBoxNum; i++)
	{
		std::shared_ptr<Flow::StaticMeshComponent> NewCube = CreateComponent<Flow::StaticMeshComponent>("Box");
		NewCube->SetMeshAndMaterial(BoxMesh, WallMaterial);
		NewCube->SetRelativeScale(Vector(NewLayout.CubeWidth_ / 2, 20.0f, 20.0f));
		NewCube->SetRelativePosition(Vector(-InitialOffset + (i * Gap), 20.0f, 0.0f));
		NewCube->DisableGravity();
		NewCube->UseCompoundCollision_ = true;
		RootComponent_->AddChild(NewCube.get());
		Boxes_.push_back(NewCube);
	}
}

void ObstacleWall::CreateWall(WallLayoutContainer NewLayout)
{
	auto BoxMesh = Flow::AssetSystem::GetAsset<Flow::MeshAsset>("Box");
	//auto WallMaterial = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_LitGrey")->GetMaterial();
	auto WallMaterial = Flow::AssetSystem::GetAsset<Flow::MaterialAsset>("Mat_Wood")->GetMaterial();

	if (!EmptyRoot_)
	{
		EmptyRoot_ = CreateComponent<Flow::WorldComponent>("Root");
		RootComponent_ = EmptyRoot_.get();
	}

	for (int i = 0; i < NewLayout.Cubes_.size(); i++)
	{
		std::shared_ptr<Flow::StaticMeshComponent> NewCube;
		NewCube = i >= (Boxes_.size()) ?
			CreateComponent<Flow::StaticMeshComponent>("Box") :
			Boxes_[i];

		RootComponent_->AddChild(NewCube.get());
		NewCube->DisableGravity();
		NewCube->UseCompoundCollision_ = true;
		NewCube->BoxCollision = true;

		if(!NewCube->GetMesh())
			NewCube->SetMeshAndMaterial(BoxMesh, WallMaterial);

		NewCube->SetRelativeScale(Vector(NewLayout.Cubes_[i].Scale_.X, NewLayout.Height_, NewLayout.Cubes_[i].Scale_.Z));
		NewCube->SetRelativePosition(Vector(NewLayout.Cubes_[i].Position_.X, 0.0f, 0.0f));

		Boxes_.push_back(NewCube);
	}

	float Diff = NewLayout.Height_ / 2;

	RootComponent_->SetRelativePosition(Vector(0.0f, 50.0f, 0.0f));
	SetPhysicsMode(Flow::PhysicsMode::Kinematic);

	if (!Cube_Bottom_)
	{
		Cube_Bottom_ = CreateComponent<Flow::StaticMeshComponent>("Bottom");
		Cube_Bottom_->SetMeshAndMaterial(BoxMesh, WallMaterial);
		Cube_Bottom_->DisableGravity();
		RootComponent_->AddChild(Cube_Bottom_.get());
	}

	if (!Cube_Top_)
	{
		Cube_Top_ = CreateComponent<Flow::StaticMeshComponent>("Top");
		Cube_Top_->SetMeshAndMaterial(BoxMesh, WallMaterial);
		Cube_Top_->DisableGravity();
		RootComponent_->AddChild(Cube_Top_.get());
	}
		

	Cube_Bottom_->SetRelativePosition(Vector(0.0f, Diff, 0.0f));
	Cube_Top_->SetRelativePosition(Vector(0.0f, -Diff, 0.0f));

	Cube_Bottom_->SetRelativeScale(Vector(500.0f, 20.0f, 20.0f));
	Cube_Top_->SetRelativeScale(Vector(500.0f, 20.0f, 20.0f));

	Cube_Top_->UseCompoundCollision_ = true;
	Cube_Bottom_->UseCompoundCollision_ = true;
	Cube_Top_->BoxCollision = true;
	Cube_Bottom_->BoxCollision = true;
}

void ObstacleWall::CreateWall()
{
	CreateWall(Layouts_[Distribution_(Random_)]);
}
