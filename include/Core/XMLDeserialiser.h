#ifndef BH_XML_DESERIALISER_H
#define BH_XML_DESERIALISER_H

#include "Core/XMLSerialiseType.h"

#include "Core/CoreEssential.h"
#include "Core/Deserialiser.h"

namespace BH
{
	class XMLDeserialiser : public BH::Deserialiser
	{
	public:
		// Constructor
		XMLDeserialiser();

		// Destructor
		~XMLDeserialiser();

		// Check if the stream has ended.
		bool IsStreamEnded() const;

		// Traverse the stream specified by the path.
		StreamPtr TraverseStream(const BH::Char* path) override;

		// Actual serialise method.
		StreamPtr DoDeserialise(StreamPtr s,						//!< Stream Pointer
								const BH::Type* type,				//!< Type of object
								void *& obj,						//!< Object
								const BH::FieldAttributes* attr,	//!< Attributes
								const BH::Field* field,			//!< Field
								bool inferType = false				//!< If true, get type through deserialisation
								);

	private:
		// Helper Functions
		/*
			Load file with the given doc. (For reading only, not for writing!)
			Did not return doc because not all Doc in parser library support
			copy construct (e.g. tinyxml2)

			return true if succeed
		*/
		static bool LoadFile(const BH::String & filename, SerialiseDoc & doc);

		/*
			Get the first element in a group with the tag name
			If pass in empty string, get the first element

			XML:
			<group>
				<tag_name attribute_name='attribute'/>	<-- an element
				<tag_name attribute_name='attribute'/>
				<tag_name attribute_name='attribute'/>
				<tag_name attribute_name='attribute'/>
			</group>
		*/
		static SerialiseElement* GetElem(const BH::String & tag_name, SerialiseElement* elem);

		/*
			Get a group or the first element

			XML:
			<group>										<-- a group
				<tag_name attribute_name='attribute'/>
				<tag_name attribute_name='attribute'/>
				<tag_name attribute_name='attribute'/>
				<tag_name attribute_name='attribute'/>
			</group>

			OR

			<tag_name attribute_name='attribute'/>		<-- first element
			<tag_name attribute_name='attribute'/>
			<tag_name attribute_name='attribute'/>
			<tag_name attribute_name='attribute'/>

		*/
		static SerialiseElement* GetElem(const BH::String & tag_name, SerialiseDoc & doc);

		/*
			Get the next element (nullptr if end of group)

			XML:
			<tag_name0 attribute_name='attribute'/>		<-- current element
			<tag_name1 attribute_name='attribute'/>		<-- next element
			<tag_name2 attribute_name='attribute'/>
			<tag_name3 attribute_name='attribute'/>
		*/
		static SerialiseElement* GetNextElem(SerialiseElement* elem);

		// Get name of the elem
		static const BH::Char * GetElemName(const SerialiseElement* elem);

		/*
			Get attributes from element. 
			True if data has deserialise successfully

			XML:
			<tag_name attribute_name='attribute'/>		<-- an element
			<tag_name attribute_name='attribute'/>
			<tag_name attribute_name='attribute'/>
			<tag_name attribute_name='attribute'/>
		*/
		template<typename T>
		static bool GetAttribute(const BH::String & attribute_name, SerialiseElement* elem, T & value);

	private:
		// Write value to current node
		bool GetValue(SerialiseElement * s, const BH::Type * type, void* obj, const BH::FieldAttributes * attr, const BH::Field * field);

		// Write value to current node
		SerialiseElement * GetType(StreamPtr s, BH::CName & type_name);

		// Write the container size to current noded
		SerialiseElement * GetContainer(SerialiseElement * s, BH::u32 & size);

	private:
		SerialiseDoc mDoc;
		BH::String mFileName;

		friend class XMLSerialiser;

	};
}

#include "XMLDeserialiser.inl"

#endif	// BH_XML_DESERIALISER_H