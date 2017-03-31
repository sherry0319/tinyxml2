#include "tinyxml2++.h"

namespace tinyxml2
{
	XMLPPDocument::XMLPPDocument()
	{
		m_bHaveDeclaration = false;
		m_xml.Clear();
		m_mapChildElement.clear();
	}

	XMLPPDocument::XMLPPDocument(const char* version, const char* encode, const char* standalone)
	{
		m_xml.Clear();

		m_mapChildElement.clear();

		m_bHaveDeclaration = false;

		SetDeclaration(version, encode, standalone);

	}

	XMLPPDocument::~XMLPPDocument()
	{
		m_xml.Clear();

		for (auto& pair : m_mapChildElement)
		{
			for (auto& node : pair.second)
			{
				delete node;
			}
		}

		m_mapChildElement.clear();
		m_bHaveDeclaration = false;
	}

	XMLPPDocument::operator const char*()
	{
		m_printer.ClearBuffer();
		m_xml.Print(&m_printer);
		return m_printer.CStr();
	}

	XMLPPNode* XMLPPDocument::AddElement(const char* key)
	{
		tinyxml2::XMLElement* pRootElement = m_xml.NewElement(key);

		m_xml.InsertEndChild(pRootElement);

		XMLPPNode* pNode = new XMLPPNode(pRootElement, m_xml);

		m_mapChildElement[key].insert(pNode);

		return pNode;
	}

	XMLPPNode* XMLPPDocument::GetElement(const char* key)
	{
		auto it = m_mapChildElement.find(key);
		if (it != m_mapChildElement.end() && !it->second.empty())
		{
			return (*(it->second.begin()));
		}

		return NULL;
	}

	void XMLPPDocument::SetDeclaration(const char* version, const char* encode, const char* standalone)
	{
		char buffer[256] = { 0 };
		if (standalone == NULL)
		{
			_snprintf_s(buffer, 255, "xml version=\"%s\" encoding=\"%s\"", version, encode);
		}
		else
		{
			_snprintf_s(buffer, 255, "xml version=\"%s\" encoding=\"%s\" standalone=\"%s\"", version, encode, standalone);
		}

		if (m_bHaveDeclaration)
		{
			tinyxml2::XMLDeclaration* pDec = (tinyxml2::XMLDeclaration*)m_xml.FirstChild();

			pDec->SetValue(buffer);
		}
		else
		{
			m_xml.InsertFirstChild(m_xml.NewDeclaration(buffer));

			m_bHaveDeclaration = true;
		}
	}

	void XMLPPDocument::SetComment(const char* comment)
	{
		if (m_bHaveDeclaration)
		{
			m_xml.InsertAfterChild(m_xml.FirstChildElement(), m_xml.NewComment(comment));
		}
		else
		{
			m_xml.InsertFirstChild(m_xml.NewComment(comment));
		}
	}

	XMLPPNode& XMLPPDocument::operator[](const char* key)
	{
		XMLPPNode* pNode = GetElement(key);
		if (pNode == NULL)
		{
			pNode = AddElement(key);
		}

		return (*pNode);
	}

	XMLPPNode::~XMLPPNode()
	{
		m_pCurElement = NULL;

		for (auto& pair : m_mapChildElement)
		{
			for (auto& node : pair.second)
			{
				delete node;
			}
		}

		m_mapChildElement.clear();
	}

	XMLPPNode& XMLPPNode::operator=(const XMLPPNode& base)
	{
		m_pCurElement = base.m_pCurElement;

		return (*this);
	}

	bool XMLPPNode::operator==(const XMLPPNode& o)
	{
		return (m_pCurElement == o.m_pCurElement);
	}

	XMLPPNode::operator const char*() const
	{
		const char* value = m_pCurElement->GetText();
		if (value == NULL)
		{
			return "";
		}
		return value;
	}

	XMLPPNode::operator int() const
	{
		const char* value = m_pCurElement->GetText();
		if (value == NULL)
		{
			return 0;
		}
		return atoi(value);
	}

	XMLPPNode::operator double() const
	{
		const char* value = m_pCurElement->GetText();
		if (value == NULL)
		{
			return 0.000000;
		}
		return atof(value);
	}

	XMLPPNode::operator long() const
	{
		const char* value = m_pCurElement->GetText();
		if (value == NULL)
		{
			return 0;
		}
		return atol(value);
	}

	XMLPPNode::operator int64_t() const
	{
		const char* value = m_pCurElement->GetText();
		if (value == NULL)
		{
			return 0;
		}
		return (int64_t) atoll(value);
	}

	XMLPPNode* XMLPPNode::AddElement(const char* key)
	{
		tinyxml2::XMLElement* pElementChild = m_doc.NewElement(key);

		m_pCurElement->LinkEndChild(pElementChild);

		XMLPPNode* pNode = new XMLPPNode(pElementChild,m_doc);

		m_mapChildElement[key].insert(pNode);

		return pNode;
	}

	XMLPPNode* XMLPPNode::GetElement(const char* key)
	{
		auto it = m_mapChildElement.find(key);
		if (it != m_mapChildElement.end() && !it->second.empty())
		{
			return (*(it->second.begin()));
		}

		return NULL;
	}

	void XMLPPNode::SetValue(const char* value)
	{
		m_pCurElement->SetText(value);
	}

	void XMLPPNode::SetAttribute(const char* name, const char* value)
	{
		m_pCurElement->SetAttribute(name, value);
	}

	void XMLPPNode::SetComment(const char* comment)
	{
		m_pCurElement->LinkEndChild(m_doc.NewComment(comment));
	}

	const char* XMLPPNode::GetText()
	{
		return m_pCurElement->GetText();
	}

	const char* XMLPPNode::GetName()
	{
		return m_pCurElement->Value();
	}

	XMLPPNode& XMLPPNode::Append(const char* key)
	{
		XMLPPNode* pNewNode = AddElement(key);

		return (*pNewNode);
	}

	XMLPPNode& XMLPPNode::operator[](const char* key)
	{
		XMLPPNode* pNode = GetElement(key);
		if (pNode == NULL)
		{
			pNode = AddElement(key);
		}

		return (*pNode);
	}

	XMLPPNode& XMLPPNode::operator[](std::string key)
	{
		XMLPPNode* pNode = GetElement(key.c_str());
		if (pNode == NULL)
		{
			pNode = AddElement(key.c_str());
		}

		return (*pNode);
	}
}
