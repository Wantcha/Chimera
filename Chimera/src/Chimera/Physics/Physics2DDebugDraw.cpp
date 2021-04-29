#include "cmpch.h"
#include "Physics2DDebugDraw.h"
#include "Chimera/Renderer/Renderer2D.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#define BUFFER_OFFSET(x)  ((const void*) (x))
#define DEBUG_COLOR_R 0.0f;
#define DEBUG_COLOR_G 1.0f;
#define DEBUG_COLOR_B 0.0f;
#define DEBUG_COLOR_A 1.0f;

namespace Chimera
{
	Physics2DDebugDraw g_debugDraw;

	static GLuint sCreateShaderFromString(const char* source, GLenum type)
	{
		GLuint res = glCreateShader(type);
		const char* sources[] = { source };
		glShaderSource(res, 1, sources, NULL);
		glCompileShader(res);
		GLint compile_ok = GL_FALSE;
		glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
		if (compile_ok == GL_FALSE)
		{
			fprintf(stderr, "Error compiling shader of type %d!\n", type);
			//sPrintLog(res);
			glDeleteShader(res);
			return 0;
		}

		return res;
	}

	static GLuint sCreateShaderProgram(const char* vs, const char* fs)
	{
		GLuint vsId = sCreateShaderFromString(vs, GL_VERTEX_SHADER);
		GLuint fsId = sCreateShaderFromString(fs, GL_FRAGMENT_SHADER);
		assert(vsId != 0 && fsId != 0);

		GLuint programId = glCreateProgram();
		glAttachShader(programId, vsId);
		glAttachShader(programId, fsId);
		glBindFragDataLocation(programId, 0, "color");
		glLinkProgram(programId);

		glDeleteShader(vsId);
		glDeleteShader(fsId);

		GLint status = GL_FALSE;
		glGetProgramiv(programId, GL_LINK_STATUS, &status);
		assert(status != GL_FALSE);

		return programId;
	}

	struct GLRenderPoints
	{
		void Create()
		{
			const char* vs = \
				"#version 330\n"
				"uniform mat4 projectionMatrix;\n"
				"layout(location = 0) in vec2 v_position;\n"
				"layout(location = 1) in vec4 v_color;\n"
				"layout(location = 2) in float v_size;\n"
				"out vec4 f_color;\n"
				"void main(void)\n"
				"{\n"
				"	f_color = v_color;\n"
				"	gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);\n"
				"   gl_PointSize = v_size;\n"
				"}\n";

			const char* fs = \
				"#version 330\n"
				"in vec4 f_color;\n"
				"layout(location = 0) out vec4 color;\n"
				"void main(void)\n"
				"{\n"
				"	color = f_color;\n"
				"}\n";

			m_programId = sCreateShaderProgram(vs, fs);
			m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
			m_vertexAttribute = 0;
			m_colorAttribute = 1;
			m_sizeAttribute = 2;

			// Generate
			glGenVertexArrays(1, &m_vaoId);
			glGenBuffers(3, m_vboIds);

			glBindVertexArray(m_vaoId);
			glEnableVertexAttribArray(m_vertexAttribute);
			glEnableVertexAttribArray(m_colorAttribute);
			glEnableVertexAttribArray(m_sizeAttribute);

			// Vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
			glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
			glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
			glVertexAttribPointer(m_sizeAttribute, 1, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_sizes), m_sizes, GL_DYNAMIC_DRAW);

			//sCheckGLError();

			// Cleanup
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			m_count = 0;
		}

		void Destroy()
		{
			if (m_vaoId)
			{
				glDeleteVertexArrays(1, &m_vaoId);
				glDeleteBuffers(3, m_vboIds);
				m_vaoId = 0;
			}

			if (m_programId)
			{
				glDeleteProgram(m_programId);
				m_programId = 0;
			}
		}

		void Vertex(const b2Vec2& v, const b2Color& c, float size)
		{
			if (m_count == e_maxVertices)
				Flush();

			m_vertices[m_count] = v;
			m_colors[m_count] = c;
			m_sizes[m_count] = size;
			++m_count;
		}

		void Flush()
		{
			if (m_count == 0)
				return;

			glUseProgram(m_programId);

			glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(g_debugDraw.GetCamera().GetViewProjection()));

