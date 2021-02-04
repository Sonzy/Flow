#include "Flowpch.h"
#include "SelectionTool.h"
#include "Editor/Editor.h"
#include "Input/Input.h"

#include "Rendering\RenderCommand.h"

#include "Physics/Physics.h"

#include "GameFramework/Components/WorldComponent.h"
#include "GameFramework/Components/StaticMeshComponent.h"
#include "GameFramework/World.h"

#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "GameFramework/Actor.h"

#include "Editor/UIComponents/Console.h"

#include "Rendering/Core/Camera/Camera.h"

SelectionTool::SelectionTool()
	: m_SelectedComponent(nullptr)
	, m_SpaceMode(ImGuizmo::WORLD)
{

}

SelectionTool::~SelectionTool()
{
}

void SelectionTool::UpdateTool(float DeltaTime)
{
	for (auto& line : m_PreviousLines)
	{
		World::GetLineBatcher_S().AddLine(line.first, line.second, m_DebugLineColor);
	}
}

void SelectionTool::RenderTool()
{
}

void SelectionTool::BeginPlay()
{
}

void SelectionTool::RenderImGuiGizmo()
{
	if (m_SelectedComponent != nullptr)
	{
		m_CurrentMatrix = m_SelectedComponent->GetTransformationMatrix();

		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(); //TODO: Put this inside ImGui?

		Editor& editor = Editor::Get();
		Vector2 rectPos = editor.GetSceneWindowPosition();
		Vector2 rectSize = editor.GetSceneWindowSize();
		ImGuizmo::SetRect(rectPos.x, rectPos.y, rectSize.x, rectSize.y);
	
		//= Convert view matrix to valid format

		DirectX::XMFLOAT4X4 fViewMatrix;
		DirectX::XMStoreFloat4x4(&fViewMatrix, RenderCommand::GetMainCamera()->GetViewMatrix());

		DirectX::XMFLOAT4X4 fProjectionMatrix;
		DirectX::XMStoreFloat4x4(&fProjectionMatrix, RenderCommand::GetMainCamera()->GetProjectionMatrix());

		ImGuizmo::Manipulate(
			reinterpret_cast<float*>(&fViewMatrix),
			reinterpret_cast<float*>(&fProjectionMatrix),
			TranslateTransformation(m_transformationMode),
			m_transformationMode != TransformMode::Scale ? m_SpaceMode : ImGuizmo::MODE::LOCAL, //Only use local scaling 
			reinterpret_cast<float*>(&m_CurrentMatrix)
		);

		if (ImGuizmo::IsUsing())
		{			
			DirectX::XMVECTOR quatRotation;
			DirectX::XMVECTOR position;
			DirectX::XMVECTOR scale;
			DirectX::XMMatrixDecompose(&scale, &quatRotation, &position, DirectX::XMLoadFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&m_CurrentMatrix)));

			DirectX::XMFLOAT3 pos, sca;
			DirectX::XMStoreFloat3(&pos, position);
			DirectX::XMStoreFloat3(&sca, scale);

			m_SelectedComponent->SetWorldPosition(pos);
			m_SelectedComponent->SetWorldRotation(Maths::QuaternionToEulers(quatRotation));
			m_SelectedComponent->SetWorldScale(sca);
		}
	}
}

bool SelectionTool::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() != MOUSE_LEFT)
		return false;

	if (m_SelectedComponent != nullptr && ImGuizmo::IsOver())
	{
		return true;
	}

	IntVector2 MousePosition = Input::GetMousePosition();

	HRESULT ResultHandle;
	D3D11_MAPPED_SUBRESOURCE MSR;

	const FrameBuffer* buf = RenderQueue::GetSelectionBuffer();

	{
		//Create the texture resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = buf->GetWidth();
		textureDesc.Height = buf->GetHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_STAGING;
		textureDesc.BindFlags = 0;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		textureDesc.MiscFlags = 0;


		CATCH_ERROR_DX(RenderCommand::DX11GetDevice()->CreateTexture2D(&textureDesc, nullptr, &pNewTexture));
	}

	RenderCommand::DX11GetContext()->CopyResource(pNewTexture.Get(), buf->GetTexture());


	CATCH_ERROR_DX(RenderCommand::DX11GetContext()->Map(
		pNewTexture.Get(), 0u, D3D11_MAP_READ, 0u, &MSR));

	MousePosition -= Editor::Get().GetSceneWindowPosition();
	int index = (MousePosition.x * 4) + (MousePosition.y * MSR.RowPitch);
	uint8* vals = reinterpret_cast<uint8*>(MSR.pData);
	uint8* pixel = &vals[index];
	uint32* pixelData = reinterpret_cast<uint32*>(pixel);

	//_B8G8R8A8
	uint32 A = *pixelData & 0xff000000;
	uint32 R = *pixelData & 0xff0000;
	uint32 G = *pixelData & 0xff00;
	uint32 B = *pixelData & 0xff;

	FGUID guid = 0;

	RenderCommand::DX11GetContext()->Unmap(pNewTexture.Get(), 0u);

	//debug
	recentcolorclicked = Vector4((float)(R >> 16) / 255.0f, (float)(G >> 8) / 255.0f, (float)(B / 255.0f), (float)(A >> 24) / 255.0f);

	guid = (R << 8) | (G << 8) | (B<< 8) | (A >> 24);

	// Debug
	//FLOW_ENGINE_LOG("Pixel: %lu, Guid: %lu B:%lu G:%lu R: %lu A:%lu", *pixelData, guid, B / 4, G / 4, R / 4, A / 4);
	//FLOW_ENGINE_LOG("R:%lu G:%lu B: %lu A:%lu", (R >> 16), (G >> 8), B, (A >> 24));
	//FLOW_ENGINE_LOG("R:%lu G:%lu B: %lu A:%lu", (R << 8), (G << 8), (B << 8), (A >> 24));
	//FLOW_ENGINE_LOG("Guid: %lu", guid);
	//FLOW_ENGINE_LOG("Clicked Color: %f %f %f %f", recentcolorclicked.x, recentcolorclicked.y, recentcolorclicked.z, recentcolorclicked.w);

	SelectComponent(World::Get().FindComponent<WorldComponent>(guid));
	return true;
}

