#include <Chimera.h>

//--------ENTRY POINT----------------
#include <Chimera/Core/EntryPoint.h>
//-----------------------------------

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/OpenGLShader.h"

#include "Sandbox2D.h"


class ExampleLayer : public Chimera::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{
		m_VertexArray = Chimera::VertexArray::Create();

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f, 0.5f, 0.0f, 0.8f, 0.7f, 0.2f, 1.0f
		};

		Chimera::Ref<Chimera::VertexBuffer> vertexBuffer;
		vertexBuffer = Chimera::VertexBuffer::Create(vertices, sizeof(vertices));

		Chimera::BufferLayout layout = {
			{ Chimera::ShaderDataType::Float3, "a_Position" },
			{ Chimera::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] =
		{
			0, 1, 2
		};

		Chimera::Ref<Chimera::IndexBuffer> indexBuffer;
		indexBuffer = Chimera::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Chimera::VertexArray::Create();

		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		};

		Chimera::Ref<Chimera::VertexBuffer> squareVB;
		squareVB = Chimera::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		Chimera::BufferLayout squareVBLayout = {
			{ Chimera::ShaderDataType::Float3, "a_Position" },
			{ Chimera::ShaderDataType::Float2, "a_TexCoord" },
		};

		squareVB->SetLayout(squareVBLayout);

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] =
		{
			0, 1, 2,
			2, 3, 0
		};
		Chimera::Ref<Chimera::IndexBuffer> squareIB;
		squareIB = Chimera::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_VP;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = u_VP * u_Transform * vec4(a_Position, 1.0);
			}
			 
		)";

		std::string fragmentSrc = R"( 
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
			
		)";

		m_Shader = Chimera::Shader::Create("VertexPosTriangle", vertexSrc, fragmentSrc);

		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_VP;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_VP * u_Transform * vec4(a_Position, 1.0);
			}
			 
		)";

		std::string flatColorFragmentSrc = R"( 
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
			
		)";

		m_FlatColorShader = Chimera::Shader::Create("FlatColor", flatColorVertexSrc, flatColorFragmentSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Chimera::Texture2D::Create("assets/textures/laurgras.png");
		m_NuvotekLogoTexture = Chimera::Texture2D::Create("assets/textures/nuvoteklogo.png");

		std::dynamic_pointer_cast<Chimera::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Chimera::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Chimera::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Chimera::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Chimera::RenderCommand::Clear();

		Chimera::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Chimera::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Chimera::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for(int y = 0; y < 20; y++)
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Chimera::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Chimera::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		m_NuvotekLogoTexture->Bind();
		Chimera::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		// Triangle
		// Chimera::Renderer::Submit(m_Shader, m_VertexArray);

		Chimera::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	void OnEvent(Chimera::Event& e) override
	{
		m_CameraController.OnEvent(e);

		/*if (e.GetEventType() == Chimera::EventType::WindowResize)
		{
			auto& re = (Chimera::WindowResizeEvent&)e;
			float zoom = (float)re.GetWidth() / 1280.0f;
			m_CameraController.SetZoomLevel(zoom);
		}*/
	}

private:
	Chimera::ShaderLibrary m_ShaderLibrary;

	Chimera::Ref<Chimera::Shader> m_Shader;
	Chimera::Ref<Chimera::VertexArray> m_VertexArray;

	Chimera::Ref<Chimera::Shader> m_FlatColorShader;
	Chimera::Ref<Chimera::VertexArray> m_SquareVA;

	Chimera::Ref<Chimera::Texture2D> m_Texture, m_NuvotekLogoTexture;

	Chimera::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
};

class Sandbox : public Chimera::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Chimera::Application* Chimera::CreateApplication()
{
	return new Sandbox();
}