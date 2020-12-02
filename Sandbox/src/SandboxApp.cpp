#include <Chimera.h>

#include "imgui/imgui.h"

class ExampleLayer : public Chimera::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPos(0.0f, 0.0f, 0.0f)
	{
		m_VertexArray.reset(Chimera::VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f, 0.5f, 0.0f, 0.8f, 0.7f, 0.2f, 1.0f
		};

		std::shared_ptr<Chimera::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Chimera::VertexBuffer::Create(vertices, sizeof(vertices)));

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

		std::shared_ptr<Chimera::IndexBuffer> indexBuffer;
		indexBuffer.reset(Chimera::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Chimera::VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f, 0.75f, 0.0f,
			-0.75f, 0.75f, 0.0f
		};

		std::shared_ptr<Chimera::VertexBuffer> squareVB;
		squareVB.reset(Chimera::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		Chimera::BufferLayout squareVBLayout = {
			{ Chimera::ShaderDataType::Float3, "a_Position" },
		};

		squareVB->SetLayout(squareVBLayout);

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] =
		{
			0, 1, 2,
			2, 3, 0
		};
		std::shared_ptr<Chimera::IndexBuffer> squareIB;
		squareIB.reset(Chimera::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_VP;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = u_VP * vec4(a_Position, 1.0);
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

		m_Shader.reset(new Chimera::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_VP;

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_VP * vec4(a_Position, 1.0);
			}
			 
		)";

		std::string blueShaderFragmentSrc = R"( 
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
			
		)";

		m_BlueShader.reset(new Chimera::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnUpdate(Chimera::Timestep ts) override
	{
		if (Chimera::Input::IsKeyPressed(KEY_LEFT))
		{
			m_CameraPos.x -= m_CameraMoveSpeed * ts;
		}

		else if (Chimera::Input::IsKeyPressed(KEY_RIGHT))
		{
			m_CameraPos.x += m_CameraMoveSpeed * ts;
		}

		if (Chimera::Input::IsKeyPressed(KEY_UP))
		{
			m_CameraPos.y += m_CameraMoveSpeed * ts;
		}

		else if (Chimera::Input::IsKeyPressed(KEY_DOWN))
		{
			m_CameraPos.y -= m_CameraMoveSpeed * ts;
		}

		if (Chimera::Input::IsKeyPressed(KEY_A))
		{
			m_CameraRot += m_CameraRotationSpeed * ts;
		}

		else if (Chimera::Input::IsKeyPressed(KEY_D))
		{
			m_CameraRot -= m_CameraRotationSpeed * ts;
		}

		Chimera::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Chimera::RenderCommand::Clear();

		m_Camera.SetPosition({ m_CameraPos });
		m_Camera.SetRotation(m_CameraRot);

		Chimera::Renderer::BeginScene(m_Camera);

		Chimera::Renderer::Submit(m_BlueShader, m_SquareVA);
		Chimera::Renderer::Submit(m_Shader, m_VertexArray);

		Chimera::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{

	}

	void OnEvent(Chimera::Event& event) override
	{
	}

private:
	std::shared_ptr<Chimera::Shader> m_Shader;
	std::shared_ptr<Chimera::VertexArray> m_VertexArray;

	std::shared_ptr<Chimera::Shader> m_BlueShader;
	std::shared_ptr<Chimera::VertexArray> m_SquareVA;

	Chimera::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPos;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 60.0f;
	float m_CameraRot = 0.0f;
};

class Sandbox : public Chimera::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Chimera::Application* Chimera::CreateApplication()
{
	return new Sandbox();
}