bool SelectionTool::OnKeyPressed(KeyPressedEvent& e)
{
	if (e.GetKeyCode() == KEY_DELETE && m_SelectedActor)
	{
		//Deselect and delete component
		if (m_SelectedComponent)
		{
			m_SelectedComponent->OnViewportDeselected();
			m_SelectedComponent = nullptr;
		}

		//Deselect and delete actor
		if (m_SelectedActor != nullptr)
		{
			m_SelectedActor->OnViewportDeselected();
			World::Get().DestroyActor(m_SelectedActor->GetGuid());
			m_SelectedActor = nullptr;
		}

		return true;
	}

	if (e.GetKeyCode() == KEY_W)
	{
		m_transformationMode = TransformMode::Translate;
	}

	if (e.GetKeyCode() == KEY_E)
	{
		m_transformationMode = TransformMode::Rotate;
	}

	if (e.GetKeyCode() == KEY_R)
	{
		m_transformationMode = TransformMode::Scale;
	}

	return false;
}

bool SelectionTool::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	return false;
}

void SelectionTool::DrawConfigWindow()
{
	if (ImGui::Begin("Selection Tool Config", &m_ConfigurationWindowOpen))
	{
		if (ImGui::CollapsingHeader("Picking"))
		{
			ImGui::Image(RenderQueue::GetSelectionBuffer()->GetTextureView(), ImVec2(500, 500));
			ImGui::SameLine(); ImGui::ColorButton("clicked color", recentcolorclicked);
		}

		//Clear the stored lines on setting change
		if (ImGui::Checkbox("Draw debug lines in world from click", &m_DrawSelectionLines))
		{
			m_PreviousLines.clear();
		}

		if (ImGui::Button(m_SpaceMode == ImGuizmo::WORLD ? "World" : "Local"))
		{
			m_SpaceMode = (m_SpaceMode == ImGuizmo::WORLD) ? ImGuizmo::LOCAL : ImGuizmo::WORLD;
		}

		ImGui::ColorEdit3("Debug Line Color", reinterpret_cast<float*>(&m_DebugLineColor));//, ImGuiColorEditFlags_NoInputs);		
	}
	ImGui::End();
}

void SelectionTool::SelectComponent(WorldComponent* NewComponent)
{
	Actor* Parent = NewComponent != nullptr ? NewComponent->GetParentActor() : nullptr;

	//Pass on the onclicked event
	if (m_SelectedActor != Parent)
	{
		if (m_SelectedActor)
		{
			m_SelectedActor->OnViewportDeselected();
		}

		m_SelectedActor = Parent;

		if (m_SelectedActor)
		{
			m_SelectedActor->OnViewportSelected();
		}
	}

	//Pass on the on clicked event
	if (m_SelectedComponent != NewComponent)
	{
		if (m_SelectedComponent)
		{
			m_SelectedComponent->OnViewportDeselected();
		}

		m_SelectedComponent = NewComponent;

		if (m_SelectedComponent)
		{
			m_CurrentMatrix = m_SelectedComponent->GetTransformationMatrix();
			m_SelectedComponent->OnViewportSelected();
		}
	}

	//If we didnt hit anything, reset the gizmo
	if (m_SelectedActor == nullptr)
	{
		if (m_SelectedComponent)
		{
			m_SelectedComponent->OnViewportDeselected();
			m_SelectedComponent = nullptr;
		}
	}
}

ImGuizmo::OPERATION SelectionTool::TranslateTransformation(TransformMode mode)
{
	switch (mode)
	{
	case TransformMode::Translate:	return ImGuizmo::OPERATION::TRANSLATE;
	case TransformMode::Rotate:		return ImGuizmo::OPERATION::ROTATE;
	case TransformMode::Scale:		return ImGuizmo::OPERATION::SCALE;
	default:						return ImGuizmo::OPERATION::TRANSLATE;
	}
}
