#ifndef BH_XML_SERIALISER_H
#define BH_XML_SERIALISER_H

#include "Core/XMLSerialiseType.h"

#include "Core/CoreEssential.h"
#include "Core/Serialiser.h"

namespace BH
{
	class XMLSerialiser : public BH::Serialiser
	{
	public:
		// Constructor
		XMLSerialiser();

		// Destructor
		~XMLSerialiser();

		// Traverse the stream specified by the path.
		StreamPtr TraverseStream(const BH::Char* path) override;

		// Actual serialise method.
		StreamPtr DoSerialise(StreamPtr s,						//!< Stream Pointer
							  const BH::Type* type,			//!< Type of the object
							  const void* obj,					//!< Object
							  const BH::FieldAttributes* attr,	//!< Attributes
							  const BH::Field* field			//!< Field
							  );

	public:
		// Helper Functions

		/*
			After finish parsing, save file to specified filename

			return true if succeed
		*/
		static bool SaveFile(const BH::String & filename, SerialiseDoc & doc);

		// Print doc content.
		static void PrintDoc(SerialiseDoc & doc);

		/*
			Make and push new group

			XML:
				<blah>
					<la lala='lalala'/>
					<ga gaga='gaga'/>
				</blah>
				<tag_name></tag_name>
		*/
		static SerialiseElement * PushElem(const BH::String & tag_name, SerialiseDoc & doc);

		/*
			Make and push new element

			XML:
			<blah>
				<la lala='lalala'/>
				<ga gaga='gaga'/>
			</blah>
			<group_name>
				<tag_name></tag_name>
			</group_name>
		*/
		static SerialiseElement * PushElem(const BH::String & tag_name, SerialiseElement * elem, SerialiseDoc & doc);

		/*
			Remove a group

			XML:
				<blah>
					<la lala='lalala'/>
					<ga gaga='gaga'/>
				</blah>
				<tag_name></tag_name>	<-- will be removed
		*/
		static void RemoveElem(const BH::String & tag_name, SerialiseDoc & doc);

		/*
			Remove element

			XML:
			<blah>
				<la lala='lalala'/>
				<ga gaga='gaga'/>
			</blah>
			<group_name>
				<tag_name></tag_name>	<-- will be removed
			</group_name>
		*/
		static SerialiseElement * RemoveElem(const BH::String & tag_name, SerialiseElement * elem, SerialiseDoc & doc);

		/*
			Set attributes from element.

			XML:
				<tag_name attribute_name='attribute'/>		<-- an element
				<tag_name attribute_name='attribute'/>
				<tag_name attribute_name='attribute'/>
				<tag_name attribute_name='attribute'/>
		*/
		template<typename T>
		static void SetAttribute(const BH::String & attribute_name, T attribute, SerialiseElement * elem);

		template<typename T>
		static void SetText(SerialiseElement* elem, T text);

	private:
		// Write value to current node
		bool SetValue(SerialiseElement * s, const BH::Type * type, const void* obj, const BH::FieldAttributes * attr, const BH::Field * field);

		// Create new node and write the type name
		SerialiseElement * SetType(StreamPtr s, const BH::Type* type);

		// Write the container size to current noded
		SerialiseElement * SetContainer(SerialiseElement * s, BH::u32 size);

	private:
		SerialiseDoc mDoc;
		BH::String mFileName;

		friend class XMLDeserialiser;

	};
}

#include "XMLSerialiser.inl"

#endif	// BH_XML_SERIALISER_H