#pragma once

#include "Chimera/Renderer/Camera.h"

namespace Chimera
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType {Perspective = 0, Orthographic = 1};

	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float fov, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetFOV() const { return m_FOV; }
		void SetFOV(float fov) { m_FOV = fov; RecalculateProjection(); }
		float GetPerspNearClip() const { return m_PerspectiveNear; }
		float GetPerspFarClip() const { return m_PerspectiveFar; }
		void SetPerspNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
		void SetPerspFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

		float GetOrthographicSize() const { return m_OrthographicSize; }
		void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
		float GetOrthoNearClip() const { return m_OrthographicNear; }
		float GetOrthoFarClip() const { return m_OrthographicFar; }
		void SetOrthoNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
		void SetOrthoFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		glm::vec3 ScreenToWorld(const glm::vec2& mousePosition, const glm::mat4& camTransform);
	private:
		void RecalculateProjection();

		ProjectionType m_ProjectionType = ProjectionType::Orthographic;

		float m_OrthographicSize = 10.0f;
		float m_OrthographicNear = 0.01f;
		float m_OrthographicFar = 1000.0f;

		float m_FOV = glm::radians(45.0f);
		float m_PerspectiveNear = 0.01f;
		float m_PerspectiveFar = 10000.0f;

		uint32_t m_Width, m_Height;

		//glm::mat4 m_Transform;

		float m_AspectRatio = 1.7778f;
	};
}