			glBindVertexArray(m_vaoId);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2), m_vertices);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color), m_colors);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(float), m_sizes);

			glEnable(GL_PROGRAM_POINT_SIZE);
			glDrawArrays(GL_POINTS, 0, m_count);
			glDisable(GL_PROGRAM_POINT_SIZE);

			//sCheckGLError();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glUseProgram(0);

			m_count = 0;
		}

		enum { e_maxVertices = 512 };
		b2Vec2 m_vertices[e_maxVertices];
		b2Color m_colors[e_maxVertices];
		float m_sizes[e_maxVertices];

		int32 m_count;

		GLuint m_vaoId;
		GLuint m_vboIds[3];
		GLuint m_programId;
		GLint m_projectionUniform;
		GLint m_vertexAttribute;
		GLint m_colorAttribute;
		GLint m_sizeAttribute;
	};

	struct GLRenderLines
	{
		void Create()
		{
			const char* vs = \
				"#version 330\n"
				"uniform mat4 projectionMatrix;\n"
				"layout(location = 0) in vec2 v_position;\n"
				"layout(location = 1) in vec4 v_color;\n"
				"out vec4 f_color;\n"
				"void main(void)\n"
				"{\n"
				"	f_color = v_color;\n"
				"	gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);\n"
				"}\n";

			const char* fs = \
				"#version 330\n"
				"in vec4 f_color;\n"
				"layout(location = 0) out vec4 color;\n"
				"void main(void)\n"
				"{\n"
				"	color = f_color;\n"
				"}\n";

			m_programId = sCreateShaderProgram(vs, fs);
			m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
			m_vertexAttribute = 0;
			m_colorAttribute = 1;

			// Generate
			glGenVertexArrays(1, &m_vaoId);
			glGenBuffers(2, m_vboIds);

			glBindVertexArray(m_vaoId);
			glEnableVertexAttribArray(m_vertexAttribute);
			glEnableVertexAttribArray(m_colorAttribute);

			// Vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
			glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
			glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

			// Cleanup
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			m_count = 0;
		}

		void Destroy()
		{
			if (m_vaoId)
			{
				glDeleteVertexArrays(1, &m_vaoId);
				glDeleteBuffers(2, m_vboIds);
				m_vaoId = 0;
			}

			if (m_programId)
			{
				glDeleteProgram(m_programId);
				m_programId = 0;
			}
		}

		void Vertex(const b2Vec2& v, const b2Color& c)
		{
			if (m_count == e_maxVertices)
				Flush();

			m_vertices[m_count] = v;
			m_colors[m_count] = c;
			++m_count;
		}

		void Flush()
		{
			if (m_count == 0)
				return;

			glUseProgram(m_programId);

			glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(g_debugDraw.GetCamera().GetViewProjection()));

			glBindVertexArray(m_vaoId);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2), m_vertices);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color), m_colors);

			glDrawArrays(GL_LINES, 0, m_count);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glUseProgram(0);

			m_count = 0;
		}

		enum { e_maxVertices = 2 * 512 };
		b2Vec2 m_vertices[e_maxVertices];
		b2Color m_colors[e_maxVertices];

		int32 m_count;

		GLuint m_vaoId;
		GLuint m_vboIds[2];
		GLuint m_programId;
		GLint m_projectionUniform;
		GLint m_vertexAttribute;
		GLint m_colorAttribute;
	};


	struct GLRenderTriangles
	{
		void Create()
		{
			const char* vs = \
				"#version 330\n"
				"uniform mat4 projectionMatrix;\n"
				"layout(location = 0) in vec2 v_position;\n"
				"layout(location = 1) in vec4 v_color;\n"
				"out vec4 f_color;\n"
				"void main(void)\n"
				"{\n"
				"	f_color = v_color;\n"
				"	gl_Position = projectionMatrix * vec4(v_position, 0.0f, 1.0f);\n"
				"}\n";

			const char* fs = \
				"#version 330\n"
				"in vec4 f_color;\n"
				"layout(location = 0) out vec4 color;\n"
				//"layout(location = 1) out int color2;\n"
				"void main(void)\n"
				"{\n"
				"	color = f_color;\n"
				//"   color2 = 0;\n"
				"}\n";

			m_programId = sCreateShaderProgram(vs, fs);
			m_projectionUniform = glGetUniformLocation(m_programId, "projectionMatrix");
			m_vertexAttribute = 0;
			m_colorAttribute = 1;

			// Generate
			glGenVertexArrays(1, &m_vaoId);
			glGenBuffers(2, m_vboIds);

			glBindVertexArray(m_vaoId);
			glEnableVertexAttribArray(m_vertexAttribute);
			glEnableVertexAttribArray(m_colorAttribute);

			// Vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
			glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
			glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
			glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

			// Cleanup
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			m_count = 0;
		}

		void Destroy()
		{
			if (m_vaoId)
			{
				glDeleteVertexArrays(1, &m_vaoId);
				glDeleteBuffers(2, m_vboIds);
				m_vaoId = 0;
			}

			if (m_programId)
			{
				glDeleteProgram(m_programId);
				m_programId = 0;
			}
		}

		void Vertex(const b2Vec2& v, const b2Color& c)
		{
			if (m_count == e_maxVertices)
				Flush();

			m_vertices[m_count] = v;
			m_colors[m_count] = c;
			++m_count;
		}

		void Flush()
		{
			if (m_count == 0)
				return;

			glUseProgram(m_programId);

			glUniformMatrix4fv(m_projectionUniform, 1, GL_FALSE, glm::value_ptr(g_debugDraw.GetCamera().GetViewProjection()));

			glBindVertexArray(m_vaoId);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2), m_vertices);

			glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color), m_colors);

			//glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDrawArrays(GL_TRIANGLES, 0, m_count);
			//glDisable(GL_BLEND);

			//sCheckGLError();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glUseProgram(0);

			m_count = 0;
		}

		enum { e_maxVertices = 3 * 512 };
		b2Vec2 m_vertices[e_maxVertices];
		b2Color m_colors[e_maxVertices];

		int32 m_count = 0;

		GLuint m_vaoId;
		GLuint m_vboIds[2];
		GLuint m_programId;
		GLint m_projectionUniform;
		GLint m_vertexAttribute;
		GLint m_colorAttribute;
	};

	void Physics2DDebugDraw::Create()
	{
		m_points = new GLRenderPoints;
		m_points->Create();
		m_lines = new GLRenderLines;
		m_lines->Create();
		m_triangles = new GLRenderTriangles;
		m_triangles->Create();
	}

	void Physics2DDebugDraw::Destroy()
	{
		m_points->Destroy();
		delete m_points;
		m_points = NULL;

		m_lines->Destroy();
		delete m_lines;
		m_lines = NULL;

		m_triangles->Destroy();
		delete m_triangles;
		m_triangles = NULL;
	}

	void Physics2DDebugDraw::Flush()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		m_triangles->Flush();
		m_lines->Flush();
		m_points->Flush();
	}

	void Physics2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		b2Color drawColor;
		//drawColor.Set(DEBUG_COLOR_R, DEBUG_COLOR_G, DEBUG_COLOR_B, DEBUG_COLOR_A);
		drawColor.Set(0, 1, 0, 1);
		b2Vec2 p1 = vertices[vertexCount - 1];
		for (int32 i = 0; i < vertexCount; ++i)
		{
			b2Vec2 p2 = vertices[i];
			m_lines->Vertex(p1, drawColor);
			m_lines->Vertex(p2, color);
			p1 = p2;
		}
	}
	void Physics2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
	{
		b2Color drawColor;
		//drawColor.Set(DEBUG_COLOR_R, DEBUG_COLOR_G, DEBUG_COLOR_B, DEBUG_COLOR_A);
		drawColor.Set(0, 1, 0, 1);

		b2Color fillColor(0.5f * drawColor.r, 0.5f * drawColor.g, 0.5f * drawColor.b, 0.5f);

		for (int32 i = 1; i < vertexCount - 1; ++i)
		{
			m_triangles->Vertex(vertices[0], fillColor);
			m_triangles->Vertex(vertices[i], fillColor);
			m_triangles->Vertex(vertices[i + 1], fillColor);
		}

		b2Vec2 p1 = vertices[vertexCount - 1];
		for (int32 i = 0; i < vertexCount; ++i)
		{
			b2Vec2 p2 = vertices[i];
			m_lines->Vertex(p1, drawColor);
			m_lines->Vertex(p2, drawColor);
			p1 = p2;
		}
	}
	void Physics2DDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
	{
		b2Color drawColor;
		drawColor.Set(0, 1, 0, 1);

		const float k_segments = 16.0f;
		const float k_increment = 2.0f * b2_pi / k_segments;
		float sinInc = sinf(k_increment);
		float cosInc = cosf(k_increment);
		b2Vec2 r1(1.0f, 0.0f);
		b2Vec2 v1 = center + radius * r1;
		for (int32 i = 0; i < k_segments; ++i)
		{
			// Perform rotation to avoid additional trigonometry.
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			m_lines->Vertex(v1, drawColor);
			m_lines->Vertex(v2, drawColor);
			r1 = r2;
			v1 = v2;
		}
	}
	void Physics2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
	{
		b2Color drawColor;
		drawColor.Set(0, 1, 0, 1);

		const float k_segments = 16.0f;
		const float k_increment = 2.0f * b2_pi / k_segments;
		float sinInc = sinf(k_increment);
		float cosInc = cosf(k_increment);
		b2Vec2 v0 = center;
		b2Vec2 r1(cosInc, sinInc);
		b2Vec2 v1 = center + radius * r1;
		b2Color fillColor(0.5f * drawColor.r, 0.5f * drawColor.g, 0.5f * drawColor.b, 0.5f);
		for (int32 i = 0; i < k_segments; ++i)
		{
			// Perform rotation to avoid additional trigonometry.
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			m_triangles->Vertex(v0, fillColor);
			m_triangles->Vertex(v1, fillColor);
			m_triangles->Vertex(v2, fillColor);
			r1 = r2;
			v1 = v2;
		}

		r1.Set(1.0f, 0.0f);
		v1 = center + radius * r1;
		for (int32 i = 0; i < k_segments; ++i)
		{
			b2Vec2 r2;
			r2.x = cosInc * r1.x - sinInc * r1.y;
			r2.y = sinInc * r1.x + cosInc * r1.y;
			b2Vec2 v2 = center + radius * r2;
			m_lines->Vertex(v1, drawColor);
			m_lines->Vertex(v2, drawColor);
			r1 = r2;
			v1 = v2;
		}

		// Draw a line fixed in the circle to animate rotation.
		b2Vec2 p = center + radius * axis;
		m_lines->Vertex(center, drawColor);
		m_lines->Vertex(p, drawColor);
	}
	void Physics2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
	{

	}
	void Physics2DDebugDraw::DrawTransform(const b2Transform& xf)
	{

	}
	void Physics2DDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
	{

	}
}