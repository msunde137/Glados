#pragma once

#include "Core/Core.h"
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "Buffer.h"

// foreward declaring the object class to avoid infinite inclusion

namespace Glados {

	class Object;
	class Material;

	class ObjAttrib
	{
	public:
		ObjAttrib() {};
		~ObjAttrib() {};

		virtual void OnUpdate() {};
		virtual void OnImGuiRender() {};
	};

	class Transform : public ObjAttrib
	{
	public:
		glm::vec3 position, scale, rotation, up, forward;
		glm::mat4 transform;
	private:
		glm::vec3 o_pos, o_scl, o_rot, o_up, o_fwd;
	public:
		Transform(glm::vec3 pos = glm::vec3(0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
		~Transform();

		void OnUpdate() override;
		void OnImGuiRender() override;
		void UpdateBasis(glm::mat4 mat);
		void UpdatePosition(glm::mat4 mat);
		void UpdateRotation(glm::vec3 rot);

	private:
		void UpdateTransform();
		void RotateVec4(glm::vec4& v4, glm::vec3 rot);
		void RotateVec3(glm::vec3& v3, glm::vec3 rot);

	};

	class Mesh : public ObjAttrib
	{
	public:
		std::shared_ptr<VertexArray> VAO;
		std::unique_ptr<IndexBuffer> IB;

	private:
		std::string filepath;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;
		std::unique_ptr<VertexBuffer> VB;
		std::unique_ptr<VertexBuffer> NB;

	public:
		Mesh(const std::string& filepath, std::shared_ptr<VertexArray> vao);
		~Mesh();

		void OnImGuiRender() override;

		inline unsigned int Size() { return indices.size(); }
		void FlatNormals();
		void SmoothNormals();

	private:
		void Parse();
	};

	class Camera : public ObjAttrib
	{
	public:
		bool active;
		glm::mat4 projection;
		glm::mat4 view;
	private:
		std::shared_ptr<Transform> transf;
		glm::vec3 eye, center, up;
	public:
		Camera(glm::mat4 proj, std::shared_ptr<Transform> transf);
		~Camera();

		void OnUpdate() override;

		void UpdateView();
	};

	class Light : public ObjAttrib
	{
	public:
		bool active;
		glm::vec4 lightDir;
		glm::vec4 lightCol;
		float lightInt;

	public:
		Light(glm::vec4 lightdir, glm::vec4 lightcol, float lightint);
		~Light();

		void OnUpdate() override;
		void OnImGuiRender() override;
	};

	class MeshRenderer : public ObjAttrib
	{
	private:
		std::shared_ptr<Material> material;
		Object* activeCamera;
		bool isLit;

	public:
		MeshRenderer(std::shared_ptr<Material> mat, Object* activecamera);
		~MeshRenderer();

		void OnUpdate() override;
		void OnImGuiRender() override;

		void Clear() const;
		void Draw(const Transform& transf, const Mesh& mesh);
	};

}