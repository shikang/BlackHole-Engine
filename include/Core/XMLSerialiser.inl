// Implementation
namespace BH
{
	inline bool XMLSerialiser::SaveFile(const BH::String & filename, SerialiseDoc & doc)
	{
		tinyxml2::XMLError err = doc.SaveFile(filename.c_str());
		return err == tinyxml2::XMLError::XML_NO_ERROR || err == tinyxml2::XMLError::XML_ERROR_FILE_NOT_FOUND;
	}

	inline void XMLSerialiser::PrintDoc(SerialiseDoc & doc)
	{
		doc.Print();
	}

	// ------------------------------ Set Elem --------------------------------------
	inline SerialiseElement * XMLSerialiser::PushElem(const BH::String & tag_name, SerialiseDoc & doc)
	{
		return doc.InsertEndChild( doc.NewElement( tag_name.c_str() ) )->ToElement();
	}

	inline SerialiseElement * XMLSerialiser::PushElem(const BH::String & tag_name, SerialiseElement * elem, SerialiseDoc & doc)
	{
		return elem->InsertEndChild( doc.NewElement( tag_name.c_str() ) )->ToElement();
	}

	// ----------------------------- Remove Elem ------------------------------------
	// @todo : to be done if needed
	// ------------------------------ End Elem --------------------------------------

	template<typename T>
	inline void XMLSerialiser::SetAttribute(const BH::String & attribute_name, T attribute, SerialiseElement * elem)
	{
		elem->SetAttribute(attribute_name.c_str(), attribute);
	}

	template<>
	inline void XMLSerialiser::SetAttribute(const BH::String & attribute_name, BH::String attribute, SerialiseElement * elem)
	{
		elem->SetAttribute(attribute_name.c_str(), attribute.c_str());
	}

	// ------------------------------ Set Text --------------------------------------
	template<typename T>
	inline void XMLSerialiser::SetText(SerialiseElement* elem, T text)
	{
		elem->SetText(text);
	}
}