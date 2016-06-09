// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/TypeRegistrar.h"
#include "Core/EntityManager.h"
#include "Core/Hash.h"
#include "Core/TypeStringifier.h"
#include "Core/Materials.h"

namespace BH
{
	void FileInfoToString(String & str, const Type *, const void * obj, const FieldAttributes *)
	{
		str = reinterpret_cast<const FileInfo*>(obj)->GetFullFile();
	}

	void FileInfoFromString(const String & str, const Type *, void * obj, const FieldAttributes *)
	{
		reinterpret_cast<FileInfo*>(obj)->Set(str.c_str());
	}

	TypeRegistrar TypeRegistrar::sInstance = TypeRegistrar();

	void TypeRegistrar::RegisterTypes()
	{
		//TypeDB& typedb = ENTITY_MANAGER.Types;
		TypeDB& typedb = TypeDB::Instance();

		// Create all basic BH defined types.
		typedb.Create<ObjectComponent>();
		typedb.Create<Object>()
			  .FIELDM(Type, Object)
			  .FIELDM(Name, Object)
			  .FIELDM(Components, Object)
			  .GetField("Type")->Attributes.ReadOnly = 1;
		typedb.Create<GameState>();
		typedb.Create<LevelState>();

		// Vector2
		typedb.Create<Vector2f>()
			.FIELD(x, Vector2f)
			.FIELD(y, Vector2f);

		// Vector3
		typedb.Create<Vector3f>()
			  .FIELD(x, Vector3f)
			  .FIELD(y, Vector3f)
			  .FIELD(z, Vector3f);

		// Vector4
		typedb.Create<Vector4f>()
			  .FIELD(x, Vector4f)
			  .FIELD(y, Vector4f)
			  .FIELD(z, Vector4f)
			  .FIELD(w, Vector4f);

		// Matrix4
		//typedb.Create<Matrix4>()
		//	  .FIELD(_pM, Matrix4);

		// Hash
		typedb.Create<Hash>();
		TYPE_STRINGIFIER.AddTypeStringifier( typedb.Get<Hash>(), &Hash::ToString, &Hash::FromString );

		// FileInfo
		typedb.Create<FileInfo>();
		TYPE_STRINGIFIER.AddTypeStringifier( typedb.Get<FileInfo>(), &FileInfoToString, &FileInfoFromString );

		// Materials
		typedb.Create<Material>()
			  .FIELD( Name, Material );

		typedb.Create<Phong>()
			  .FIELDM( Specular, Phong )
			  .FIELDM( Roughness, Phong )
			  .FIELDM( Texture, Phong )
			  .Base< Material >();
	}

	TypeRegistrar::TypeRegistrar()
	{
		// Automatically register cbl types.
		//RegisterTypes();
	}
}