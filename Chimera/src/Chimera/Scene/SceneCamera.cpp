#include "cmpch.h"
#include "SceneCamera.h"

#include "Chimera/Core/Application.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}
	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;

		RecalculateProjection();
	}
	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		m_FOV = fov;

		RecalculateProjection();
	}
	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		m_Width = width; m_Height = height;
		RecalculateProjection();
	}
	glm::vec3 SceneCamera::ScreenToWorld(const glm::vec2& mousePosition, const glm::mat4& camTransform)
	{
		glm::mat4 PV = m_Projection * glm::inverse(camTransform);
		PV = glm::inverse(PV);
		//glm::vec2 gameWindowOffset = Application::Get().GetGameWindowOffset();

		glm::vec4 in;
		in.x = (2.0f * ((float)(mousePosition.x/* - gameWindowOffset.x*/) / m_Width)) - 1.f;
		in.y = (2.0f * ((float)(mousePosition.y/* - gameWindowOffset.y*/) / m_Height)) - 1.f;
		in.z = 0.f;
		in.w = 1.f;

		glm::vec4 pos = PV * in;

		pos /= pos.w;
		pos.z = 0.f;

		return pos;
	}
	void SceneCamera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective)
		{
			// CM_CORE_ERROR("{0}, {1}", m_FOV, m_AspectRatio);
			m_Projection = glm::perspective(m_FOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}

		else
		{
			float orthoLeft = -0.5 * m_AspectRatio * m_OrthographicSize;
			float orthoRight = 0.5 * m_AspectRatio * m_OrthographicSize;
			float orthoBottom = -0.5 * m_OrthographicSize;
			float orthoTop = 0.5 * m_OrthographicSize;

			m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
	}
}