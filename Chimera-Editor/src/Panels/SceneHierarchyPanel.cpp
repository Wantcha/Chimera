#include "SceneHierarchyPanel.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Chimera/Scene/Components.h"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}
	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};

		m_CurrentPath = fs::current_path();
		m_CurrentPath /= "assets";
		m_IsCurrentPathDir = true;

		m_FilesInScope = ImGuiFilepath::GetFilesInPath(m_CurrentPath, m_CurrentPath != m_RootPath);
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, m_Context.get() };
				DrawEntityNode(entity);
			}
		);

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::BeginMenu("Create..."))
			{
				if (ImGui::MenuItem("Empty Entity"))
				{
					m_Context->CreateEntity("Empty Entity");
				}

				else if (ImGui::MenuItem("Sprite"))
				{
					Entity entity = { m_Context->CreateEntity("Sprite"), m_Context.get() };
					entity.AddComponent<SpriteRendererComponent>();

				}

				else if (ImGui::MenuItem("Camera"))
				{
					Entity entity = { m_Context->CreateEntity("Camera"), m_Context.get() };
					entity.AddComponent<CameraComponent>();

				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			//if (m_SelectionContext.HasComponent<BoxCollider2DComponent>())
			m_Context->m_EditColliders = false;
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			/*if (entity.HasComponent<Body2DComponent>())
			{
				Entity* e = reinterpret_cast<Entity*>(entity.GetComponent<Body2DComponent>().Body->GetUserData().pointer);
				delete e;
				m_Context->m_World->DestroyBody(entity.GetComponent<Body2DComponent>().Body);
			}*/
				

			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
			
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float defaultValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = defaultValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.25f, 0.0f, 0.0f, "%.3f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = defaultValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.25f, 0.0f, 0.0f, "%.3f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = defaultValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.25f, 0.0f, 0.0f, "%.3f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed
			| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();

			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
			if (removeComponent)
				entity.RemoveComponent<T>();
			//ImGui::Separator();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			bool& enabled = entity.GetComponent<TagComponent>().Enabled;

			ImGui::Checkbox("##Enabled", &enabled);
			ImGui::SameLine();

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());

			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		} 

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add..."))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_SelectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_SelectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Box Collider 2D"))
			{
				m_SelectionContext.AddComponent<BoxCollider2DComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Circle Collider 2D"))
			{
				m_SelectionContext.AddComponent<CircleCollider2DComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Rigid Body 2D"))
			{
				m_SelectionContext.AddComponent<RigidBody2DComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();
		//ImGui::Separator();

		DrawComponent<TransformComponent>("Transform", entity, [&](auto& component)
		{	
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Position", component.Position);

				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
				
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection Type", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float fov = glm::degrees(camera.GetFOV());
					if (ImGui::DragFloat("FOV", &fov))
						camera.SetFOV(glm::radians(fov));

					float nearClip = camera.GetPerspNearClip();
					if (ImGui::DragFloat("Near", &nearClip))
						camera.SetPerspNearClip(nearClip);

					float farClip = camera.GetPerspFarClip();
					if (ImGui::DragFloat("Far", &farClip))
						camera.SetPerspFarClip(farClip);
				}

				else
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float nearClip = camera.GetOrthoNearClip();
					if (ImGui::DragFloat("Near", &nearClip, 0.2f))
						camera.SetOrthoNearClip(nearClip);

					float farClip = camera.GetOrthoFarClip();
					if (ImGui::DragFloat("Far", &farClip, 0.2f))
						camera.SetOrthoFarClip(farClip);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [&](auto& component)
		{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::Text("Texture");
				ImGui::SameLine();

				Ref<Texture2D>& tex = component.SpriteTexture;
				std::string outPath = "";
				ImGui::Image(reinterpret_cast<void*>(tex->GetRendererID()), ImVec2{ 30, 30 }, ImVec2{ 0,1 }, ImVec2{ 1,0 },
					{ component.Color.x, component.Color.y, component.Color.z, component.Color.w });
				ImGuiWindowFlags modalFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
					ImGuiWindowFlags_AlwaysAutoResize;

				bool isOpen = true;
				if (ImGui::IsItemClicked())
				{
					ImGui::OpenPopup("SelectAsset");
				}
				if (ImGui::BeginPopupModal("SelectAsset", &isOpen, modalFlags))
				{
					//Auto resize text wrap to popup width.
					ImGui::Spacing();
					ImGui::PushItemWidth(-1);
					ImGui::TextWrapped(m_CurrentPath.string().data());
					ImGui::PopItemWidth();

					//ImVec2 size = ImGui::GetContentRegionAvail();

					ImGui::SameLine();

					// Make the "Select" button look / act disabled if the current selection is a directory.
					if (m_IsCurrentPathDir)
					{

						static const ImVec4 disabledColor = { 0.3f, 0.3f, 0.3f, 1.0f };

						ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);

						ImGui::Button("Select");

						ImGui::PopStyleColor();
						ImGui::PopStyleColor();
						ImGui::PopStyleColor();

					}
					else
					{

						if (ImGui::Button("Select"))
						{
							ImGui::CloseCurrentPopup();

							outPath = m_CurrentPath.string();
							component.SpriteTexture = Texture2D::Create(outPath);
						}

					}

					//ImGui::Text(std::to_string(size.y / (style.ItemSpacing.y + ImGui::GetFontSize() )).c_str()  );
					ImGui::PushItemWidth(-1);
					if (ImGui::ListBox("##", &m_SelectionPath, ImGuiFilepath::vector_file_items_getter, &m_FilesInScope, m_FilesInScope.size(), 5))
					{
						//Update current path to the selected list item.
						m_CurrentPath = m_FilesInScope[m_SelectionPath].Path;
						m_IsCurrentPathDir = fs::is_directory(m_CurrentPath);

						//If the selection is a directory, repopulate the list with the contents of that directory.
						if (m_IsCurrentPathDir) {
							m_FilesInScope = ImGuiFilepath::GetFilesInPath(m_CurrentPath, m_CurrentPath != m_RootPath);
						}

					}
					ImGui::PopItemWidth();

					ImGui::EndPopup();
				}
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [&](auto& component)
			{
				bool& edit = m_Context->m_EditColliders;

				ImGui::Checkbox("Edit Collider", &edit);

				BoxCollider2D& body = component.BoxCollider;

				if (edit)
				{
					float boxPos[2] = { body.GetCenter().x, body.GetCenter().y };
					if (ImGui::DragFloat2("Collider Center", boxPos, 0.05f))
					{
						body.SetCenter(boxPos[0], boxPos[1]);
					}

					float boxSize[2] = { body.GetWidth(), body.GetHeight() };
					if (ImGui::DragFloat2("Collider Size", boxSize, 0.05f))
					{
						body.SetSize(boxSize[0], boxSize[1]);
					}
				}

				bool sensor = body.IsSensor();
				if (ImGui::Checkbox("Is Sensor", &sensor))
				{
					body.SetSensor(sensor);
				}
				ImGui::Separator();

				float density = body.GetDensity();
				if (ImGui::DragFloat("Density", &density, 0.25f, 0.0f, 100000.0f))
				{
					body.SetDensity(density);
				}

				float friction = body.GetFriction();
				if (ImGui::DragFloat("Friction", &friction, 0.05f, 0.0f, 1.0f))
				{
					body.SetFriction(friction);
				}

				float bounciness = body.GetBounciness();
				if (ImGui::DragFloat("Bounciness", &bounciness, 0.05f, 0.0f, 1.0f))
				{
					body.SetBounciness(bounciness);
				}
			});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [&](auto& component)
			{
				bool& edit = m_Context->m_EditColliders;

				ImGui::Checkbox("Edit Collider", &edit);

				CircleCollider2D& body = component.CircleCollider;

				if (edit)
				{
					float circlePos[2] = { body.GetCenter().x, body.GetCenter().y };
					if (ImGui::DragFloat2("Center", circlePos, 0.05f))
					{
						body.SetCenter(circlePos[0], circlePos[1]);
					}

					float circleRadius = body.GetRadius();
					if (ImGui::DragFloat("Radius", &circleRadius, 0.05f))
					{
						body.SetRadius(circleRadius);
					}
				}

				bool sensor = body.IsSensor();
				if (ImGui::Checkbox("Is Sensor", &sensor))
				{
					body.SetSensor(sensor);
				}
				ImGui::Separator();

				float density = body.GetDensity();
				if (ImGui::DragFloat("Density", &density, 0.25f, 0.0f, 100000.0f))
				{
					body.SetDensity(density);
				}

				float friction = body.GetFriction();
				if (ImGui::DragFloat("Friction", &friction, 0.05f, 0.0f, 1.0f))
				{
					body.SetFriction(friction);
				}

				float bounciness = body.GetBounciness();
				if (ImGui::DragFloat("Bounciness", &bounciness, 0.05f, 0.0f, 1.0f))
				{
					body.SetBounciness(bounciness);
				}
			});

		DrawComponent<RigidBody2DComponent>("RigidBody 2D", entity, [](auto& component)
			{
				RigidBody2D& body = component.RigidBody;

				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)body.GetBodyType()];
				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							body.SetBodyType((RigidBody2D::Body2DType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (body.GetBodyType() != RigidBody2D::Body2DType::Static)
				{
					float gravityScale = body.GetGravityScale();
					if (ImGui::DragFloat("Gravity Scale", &gravityScale, 0.05f))
					{
						body.SetGravityScale(gravityScale);
					}

					bool discrete = body.IsDiscreteCollision();
					if (ImGui::Checkbox("Use Discrete Collision", &discrete))
						body.SetDiscreteCollision(discrete);

					bool fixed = body.IsFixedRotation();
					if (ImGui::Checkbox("Has Fixed Rotation", &fixed))
						body.SetFixedRotation(fixed);
				}
				
			});

		/*DrawComponent<Body2DComponent>("Body 2D Component", entity, [](auto& component)
			{
				
			});*/
	}
}