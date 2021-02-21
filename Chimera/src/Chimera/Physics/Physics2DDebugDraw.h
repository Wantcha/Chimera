#pragma once

#include "box2d/box2d.h"
#include "Chimera/Renderer/EditorCamera.h"

namespace Chimera
{   
    struct GLRenderPoints;
    struct GLRenderLines;
    struct GLRenderTriangles;

	class Physics2DDebugDraw : public b2Draw
	{

    public:
        void Create();
        void Destroy();
        void Flush();

        virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

        virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

        virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

        virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

        virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

        virtual void DrawTransform(const b2Transform& xf) override;

        virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

        void SetCamera(EditorCamera& camera) { m_Camera = camera; }
        EditorCamera& GetCamera() { return m_Camera; }

        //virtual void DrawString(int x, int y, const char* string, ...) override;

        //virtual void DrawAABB(b2AABB* aabb, const b2Color& color) override;

        GLRenderPoints* m_points;
        GLRenderLines* m_lines;
        GLRenderTriangles* m_triangles;

    private:
        EditorCamera m_Camera;
    };

    extern Physics2DDebugDraw g_debugDraw;
}