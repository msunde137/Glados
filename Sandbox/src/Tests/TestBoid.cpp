//#include "gladospch.h"
#include "TestBoid.h"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/misc/cpp/imgui_stdlib.h"

namespace test {

	TestBoid::TestBoid()
	{
		RenderCommand::SetBlend(BlendMode::ALPHA);
		RenderCommand::SetClearColor(vec4(0, .3, .3, 1));
		RenderCommand::SetFaceCull(true);

		Ref<Texture2D> tex = Texture2D::Create("res/textures/particle.png");
		m_Particles.Init({ "Boids!", 200, 200, tex, BlendMode::ADD, true });
	}

	TestBoid::~TestBoid()
	{
	}

	void TestBoid::OnUpdate(float timestep)
	{
		m_Camera.OnUpdate(timestep);
		m_Particles.OnUpdate(timestep);
	}

	void TestBoid::OnRender()
	{
		//mat4 a = translate(mat4(1), vec3(0, 0, 1)) * scale(mat4(1), vec3(-2));
		Renderer2D::BeginScene(m_Camera);
		//Renderer2D::DrawQuad(rotate(mat4(1), glm::radians(90.0f), vec3(0,1,0)) * a * rotate(mat4(1), glm::radians(180.0f), vec3(0,1,0)), vec4(0, 1, .8, 1));
		//Renderer2D::DrawQuad(rotate(mat4(1), glm::radians(-90.0f), vec3(0, 1, 0)) * a * rotate(mat4(1), glm::radians(180.0f), vec3(0,1,0)), vec4(0, 1, .8, 1));
		//Renderer2D::DrawQuad(rotate(mat4(1), glm::radians(90.0f), vec3(1, 0, 0)) * a * rotate(mat4(1), glm::radians(180.0f), vec3(0,1,0)), vec4(0, 1, .8, 1));
		//Renderer2D::DrawQuad(rotate(mat4(1), glm::radians(-90.0f), vec3(0, 1, 0)) * a * rotate(mat4(1), glm::radians(180.0f), vec3(0,1,0)), vec4(0, 1, .8, 1));
		//Renderer2D::DrawQuad(rotate(mat4(1), glm::radians(90.0f), vec3(0, 1, 0)) * a * rotate(mat4(1), glm::radians(180.0f), vec3(0,1,0)), vec4(0, 1, .8, 1));
		//Renderer2D::DrawQuad( a * rotate(mat4(1), glm::radians(180.0f), vec3(0,1,0)), vec4(0, 1, .8, 1));
		m_Particles.OnRender(m_Camera);
		Renderer2D::EndScene();
	}

	void TestBoid::OnImGuiRender()
	{
		m_Particles.OnImGuiRender();
	}

	void TestBoid::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		m_Camera.OnEvent(e);
	}

	void TestBoid::OnViewportResize(glm::vec2 viewportSize)
	{
		m_Camera.SetViewportSize(viewportSize.x, viewportSize.y);
	}

}