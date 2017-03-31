/*
Original code (tinyxml2++) by Archer Xu via tinyxml2 by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.*/

/*
this is to make tinyxml2 more convenience to use in project.
Now you can use tinyxml2 like this.

#include "tinyxml2++.h"

int main(int argc, char* argv[])
{
	tinyxml2::XMLPPDocument doc("1.0","utf-8");

	doc["root"]["name"] = "archer";
	doc["root"]["age"] = 10;

	doc["root"]["info"]["tel"] = "1000229";

	cout << (const char*)doc << endl;
	
	return 0;
}

output : 
<?xml version="1.0" encoding="utf-8"?>
<root>
	<name>archer</name>
	<age>10</age>
	<info>
		<tel>1000229</tel>
	</info>
</root>

*/

#pragma once

#ifndef TINYXML2PP_INCLUDED
#define TINYXML2PP_INCLUDED

#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include "tinyxml2.h"

#ifdef _MSC_VER
#   pragma warning(push)
#   pragma warning(disable: 4251)
#endif

namespace tinyxml2
{
	class XMLPPDocument;
	class XMLPPNode;

	class TINYXML2_LIB XMLPPNode
	{
	public:
		typedef std::unordered_map<std::string, std::unordered_set<XMLPPNode*> > _XMLPPChildList;
	public:
		XMLPPNode(tinyxml2::XMLElement* pCur, tinyxml2::XMLDocument& doc) : m_pCurElement(pCur), m_doc(doc)
		{
			m_mapChildElement.clear();
		}

		XMLPPNode(const XMLPPNode& base) : m_pCurElement(base.m_pCurElement), m_doc(base.m_doc)
		{
	
		}

		virtual ~XMLPPNode();

		XMLPPNode& operator=(const XMLPPNode& base);
	private:
		//Add a child element to this node 
		XMLPPNode* AddElement(const char* key);
		//Look up a child element from this node which named "key"
		XMLPPNode* GetElement(const char* key);
	public:
		//Set value to this child node
		void SetValue(const char* value);

		//Set attribute to this child node;
		void SetAttribute(const char* name, const char* value);

		//Set comment to this child node;
		void SetComment(const char* comment);

		//query text inside this element
		const char* GetText();

		//query key name of this element
		const char* GetName();

		/*
			append a node to this element.this for array in xml

			tinyxml2::XMLPPDocument doc("1.0","utf-8");

			tinyxml2::XMLPPNode& info = doc["root"].Append("info");
			info["name"] = "jason";
			info["age"] = 10;

			tinyxml2::XMLPPNode& info2 = doc["root"].Append("info");
			info2["name"] = "lily";
			info2["age"] = 7;

			cout << (const char*)doc << endl;

			return 0;

			output : 

			<?xml version="1.0" encoding="utf-8"?>
			<root>
				<info>
					<name>jason</name>
					<age>10</age>
				</info>
				<info>
					<name>lily</name>
					<age>7</age>
				</info>
			</root>
		*/
		XMLPPNode& Append(const char* key);

		bool operator==(const XMLPPNode& o);

		/* you can get value from a node directly like
			doc["root"]["age"] = 100;

			int nAge = doc["root"]["age"];

			nAge will be 100;
		*/
		operator const char*() const;

		operator int() const;

		operator double() const;

		operator long() const;

		operator int64_t() const;

		XMLPPNode& operator[](const char* key);

		XMLPPNode& operator[](std::string key);

		template<typename __VALUETYPE>
		XMLPPNode& operator=(__VALUETYPE value)
		{
			std::ostringstream s;
			s << value;
			SetValue(s.str().c_str());
			return (*this);
		}
	private:
		tinyxml2::XMLDocument& m_doc;

		tinyxml2::XMLElement* m_pCurElement;

		_XMLPPChildList m_mapChildElement;
	};

	class TINYXML2_LIB XMLPPDocument
	{
	public:
		XMLPPDocument();
		XMLPPDocument(const char* version, const char* encode, const char* standalone = NULL);

		virtual ~XMLPPDocument();
	private:
		//Add a child element to the document 
		XMLPPNode* AddElement(const char* key);
		//Find a child element from the document
		XMLPPNode* GetElement(const char* key);
	public:
		//Set the declaration
		void SetDeclaration(const char* version, const char* encode, const char* standalone = NULL);

		//Set the comment
		void SetComment(const char* comment);

		XMLPPNode& operator[](const char* key);

		operator const char*();
	private:
		bool m_bHaveDeclaration;
		tinyxml2::XMLDocument m_xml;
		tinyxml2::XMLPrinter m_printer;
		XMLPPNode::_XMLPPChildList m_mapChildElement;
	};
}



#endif
