// Constant hashed string object.
namespace BH
{
	inline CName::CName(const Char* txt)
	: Text(txt)
	, Hash(txt ? BH::Hash::Generate(txt) : 0) {}

	inline CName::CName(const BH::Hash& hash)
	: Text( hash.GetText().c_str() )
	, Hash(hash.GetHash()) {}

	inline CName::CName(const Char* txt, HashValue hash)
	: Text(txt)
	, Hash(hash) {}

	inline CName::CName(HashValue hash)
	: Text(nullptr)
	, Hash(hash) {}

	inline CName & CName::operator = (const Char* txt)
	{
		Text = txt;
		Hash = BH::Hash::Generate(Text);
		return *this;
	}

	inline CName & CName::operator = (const BH::Hash& hash)
	{
		Text = hash.GetText().c_str();
		Hash = hash.GetHash();
		return *this;
	}

	inline bool CName::operator == (const CName& rhs) const
	{
		return Hash == rhs.Hash;
	}

	inline bool CName::operator < (const CName& rhs) const
	{
		return Hash < rhs.Hash;
	}

	inline CName::operator BH::Hash() const
	{
		return BH::Hash(Text);
	}
}
