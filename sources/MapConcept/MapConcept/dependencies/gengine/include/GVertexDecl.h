#pragma once

struct sVertexDeclElem
{
	sVertexDeclElem(int Offset_,Render::eVertexStreamType StreamType_,eValueType ValType_, int usageIndex=0)
	{
		Offset=Offset_;
		StreamType=StreamType_;
		ValType=ValType_;
		this->usageIndex=usageIndex;
	};

	bool operator == (sVertexDeclElem& other) const
	{
		if((Offset!=other.Offset) || 
			(StreamType!=other.StreamType) || 
			(ValType!=other.ValType)) return false;
		return true;
	};

	bool operator != (sVertexDeclElem& other) const
	{
		return !(operator==(other));
	};

	int Offset;
	Render::eVertexStreamType StreamType;
	eValueType ValType;
	int usageIndex;
};

class GVertexDecl
{
public:
	GVertexDecl(void) {clear();};
	virtual ~GVertexDecl(void) {clear();};

	GVertexDecl(sVertexDeclElem* pElems, int Size)
	{
		for(int i=0;i<Size;i++)
		{
			addValue(pElems[i]);
		};
	};

	int getVertexSize() const {return VertexSize;};
	int getNumElements() const {return Elems.size();};
	sVertexDeclElem getValue(int Index) {return Elems[Index];};

	void addValue(sVertexDeclElem Elem)
	{
		Elem.Offset=VertexSize;
		VertexSize+=getValueSize(Elem.ValType);
		Elems.push_back(Elem);
	};
	void addValue(Render::eVertexStreamType StreamType, eValueType ValType, int index=0)
	{
		sVertexDeclElem elem(VertexSize,StreamType,ValType,index);
		VertexSize+=getValueSize(ValType);
		Elems.push_back(elem);
	};

	void clear() {Elems.clear(); VertexSize=0;};

	GVertexDecl& operator = (GVertexDecl& other)
	{
		VertexSize=other.VertexSize;
		Elems=other.Elems;
		return (*this);
	};

	bool operator == (GVertexDecl& other) const
	{
		if(Elems.size()!=other.Elems.size()) return false;
		for(UINT i=0;i<Elems.size();i++)
		{
			if(Elems[i]!=(other.Elems[i])) return false;
		};
		if(VertexSize!=other.VertexSize) return false;
		return true;
	};

	bool operator != (GVertexDecl& other) const
	{
		return !(operator==(other));
	};

	bool getStreamDesc(Render::eVertexStreamType StreamType, UINT& offset, UINT& size)
	{
		for(UINT i=0;i<Elems.size();i++)
		{
			if(Elems[i].StreamType==StreamType)
			{
				offset=Elems[i].Offset;
				size=getValueSize(Elems[i].ValType);
				return true;
			};
		};
		return false;
	};

private:
	int VertexSize;
	std::vector<sVertexDeclElem> Elems;
};

