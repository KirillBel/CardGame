#pragma once

//enum eValueType
//{
//	VALUE_BAD =-1,
//	VALUE_DOUBLE = 0,
//	VALUE_FLOAT,
//	VALUE_VEC2,
//	VALUE_VEC3,
//	VALUE_VEC4,
//	VALUE_MATRIX44,
//	VALUE_QUATERNION,
//	VALUE_COLORF,
//	VALUE_COLORB,
//
//	VALUE_BOOL,
//	VALUE_INT,
//	VALUE_UINT,
//	VALUE_SHORT,
//	VALUE_USHORT,
//	VALUE_CHAR,
//	VALUE_BYTE,
//
//	VALUE_STRING,
//	VALUE_DATA,
//
//	VALUE_TEXID,
//};
//
//inline int getValueSize(eValueType type)
//{
//	switch(type)
//	{
//	case VALUE_DOUBLE: return 16;
//	case VALUE_FLOAT: return 4;
//	case VALUE_VEC2: return 8;
//	case VALUE_VEC3: return 12;
//	case VALUE_VEC4: return 16;
//	case VALUE_MATRIX44: return 64;
//	case VALUE_QUATERNION: return 16;
//	case VALUE_COLORF: return 16;
//	case VALUE_COLORB: return 4;
//	case VALUE_TEXID: return 4;
//	case VALUE_BOOL: return 4;
//	case VALUE_INT: return 4;
//	case VALUE_UINT: return 4;
//	case VALUE_SHORT: return 2;
//	case VALUE_USHORT: return 2;
//	case VALUE_CHAR: return 1;
//	case VALUE_BYTE: return 1;
//	case VALUE_STRING: return -1;
//	case VALUE_DATA: return -1;
//	};
//	return -1;
//};
//
//struct sValue
//{
//	sValue(const char* name, eValueType type) : bAssertIfWrongType(0)
//	{
//		int valSize=getValueSize(type);
//		_ASSERT(valSize!=-1);
//
//		this->name=name;
//		this->size=valSize;
//		this->type=type;
//		this->revision=0;
//		this->pData=new BYTE[valSize];
//		pUserData=0;
//		memset(this->pData,0,valSize);
//	};
//
//	sValue(const char* name, eValueType type, void* pData) : bAssertIfWrongType(0)
//	{
//		int valSize=getValueSize(type);
//		_ASSERT(valSize!=-1);
//
//		this->name=name;
//		this->size=valSize;
//		this->type=type;
//		this->revision=0;
//		this->pData=new BYTE[valSize];
//		pUserData=0;
//		memcpy(this->pData,pData,valSize);
//	};
//
//	sValue(const char* name, void* pData, int size) : bAssertIfWrongType(0)
//	{
//		_ASSERT(size>0);
//
//		this->name=name;
//		this->size=size;
//		this->type=VALUE_DATA;
//		this->revision=0;
//		this->pData=new BYTE[size];
//		pUserData=0;
//		memcpy(this->pData,pData,size);
//	};
//
//	sValue(const char* name, const char* pData) : bAssertIfWrongType(0)
//	{
//		int valSize=strlen(pData)+1;
//		_ASSERT(valSize!=-1);
//
//		this->name=name;
//		this->size=valSize;
//		this->type=VALUE_STRING;
//		this->revision=0;
//		this->pData=new BYTE[valSize];
//		pUserData=0;
//		memcpy(this->pData,pData,valSize);
//		((char*)this->pData)[valSize-1]=0;
//	};
//
//	sValue(const sValue& other)
//	{
//		this->pData=0;
//		this->size=0;
//		(*this)=other;
//	};
//
//	~sValue()
//	{
//		if(pData) 
//		{
//			delete [] pData;
//			pData=0;
//		};
//	};
//
//	sValue& operator = (sValue const& other)
//	{
//		name=other.name;
//		if(size!=other.size)
//		{
//			size=other.size;
//			if(pData)
//			{
//				delete [] pData;
//				pData=0;
//			};
//			pData=new char[size];
//		};
//		
//		memcpy(pData,other.pData,size);
//		type=other.type;
//		revision=other.revision;
//		pUserData=other.pUserData;
//		return *this;
//	};
//
//	bool operator == (sValue& other)
//	{
//		if((name!=other.name) || (type!=other.type) || (revision!=other.revision) || (size!=other.size)) return false;
//		if(memcmp(pData,other.pData,size)!=0) return false;
//		return true;
//	}
//
//	bool operator != (sValue& other)
//	{
//		return !((*this)==other);
//	}
//
//	std::string getName() {return name;};
//	eValueType getType() {return type;}
//	UINT getSize() {return size;};
//	UINT getRevision() {return revision;};
//	void setRevision(UINT val)
//	{
//		revision=val;
//	};
//
//	double getAsDouble()	{return (type==VALUE_DOUBLE) ? *(double*)pData : 0.0f;};
//	float getAsFloat()		{return (type==VALUE_FLOAT) ? *(float*)pData : 0.0f;};
//	Vec2 getAsVec2()		{return (type==VALUE_VEC2) ? *(Vec2*)pData : Vec2();};
//	Vec3 getAsVec3()		{return (type==VALUE_VEC3) ? *(Vec3*)pData : Vec3();};
//	Vec4 getAsVec4()		{return (type==VALUE_VEC4) ? *(Vec4*)pData : Vec4();};
//	Matrix44 getAsMatrix44(){return (type==VALUE_MATRIX44) ? *(Matrix44*)pData : Matrix44();};
//	Quat getAsQuat()		{return (type==VALUE_QUATERNION) ? *(Quat*)pData : Quat();};
//	ColorF getAsColorF()		{return (type==VALUE_COLORF) ? *(ColorF*)pData : ColorF();};
//	DWORD getAsColorB()		{return (type==VALUE_COLORB) ? *(DWORD*)pData : 0;};
//	bool getAsBool()		{return (type==VALUE_BOOL) ? *(bool*)pData : false;};
//	int getAsInt()			{return (type==VALUE_INT) ? *(int*)pData : 0;};
//	UINT getAsUInt()		{return (type==VALUE_UINT) ? *(UINT*)pData : 0;};
//	SHORT getAsShort()		{return (type==VALUE_SHORT) ? *(SHORT*)pData : 0;};
//	USHORT getAsUShort()	{return (type==VALUE_USHORT) ? *(USHORT*)pData : 0;};
//	char getAsChar()		{return (type==VALUE_CHAR) ? *(char*)pData : 0;};
//	BYTE getAsByte()		{return (type==VALUE_BYTE) ? *(BYTE*)pData : 0;};
//	std::string getAsString() {return (type==VALUE_STRING) ? (char*)pData : "";};
//	void* getAsData()		{return (type==VALUE_DATA) ? pData : 0;};
//	int getAsTextureId()	{return (type==VALUE_TEXID) ? *(int*)pData : -1;};
//
//	void setValue(bool Val)			{setValue(VALUE_BOOL,(void*)&Val);};
//	void setValue(int Val)			{setValue(VALUE_INT,(void*)&Val);};
//	void setValue(UINT Val)			{setValue(VALUE_UINT,(void*)&Val);};
//	void setValue(SHORT Val)		{setValue(VALUE_SHORT,(void*)&Val);};
//	void setValue(USHORT Val)		{setValue(VALUE_USHORT,(void*)&Val);};
//	void setValue(char Val)			{setValue(VALUE_CHAR,(void*)&Val);};
//	void setValue(BYTE Val)			{setValue(VALUE_BYTE,(void*)&Val);};
//
//	void setValue(double Val)		{setValue(VALUE_DOUBLE,(void*)&Val);};
//	void setValue(float Val)		{setValue(VALUE_FLOAT,(void*)&Val);};
//	void setValue(Vec2& Val)		{setValue(VALUE_VEC2,(void*)&Val);};
//	void setValue(Vec3& Val)		{setValue(VALUE_VEC3,(void*)&Val);};
//	void setValue(Vec4& Val)		{setValue(VALUE_VEC4,(void*)&Val);};
//	void setValue(Matrix44& Val)	{setValue(VALUE_MATRIX44,(void*)&Val);};
//	void setValue(Quat& Val)		{setValue(VALUE_QUATERNION,(void*)&Val);};
//	void setValue(ColorF& Val)		{setValue(VALUE_COLORF,(void*)&Val);};
//	void setValue(ColorB& Val)		{setValue(VALUE_COLORB,(void*)&Val);};
//	void setTextureId(int Val)		{setValue(VALUE_TEXID,(void*)&Val);};
//
//	void setValue(const char* Val)		{setValue(sValue(name.c_str(),Val));};
//	void setValue(void* Val, int size)	{setValue(sValue(name.c_str(),Val,size));};
//
//	void setValue(sValue& Val)
//	{
//		if(bAssertIfWrongType)
//			_ASSERT(this->type==Val.type);
//		(*this)=Val;
//	};
//
//	void setValue(eValueType type, void* data)
//	{
//		if(bAssertIfWrongType)
//			_ASSERT(this->type==type);
//		
//		int valSize=getValueSize(type);
//		_ASSERT(valSize!=-1);
//
//		if(valSize!=size)
//		{
//			this->size=valSize;
//			if(pData)
//			{
//				delete [] pData;
//				pData=0;
//			};
//			this->pData=new BYTE[valSize];
//			
//			memcpy(this->pData,data,valSize);
//			this->revision++;
//		}
//		else if(memcmp(this->pData,data,valSize)!=0)
//		{
//			memcpy(this->pData,data,valSize);
//			this->revision++;
//		};
//	};
//
//	bool bAssertIfWrongType;
//	void* pUserData;
//private:
//	std::string name;
//	eValueType type;
//	void* pData;
//	UINT size;
//	UINT revision;
//	
//};
//
//class Values
//{
//public:
//	Values(void)
//	{
//		valueIter=values.end();
//	};
//
//	~Values(void)
//	{
//		clear();
//	};
//
//	void setValue(const char* Name, bool Val)		{setValue(sValue(Name,VALUE_BOOL,(void*)&Val));};
//	void setValue(const char* Name, int Val)		{setValue(sValue(Name,VALUE_INT,(void*)&Val));};
//	void setValue(const char* Name, UINT Val)		{setValue(sValue(Name,VALUE_UINT,(void*)&Val));};
//	void setValue(const char* Name, SHORT Val)		{setValue(sValue(Name,VALUE_SHORT,(void*)&Val));};
//	void setValue(const char* Name, USHORT Val)		{setValue(sValue(Name,VALUE_USHORT,(void*)&Val));};
//	void setValue(const char* Name, char Val)		{setValue(sValue(Name,VALUE_CHAR,(void*)&Val));};
//	void setValue(const char* Name, BYTE Val)		{setValue(sValue(Name,VALUE_BYTE,(void*)&Val));};
//	void setValue(const char* Name, float Val)		{setValue(sValue(Name,VALUE_FLOAT,(void*)&Val));};
//	void setValue(const char* Name, Vec2& Val)		{setValue(sValue(Name,VALUE_VEC2,(void*)&Val));};
//	void setValue(const char* Name, Vec3& Val)		{setValue(sValue(Name,VALUE_VEC3,(void*)&Val));};
//	void setValue(const char* Name, Vec4& Val)		{setValue(sValue(Name,VALUE_VEC4,(void*)&Val));};
//	void setValue(const char* Name, Matrix44& Val)	{setValue(sValue(Name,VALUE_MATRIX44,(void*)&Val));};
//	void setValue(const char* Name, Quat& Val)		{setValue(sValue(Name,VALUE_QUATERNION,(void*)&Val));};
//	void setValue(const char* Name, ColorF& Val)	{setValue(sValue(Name,VALUE_COLORF,(void*)&Val));};
//	void setTexture(const char* Name, int texId)	{setValue(sValue(Name,VALUE_TEXID,(void*)&texId));};
//
//	void setValue(const char* Name, const char* Val)		{setValue(sValue(Name,Val));};
//	void setValue(const char* Name, void* Val, int size)	{setValue(sValue(Name,Val,size));};
//
//	double getDouble(const char* Name)		{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_DOUBLE)); return pVal->getAsDouble();};
//	float getFloat(const char* Name)		{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_FLOAT)); return pVal->getAsFloat();};
//	Vec2 getVec2(const char* Name)			{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_VEC2)); return pVal->getAsVec2();};
//	Vec3 getVec3(const char* Name)			{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_VEC3)); return pVal->getAsVec3();};
//	Vec4 getVec4(const char* Name)			{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_VEC4)); return pVal->getAsVec4();};
//	Matrix44 getMatrix44(const char* Name)	{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_MATRIX44)); return pVal->getAsMatrix44();};
//	Quat getQuat(const char* Name)			{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_QUATERNION)); return pVal->getAsQuat();};
//	ColorF getColorF(const char* Name)		{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_COLORF)); return pVal->getAsColorF();};
//
//	int getInt(const char* Name)			{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_INT)); return pVal->getAsInt();};
//	UINT getUInt(const char* Name)			{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_UINT)); return pVal->getAsUInt();};
//	SHORT getShort(const char* Name)		{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_SHORT)); return pVal->getAsShort();};
//	USHORT getUShort(const char* Name)		{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_USHORT)); return pVal->getAsUShort();};
//	char getChar(const char* Name)			{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_CHAR)); return pVal->getAsChar();};
//	BYTE getByte(const char* Name)			{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_BYTE)); return pVal->getAsByte();};
//	std::string getString(const char* Name) {sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_STRING)); return pVal->getAsString();};
//	void* getData(const char* Name)			{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_DATA)); return pVal->getAsData();};
//	int getTextureId(const char* Name)		{sValue* pVal=findValue(Name); _ASSERT(testVal(pVal,VALUE_TEXID)); return pVal->getAsTextureId();};
//
//	bool testVal(sValue* pVal, eValueType type)
//	{
//		if(pVal==0) return false;
//		if(pVal->getType()!=type) return false;
//		return true;
//	};
//
//	inline void setValue(sValue& val)
//	{
//		sValue* pValue=findValue(val.getName().c_str());
//		if(pValue==0)
//		{
//			val.setRevision(1);
//			values.insert(std::pair<std::string, sValue>(val.getName(), val));
//		}
//		else
//		{
//			if(pValue->getType()!=val.getType()) 
//				LOGWARNING("Another value type (%s): required - %d, current - %d",val.getName().c_str(),(int)pValue->getType(),(int)val.getType());
//			(*pValue)=val;
//			pValue->setRevision(pValue->getRevision()+1);
//		};
//	};
//
//	inline sValue* findValue(const char* Name)
//	{
//		std::map<std::string, sValue>::iterator it=values.find(Name);
//		if(it!=values.end()) return &it->second;
//		return 0;
//	};
//
//	bool hasValue(const char* name)
//	{
//		return findValue(name)!=0;
//	};
//
//	inline void clear()
//	{
//		values.clear();
//	};
//
//	Values& operator = (Values const& other)
//	{
//		values.clear();
//		values=other.values;
//		return *this;
//	}
//
//	bool operator == (Values& other)
//	{
//		if(values.size()!=other.values.size()) return false;
//		if(values.size()==0) return false;
//
//		std::map<std::string, sValue>::iterator it1,it2;
//		for(it1=values.begin(),it2=other.values.begin();it1!=values.end();it1++,it2++)
//		{
//			if(it1->second!=it2->second) return false;
//		};
//		return true;
//	}
//
//	sValue* getFirstValue()
//	{
//		if(values.size()==0) 
//			return 0;
//		valueIter=values.begin();
//		return &valueIter->second;
//	};
//
//	sValue* getNextValue()
//	{
//		if(valueIter==values.end()) 
//			return 0;
//
//		valueIter++;
//		
//		if(valueIter==values.end()) 
//			return 0;
//
//		return &valueIter->second;
//	};
//
//	bool setFirstValueIter()
//	{
//		if(values.size()==0) 
//			return false;
//		valueIter=values.begin();
//		return true;
//	};
//
//	sValue* getNextValueIter()
//	{
//		if(valueIter==values.end()) 
//			return 0;
//
//		sValue* val=&valueIter->second;
//		valueIter++;
//		return val;
//	};
//
//private:
//	std::map<std::string, sValue> values;
//	std::map<std::string, sValue>::iterator valueIter;
//};
//
