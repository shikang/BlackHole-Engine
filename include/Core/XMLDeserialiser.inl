// Deserialise
namespace BH
{
	inline bool XMLDeserialiser::LoadFile(const BH::String & filename, SerialiseDoc & doc)
	{
		return doc.LoadFile(filename.c_str()) == tinyxml2::XMLError::XML_NO_ERROR;
	}

	// ----------------------------- Get Element ------------------------------------
	inline SerialiseElement * XMLDeserialiser::GetElem(const BH::String & tag_name, SerialiseElement* elem)
	{
		return elem->FirstChildElement( (tag_name == "") ? nullptr : tag_name.c_str() );
	}

	inline SerialiseElement* XMLDeserialiser::GetElem(const BH::String & tag_name, SerialiseDoc & doc)
	{
		return doc.FirstChildElement( (tag_name == "") ? nullptr : tag_name.c_str() );
	}

	inline SerialiseElement* XMLDeserialiser::GetNextElem(SerialiseElement* elem)
	{
		return elem->NextSiblingElement();
	}

	inline const BH::Char * XMLDeserialiser::GetElemName(const SerialiseElement* elem)
	{
		return elem->Name();
	}
	// ----------------------------- End Element ------------------------------------

	// --------------------------- Get attributes -----------------------------------
	template<typename T>
	inline bool XMLDeserialiser::GetAttribute(const BH::String & attribute_name, SerialiseElement* elem, T & value)
	{
		BH_STATIC_ASSERT(false, "XMLDeserialiser::GetAttribute - Type not available for deserialise");
		return false;
	}

	template<>
	inline bool XMLDeserialiser::GetAttribute(const BH::String & attribute_name, SerialiseElement* elem, bool & value)
	{
		tinyxml2::XMLError error = elem ? elem->QueryBoolAttribute(attribute_name.c_str(), &value) : tinyxml2::XMLError::XML_NO_ATTRIBUTE;
		return (error != tinyxml2::XMLError::XML_NO_ATTRIBUTE) && (error != tinyxml2::XMLError::XML_WRONG_ATTRIBUTE_TYPE);
	}

	template<>
	inline bool XMLDeserialiser::GetAttribute(const BH::String & attribute_name, SerialiseElement* elem, BH::s32 & value)
	{
		tinyxml2::XMLError error = elem ? elem->QueryIntAttribute(attribute_name.c_str(), &value) : tinyxml2::XMLError::XML_NO_ATTRIBUTE;
		return (error != tinyxml2::XMLError::XML_NO_ATTRIBUTE) && (error != tinyxml2::XMLError::XML_WRONG_ATTRIBUTE_TYPE);
	}

	template<>
	inline bool XMLDeserialiser::GetAttribute(const BH::String & attribute_name, SerialiseElement* elem, BH::u32 & value)
	{
		tinyxml2::XMLError error = elem ? elem->QueryUnsignedAttribute(attribute_name.c_str(), &value) : tinyxml2::XMLError::XML_NO_ATTRIBUTE;
		return (error != tinyxml2::XMLError::XML_NO_ATTRIBUTE) && (error != tinyxml2::XMLError::XML_WRONG_ATTRIBUTE_TYPE);
	}

	template<>
	inline bool XMLDeserialiser::GetAttribute(const BH::String & attribute_name, SerialiseElement* elem, BH::f32 & value)
	{
		tinyxml2::XMLError error = elem ? elem->QueryFloatAttribute(attribute_name.c_str(), &value) : tinyxml2::XMLError::XML_NO_ATTRIBUTE;
		return (error != tinyxml2::XMLError::XML_NO_ATTRIBUTE) && (error != tinyxml2::XMLError::XML_WRONG_ATTRIBUTE_TYPE);
	}

	template<>
	inline bool XMLDeserialiser::GetAttribute(const BH::String & attribute_name, SerialiseElement* elem, BH::f64 & value)
	{
		tinyxml2::XMLError error = elem ? elem->QueryDoubleAttribute(attribute_name.c_str(), &value) : tinyxml2::XMLError::XML_NO_ATTRIBUTE;
		return (error != tinyxml2::XMLError::XML_NO_ATTRIBUTE) && (error != tinyxml2::XMLError::XML_WRONG_ATTRIBUTE_TYPE);
	}

	template<>
	inline bool XMLDeserialiser::GetAttribute(const BH::String & attribute_name, SerialiseElement* elem, BH::String & value)
	{
		const BH::Char * str = elem ? elem->Attribute(attribute_name.c_str()) : nullptr;
		if (str)
			value = str;

		return str != nullptr;
	}
	// --------------------------- End attributes -----------------------------------
}