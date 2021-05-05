#include "EditorLayer.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

#include "Chimera/Scene/SceneSerializer.h"
#include "Chimera/Utils/PlatformUtils.h"
#include "Chimera/Utils/Utils.h"
#include "ImGuizmo.h"
#include "Chimera/Math/Math.h"
#include "Project/ProjectManager.h"
#include "Project/ProjectSerializer.h"
#include "Chimera/Scripting/LuaManager.h"
//#include "Chimera/Physics/ContactListener2D.h"

namespace Chimera
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{

	}

	void EditorLayer::OnAttach()
	{
		CM_PROFILE_FUNCTION();

		//AssetManager::GetAsset<Texture2D>("assets/textures/laurgras.png");
		//m_LaurTexture = Texture2D::Create("assets/textures/laurgras.png");

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_ViewportFramebuffer = Framebuffer::Create(fbSpec);
		m_GameWindowFramebuffer = Framebuffer::Create(fbSpec);

		Ref<Scene> activeScene = SceneManager::Get().GetActiveScene();
		//m_ActiveScene = CreateRef<Scene>();

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

		auto scriptEntity = activeScene->CreateEntity("Unu");
		scriptEntity.AddComponent<SpriteRendererComponent>().Color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

		auto secondEntity = activeScene->CreateEntity("Doi");
		secondEntity.AddComponent<SpriteRendererComponent>();
		secondEntity.GetComponent<TransformComponent>().SetPosition(glm::vec3(1.0f, 1.5f, 0.0f));
		//secondEntity.GetComponent<TransformComponent>().SetParent(scriptEntity.GetComponent<TransformComponent>());

		auto thirdEntity = activeScene->CreateEntity("Trei");
		thirdEntity.AddComponent<SpriteRendererComponent>().Color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);;
		thirdEntity.GetComponent<TransformComponent>().SetPosition(glm::vec3(-2.5f, -0.5f, 0.0f));
		//thirdEntity.GetComponent<TransformComponent>().SetParent(secondEntity);


		//scriptEntity.AddComponent<CameraComponent>();
		//scriptEntity.AddComponent<LuaScriptComponent>("H:/Programming/Chimera/Chimera-Editor/assets/scripts/SampleScript.lua");

		/*auto square = m_ActiveScene->CreateEntity("Green Square");
		square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_ActiveScene->CreateEntity("Second Camera");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				//auto& position = GetComponent<TransformComponent>().Position;
				//position.x = rand() % 10- 5.0f;
			}
			void OnDestroy()
			{}
			void OnUpdate(Timestep ts)
			{
				auto& position = GetComponent<TransformComponent>().Position;
				float speed = 5.0f;

				if (Input::IsKeyPressed(Key::A))
					position.x -= speed * ts;

				if (Input::IsKeyPressed(Key::D))
					position.x += speed * ts;

				if (Input::IsKeyPressed(Key::W))
					position.y += speed * ts;

				if (Input::IsKeyPressed(Key::S))
					position.y -= speed * ts;
			}

			void OnCollisionEnter2D(Contact2D contact)
			{
				CM_CORE_WARN("COLLISION");
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();*/

		m_SceneHierarchyPanel.SetContext(activeScene);
	}

	void EditorLayer::OnDetach()
	{
		CM_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		CM_PROFILE_FUNCTION();

		Ref<Scene> activeScene = SceneManager::Get().GetActiveScene();
		if (FramebufferSpecification spec = m_ViewportFramebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_ViewportFramebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		if (FramebufferSpecification spec = m_GameWindowFramebuffer->GetSpecification();
			m_GameWindowSize.x > 0.0f && m_GameWindowSize.y > 0.0f &&
			(spec.Width != m_GameWindowSize.x || spec.Height != m_GameWindowSize.y))
		{
			m_GameWindowFramebuffer->Resize((uint32_t)m_GameWindowSize.x, (uint32_t)m_GameWindowSize.y);
			activeScene->OnViewportResize((uint32_t)m_GameWindowSize.x, (uint32_t)m_GameWindowSize.y);
		}
		Application::Get().SetGameWindowOffset(m_GameWindowBounds[0]);

		m_EditorCamera.OnUpdate(ts);

		if (m_IsPlayMode)
			activeScene->OnUpdateRuntime(ts);
		else
			activeScene->OnUpdateEditor(ts);

		// Render
		Renderer2D::ResetStats();
		m_ViewportFramebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		m_ViewportFramebuffer->ClearAttachment(1, -1);

		{
			CM_PROFILE_SCOPE("Renderer Draw");

			activeScene->OnRenderEditor(m_EditorCamera);
		}
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my; // Make screen space (0,0) in the bottom left corner
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX <= (int)viewportSize.x && mouseY <= (int)viewportSize.y)
		{

			int pixelData = m_ViewportFramebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData <= -1 ? Entity() : Entity((entt::entity)pixelData, activeScene.get());
			//CM_CORE_INFO("{0}", pixelData);
			//CM_CORE_INFO("{0}, {1}", mouseX, mouseY);
		}

		m_ViewportFramebuffer->Unbind();

		m_GameWindowFramebuffer->Bind();

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
		m_GameWindowFramebuffer->ClearAttachment(1, -1);

		activeScene->OnRenderRuntime();

		m_GameWindowFramebuffer->Unbind();
	}

	void EditorLayer::OnFixedUpdate(float fixedts)
	{
		if(m_IsPlayMode)
			SceneManager::Get().GetActiveScene()->OnFixedUpdate(fixedts);
	}

	void EditorLayer::OnImGuiRender()
	{
		CM_PROFILE_FUNCTION();

		ImGuiWindowFlags modalFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_AlwaysAutoResize;
		if (m_ShowProjectWindow)
		{
			ImGui::OpenPopup("Project Window");

			if (ImGui::BeginPopupModal("Project Window"), &m_ShowProjectWindow, modalFlags)
			{
				int width = ImGui::GetWindowSize().x;
				int height = ImGui::GetWindowSize().y;

				ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

				ImGui::SetCursorPosX(width / 2.0f - width/6.0f);
				ImGui::SetCursorPosY(height / 2.0f - height / 3.0f);
				if (ImGui::Button("New Project", ImVec2(width / 3, height / 4)))
				{
					if (SetProject())
					{
						ImGui::CloseCurrentPopup();
						m_ShowProjectWindow = false;
					}
				}

				ImGui::Spacing();
				ImGui::SetCursorPosX(width / 2.0f - width / 6.0f);
				if (ImGui::Button("Open Project", ImVec2(width / 3, height / 4)))
				{
					if (SetProject())
					{
						ImGui::CloseCurrentPopup();
						m_ShowProjectWindow = false;
					}
				}

				ImGui::PopFont();
				ImGui::EndPopup();
			}
		}

		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 200.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		//style.WindowMinSize.x = 32.0f;

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
				{
					if (ProjectManager::Get().GetProjectPath() == "")
						SetProject();
					OpenScene();
				}

				if (ImGui::MenuItem("Save Scene as...", "Ctrl+Shift+S"))
				{
					if (ProjectManager::Get().GetProjectPath() == "")
						SetProject();
					SaveSceneAs();
				}


				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Project"))
			{
				if (ImGui::MenuItem("Set..."))
				{
					SetProject();
				}

				if (ImGui::MenuItem("Scene Manager"))
				{
					m_SceneManagerPanel.SetPanelVisiblity(true);
				}

				if (ImGui::MenuItem("Build"))
				{
					BuildProject();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGui::BeginMenuBar())
		{
			static const ImVec4 disabledColor = { 0.5f, 0.7f, 0.5f, 1.0f };
			int width = ImGui::GetWindowSize().x;
			int height = ImGui::GetWindowSize().y;

			ImGui::SetCursorPosX(width / 2.0f - 32);

			// Play button
			if (m_IsPlayMode)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, disabledColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, disabledColor);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, disabledColor);
				// Play button
				if (ImGui::Button(u8"\uf04B", ImVec2(32, 20)))
				{
					m_IsPlayMode = false;
					RestoreSceneFromPlayMode();
				}
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();	

				// Stop button
				if (ImGui::Button(u8"\uf04D", ImVec2(32, 20)))
				{
					m_IsPlayMode = false;
					RestoreSceneFromPlayMode();
				}

			}
			else
			{
				if (ImGui::Button(u8"\uf04B", ImVec2(32, 20)))
				{
					m_IsPlayMode = true;
					SceneManager::Get().SerializePlayMode();
					LuaManager::Get().RefreshScripts();
				}

				ImGui::Button(u8"\uf04D", ImVec2(32, 20));
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();
		m_SceneManagerPanel.OnImGuiRender();

		ImGui::Begin("Stats");

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());	

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		if (!ImGui::IsAnyItemActive())
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);
		else
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);
		

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x,  viewportPanelSize.y };
		
		uint64_t textureID = m_ViewportFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			//Editor camera
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4& transform = tc.GetTransform();
			glm::mat4& parentTransform = glm::mat4(1.0f);
			if(tc.GetParent() != nullptr)
				parentTransform = tc.GetParent().GetComponent<TransformComponent>().GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 5.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::mat4 localTransform = glm::inverse(parentTransform) * transform/*tc.GetLocalTransform()*/;
				//glm::mat4& localTransform = tc.GetLocalTransform();
				glm::vec3 position, rotation, scale;
				Math::DecomposeTransform(localTransform, position, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.GetLocalRotation();
				tc.SetLocalPosition(position);
				tc.SetLocalRotation (tc.GetLocalRotation() + deltaRotation);
				tc.SetLocalScale(scale);
			}
		}


		ImGui::End();

		m_AssetManagerPanel.OnImGuiRender();

		ImGui::Begin(u8"\uf01D" " Game");

		auto gameWindowMinRegion = ImGui::GetWindowContentRegionMin();
		auto gameWindowMaxRegion = ImGui::GetWindowContentRegionMax();
		auto gameWindowOffset = ImGui::GetWindowPos();

		ImVec2 vp = ImGui::GetMainViewport()->Pos;
		//CM_CORE_INFO("{0}, {1}", vp.x, vp.y);

		ImVec2 gameWindowPanelSize = ImGui::GetContentRegionAvail();
		m_GameWindowSize = { gameWindowPanelSize.x,  gameWindowPanelSize.y };

		m_GameWindowBounds[0] = { gameWindowOffset.x - Application::Get().GetWindow().GetX(), Application::Get().GetWindow().GetHeight() -
			(gameWindowOffset.y - Application::Get().GetWindow().GetY() + ImGui::GetWindowSize().y) };

		uint64_t gameWindowTextureID = m_GameWindowFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(gameWindowTextureID), ImVec2{ m_GameWindowSize.x, m_GameWindowSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

		ImGui::PopStyleVar(ImGuiStyleVar_WindowPadding);

		ImGui::End();

		
	}

	void EditorLayer::OnEvent(Event& e)
	{
		//m_CameraController.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(CM_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(CM_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<WindowFocusEvent>(CM_BIND_EVENT_FN(EditorLayer::OnWindowFocus));
	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();
				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();
				break;
			}
			case Key::S:
			{
				if (control && shift)
					SaveSceneAs();
				break;
			}

			//Gizmos
			case Key::Q:
				m_GizmoType = -1;
				break;
			case Key::T:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
				break;
		}
	}
	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == 0)
		{
			if(m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}
	bool EditorLayer::OnWindowFocus(WindowFocusEvent& e)
	{
		if (e.GetFocus() && !m_ShowProjectWindow)
			m_AssetManagerPanel.RefreshFiles();

		return false;
	}
	void EditorLayer::NewScene()
	{
		if (!m_IsPlayMode)
		{
			Ref<Scene> activeScene = SceneManager::Get().NewScene((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(activeScene);
			m_SceneManagerPanel.SetActiveScenePath("");
		}
	}
	void EditorLayer::OpenScene()
	{
		if (!m_IsPlayMode)
		{
			std::string filepath = FileDialogs::OpenFile("Chimera Scene (*.chimera)\0*.chimera\0");
			std::string relativePath = filepath;

			if (!filepath.empty() && StringUtils::TransformIntoRelativePath(ProjectManager::Get().GetProjectPath() + "\\assets", relativePath))
			{
				Ref<Scene> activeScene = SceneManager::Get().NewScene((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_SceneHierarchyPanel.SetContext(activeScene);
				m_SceneManagerPanel.SetActiveScenePath(relativePath);

				SceneManager::Get().DeserializeActiveScene(filepath);

				ProjectManager::Get().SetLastOpenScenePath(relativePath);
				ProjectSerializer projectSerializer;
				projectSerializer.SerializeEditorSettings();
			}
		}
	}
	void EditorLayer::SaveSceneAs()
	{
		if (!m_IsPlayMode)
		{
			std::string filepath = FileDialogs::SaveFile("Chimera Scene (*.chimera)\0*.chimera\0");
			std::string relativePath = filepath;

			if (!filepath.empty() && StringUtils::TransformIntoRelativePath(ProjectManager::Get().GetProjectPath() + "\\assets", relativePath))
			{
				SceneManager::Get().SerializeActiveScene(filepath);

				ProjectManager::Get().SetLastOpenScenePath(relativePath);
				ProjectSerializer projectSerializer;
				projectSerializer.SerializeEditorSettings();
				m_SceneManagerPanel.SetActiveScenePath(relativePath);
			}
		}

	}

	bool EditorLayer::SetProject()
	{
		std::string filepath = FileDialogs::SelectFolder();
		if (filepath.empty())
			return false;
		else
		{
			ProjectManager::Get().SetProjectPath(filepath);
			if (!FileDialogs::IsDirectory(filepath + "\\library") || !FileDialogs::IsDirectory(filepath + "\\assets"))
			{
				ProjectManager::Get().SetProjectDirectories();
				//ProjectManager::Get().SetProjectPath(filepath);
				m_SceneHierarchyPanel.SetRootPath(filepath + "\\assets");
				m_AssetManagerPanel.Init();
				NewScene();
				return true;
			}
			m_SceneHierarchyPanel.SetRootPath(filepath + "\\assets");
			OpenProject(filepath);
			return true;
		}
	}
	void EditorLayer::OpenProject(const std::string& path)
	{
		std::string filepath;
		if (path == "")
			filepath = FileDialogs::SelectFolder();
		else
			filepath = path;

		m_AssetManagerPanel.Init();
		ProjectManager& manager = ProjectManager::Get();

		ProjectSerializer projectSerializer;
		projectSerializer.DeserializeEditorSettings();
		projectSerializer.DeserializeBuildSettings();

		Ref<Scene> activeScene = SceneManager::Get().NewScene((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(activeScene);
		activeScene->OnViewportResize(m_GameWindowSize.x, m_GameWindowSize.y);

		std::string relativePath = manager.GetLastOpenScenePath();
		if (relativePath != "")
		{
			SceneManager::Get().DeserializeActiveScene(manager.GetProjectPath() + "\\assets" + manager.GetLastOpenScenePath());

			m_SceneManagerPanel.SetActiveScenePath(relativePath);
		}
		else
			m_SceneManagerPanel.SetActiveScenePath("");

	}
	void EditorLayer::BuildProject()
	{
		std::string filepath = FileDialogs::SelectFolder();
		if (!filepath.empty())
		{
			std::string projectPath = ProjectManager::Get().GetProjectPath();
			FileDialogs::CopyDirectory(projectPath + "\\assets", filepath + "\\assets");
			FileDialogs::CopyDirectory(projectPath + "\\library", filepath + "\\library");
			FileDialogs::CopyDirectory(projectPath + "\\.BuildSettings", filepath + "\\.BuildSettings");
			FileDialogs::CopyItem(FileDialogs::GetExecutableDirectory() + "\\Sandbox.exe", filepath + "\\Sandbox.exe");
		}
	}
	void EditorLayer::RestoreSceneFromPlayMode()
	{
		Ref<Scene> activeScene = SceneManager::Get().NewScene((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(activeScene);

		SceneManager::Get().DeserializePlayMode();
		activeScene->OnViewportResize(m_GameWindowSize.x, m_GameWindowSize.y);
	}
}
