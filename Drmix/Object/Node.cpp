#include "Object/Node.hpp"
#include "Core/Core.hpp"

namespace Drmix
{
	Node::Node():
		m_Parent(nullptr),
		m_IfRender(false)
	{

	}

	Node::~Node()
	{

	}

	void Node::render() const
	{
		if (m_IfRender)
		{
			Drmix::getInstance()->mainRenderer->render(*VAO.get(), *EBO.get(), *shader.get());
		}
	}

	void Node::setRenderStatus(bool status)
	{
		m_IfRender = status;
	}

	void Node::setParent(Node* parent)
	{
		m_Parent = parent;
	}

	void Node::addChild(std::string objectName, Node* child)
	{
		m_Children.insert(std::pair(objectName, child));
	}
}