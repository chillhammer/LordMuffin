#include <skelpch.h>
#include "VertexArray.h"
#include <GLFW/glfw3.h>

namespace Skel
{
	VertexArray::VertexArray()
	{
		m_HasBuffer = false;
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	VertexArray::VertexArray(const VertexArray & other)
	{
		ASSERT(false, "Restriction on copying Vertex Array");
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); ++i)
		{
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			if (element.type != GL_UNSIGNED_INT) {
				GLCall(glVertexAttribPointer(i, element.count, element.type,
					element.normalized, layout.GetStride(), (const void*)offset));
			}
			else {
				GLCall(glVertexAttribIPointer(i, element.count, element.type,
					layout.GetStride(), (const void*)offset));
			}
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		}

	}
}