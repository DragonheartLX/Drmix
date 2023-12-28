#pragma once
#include <string>
#include <memory>
#include <unordered_map>

#include "OpenGL/OpenGL.hpp"

namespace Drmix
{
	class Node
	{
	public:
		Node();
		~Node();

		virtual void update() {};
		virtual void fixedUpdate(double deltaTime) {};
		virtual void imGuiRender() {};

		void render() const;
		void setRenderStatus(bool status = true);

		void setParent(Node* parent);
		void addChild(std::string objectName, Node* child);

		inline Node* getParent() { return m_Parent; };
		inline std::unordered_map<std::string, Node*> getChildren() { return m_Children; };

		std::unique_ptr<OpenGL::VertexArrayBuffer> VAO;
		std::unique_ptr<OpenGL::VertexBuffer> VBO;
		std::unique_ptr<OpenGL::IndexBuffer> EBO;
		std::unique_ptr<OpenGL::Shader> shader;
	private:
		bool m_IfRender;
		Node* m_Parent;
		std::unordered_map<std::string, Node*> m_Children;
	};
}