#pragma once


#define SWCASE_ENUM_MACRO(name) case name: return #name
#define SWCASE_ENUM_MACRO_INV(str,name) if(strcmp(str,#name)==0) return name

enum eValueType
{
	//For ValueList
	VALUE_TYPE_UNKNOWN,
	VALUE_TYPE_BYTE,
	VALUE_TYPE_BOOL,
	VALUE_TYPE_SHORT,
	VALUE_TYPE_INT,
	VALUE_TYPE_FLOAT,
	VALUE_TYPE_COLORF,
	VALUE_TYPE_VEC2F,
	VALUE_TYPE_VEC3F,
	VALUE_TYPE_VEC4F,
	VALUE_TYPE_MATRIX44,
	VALUE_TYPE_TEXTURE_ID,
	VALUE_TYPE_STRING_ARRAY,
	VALUE_TYPE_FLOAT_ARRAY,
	VALUE_TYPE_INT_ARRAY,
	VALUE_TYPE_VEC2F_ARRAY,
	VALUE_TYPE_VEC3F_ARRAY,
	VALUE_TYPE_VEC4F_ARRAY,

	//OtherTypes
	VALUE_TYPE_DOUBLE,
	VALUE_TYPE_COLORB,
	VALUE_TYPE_QUATERNION,
	VALUE_TYPE_UINT,
	VALUE_TYPE_USHORT,
	VALUE_TYPE_CHAR,

};

inline const char* ToStrValueType(eValueType type)
{
	switch(type)
	{
	SWCASE_ENUM_MACRO(VALUE_TYPE_UNKNOWN);
	SWCASE_ENUM_MACRO(VALUE_TYPE_BYTE);
	SWCASE_ENUM_MACRO(VALUE_TYPE_BOOL);
	SWCASE_ENUM_MACRO(VALUE_TYPE_SHORT);
	SWCASE_ENUM_MACRO(VALUE_TYPE_INT);
	SWCASE_ENUM_MACRO(VALUE_TYPE_FLOAT);
	SWCASE_ENUM_MACRO(VALUE_TYPE_COLORF);
	SWCASE_ENUM_MACRO(VALUE_TYPE_VEC2F);
	SWCASE_ENUM_MACRO(VALUE_TYPE_VEC3F);
	SWCASE_ENUM_MACRO(VALUE_TYPE_VEC4F);
	SWCASE_ENUM_MACRO(VALUE_TYPE_MATRIX44);
	SWCASE_ENUM_MACRO(VALUE_TYPE_TEXTURE_ID);
	SWCASE_ENUM_MACRO(VALUE_TYPE_STRING_ARRAY);
	SWCASE_ENUM_MACRO(VALUE_TYPE_FLOAT_ARRAY);
	SWCASE_ENUM_MACRO(VALUE_TYPE_INT_ARRAY);
	SWCASE_ENUM_MACRO(VALUE_TYPE_VEC2F_ARRAY);
	SWCASE_ENUM_MACRO(VALUE_TYPE_VEC3F_ARRAY);
	SWCASE_ENUM_MACRO(VALUE_TYPE_VEC4F_ARRAY);

	SWCASE_ENUM_MACRO(VALUE_TYPE_DOUBLE);
	SWCASE_ENUM_MACRO(VALUE_TYPE_COLORB);
	SWCASE_ENUM_MACRO(VALUE_TYPE_QUATERNION);
	SWCASE_ENUM_MACRO(VALUE_TYPE_UINT);
	SWCASE_ENUM_MACRO(VALUE_TYPE_USHORT);
	SWCASE_ENUM_MACRO(VALUE_TYPE_CHAR);
	};
	return "VALUE_TYPE_UNKNOWN";
};

inline eValueType FromStrValueType(const char* str)
{
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_UNKNOWN);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_BYTE);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_BOOL);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_SHORT);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_INT);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_FLOAT);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_COLORF);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_VEC2F);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_VEC3F);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_VEC4F);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_MATRIX44);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_TEXTURE_ID);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_STRING_ARRAY);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_FLOAT_ARRAY);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_INT_ARRAY);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_VEC2F_ARRAY);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_VEC3F_ARRAY);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_VEC4F_ARRAY);

	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_DOUBLE);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_COLORB);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_QUATERNION);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_UINT);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_USHORT);
	SWCASE_ENUM_MACRO_INV(str,VALUE_TYPE_CHAR);
	return VALUE_TYPE_UNKNOWN;
}

inline int getValueSize(eValueType type)
{
	switch(type)
	{
	case VALUE_TYPE_DOUBLE: return 16;
	case VALUE_TYPE_FLOAT: return 4;
	case VALUE_TYPE_VEC2F: return 8;
	case VALUE_TYPE_VEC3F: return 12;
	case VALUE_TYPE_VEC4F: return 16;
	case VALUE_TYPE_MATRIX44: return 64;
	case VALUE_TYPE_QUATERNION: return 16;
	case VALUE_TYPE_COLORF: return 16;
	case VALUE_TYPE_COLORB: return 4;
	case VALUE_TYPE_TEXTURE_ID: return 4;
	case VALUE_TYPE_BOOL: return 4;
	case VALUE_TYPE_INT: return 4;
	case VALUE_TYPE_UINT: return 4;
	case VALUE_TYPE_SHORT: return 2;
	case VALUE_TYPE_USHORT: return 2;
	case VALUE_TYPE_CHAR: return 1;
	case VALUE_TYPE_BYTE: return 1;
	case VALUE_TYPE_STRING_ARRAY: return -1;
	};
	return -1;
};

union uValueData
{
	RCID TextureId;
	BYTE Byte;
	bool Bool;
	SHORT Short;
	int Int;
	
	float Float;
	char* pString;
	float Color[4];
	float Vector[4];
	float* pMatrix44;
	float* FloatArray;
	int* IntArray;
};

#define MAX_VALUE_NAME_SIZE 32


class GTexture;

class ValueData : public GSerializable
{
public:
	ValueData(bool Val)			{set(Val);};
	ValueData(int Val)			{set(Val);};
	ValueData(SHORT Val)		{set(Val);};
	ValueData(BYTE Val)			{set(Val);};
	ValueData(float Val)		{set(Val);};
	ValueData(Vec2& Val)		{set(Val);};
	ValueData(Vec3& Val)		{set(Val);};
	ValueData(Vec4& Val)		{set(Val);};
	ValueData(Matrix44& Val)	{set(Val);};
	ValueData(ColorF& Val)		{set(Val);};
	ValueData(GTexture* Val)	{set(Val);};
	ValueData(const ValueData& other)	{set(other);};

	ValueData() {init_zero();};
	ValueData(eValueType type, uValueData data, UINT count=0)	{set(type,data,count);};
	ValueData(eValueType type, void* pData, UINT count=0)		{set(type,pData,count);};

	virtual ~ValueData()
	{
		destroy();
	};

	ValueData& operator=(const ValueData& other)
	{
		destroy();
		this->type=other.type;
		this->count=other.count;
		set(&(other.data));
		return *this;
	};

	void set(bool Val)			{set(VALUE_TYPE_BOOL,(void*)&Val);};
	void set(int Val)			{set(VALUE_TYPE_INT,(void*)&Val);};
	void set(SHORT Val)			{set(VALUE_TYPE_SHORT,(void*)&Val);};
	void set(BYTE Val)			{set(VALUE_TYPE_BYTE,(void*)&Val);};
	void set(float Val)			{set(VALUE_TYPE_FLOAT,(void*)&Val);};
	void set(Vec2& Val)			{set(VALUE_TYPE_VEC2F,(void*)&Val);};
	void set(Vec3& Val)			{set(VALUE_TYPE_VEC3F,(void*)&Val);};
	void set(Vec4& Val)			{set(VALUE_TYPE_VEC4F,(void*)&Val);};
	void set(Matrix44& Val)		{set(VALUE_TYPE_MATRIX44,(void*)&Val);};
	void set(ColorF& Val)		{set(VALUE_TYPE_COLORF,(void*)&Val);};
	void set(GTexture* Val);
	void set(const ValueData& val)	{set(val.type,val.data,val.count);};

	void init_zero()
	{
		memset(&data, 0, sizeof(data));
		type = VALUE_TYPE_UNKNOWN;
		count=0;
	};

	void destroy()
	{
		if(data.pString==NULL)
			return;

		if (type == VALUE_TYPE_STRING_ARRAY)
			delete [] data.pString;
		else if (type == VALUE_TYPE_MATRIX44)
			delete [] data.pMatrix44;
		else if (type == VALUE_TYPE_FLOAT_ARRAY)
			delete [] data.FloatArray;
		else if (type == VALUE_TYPE_VEC2F_ARRAY)
			delete [] data.FloatArray;
		else if (type == VALUE_TYPE_VEC3F_ARRAY)
			delete [] data.FloatArray;
		else if (type == VALUE_TYPE_VEC4F_ARRAY)
			delete [] data.FloatArray;
		else if (type == VALUE_TYPE_INT_ARRAY)
			delete [] data.IntArray;
		memset(&data, 0, sizeof(data));
		count=0;
		init_zero();
	};

	void set(eValueType type, uValueData data, UINT count=0)
	{
		destroy();
		
		this->type=type;
		this->count=count;
		set(&data);
	};

	void set(eValueType type, void* pData, UINT count=0)
	{
		destroy();

		this->type=type;
		this->count=count;

		set((uValueData*)pData);
	};

	void set(const uValueData* otherVal)
	{
		destroy();
		if(!otherVal)
		{
			return;
		};

		if(type==VALUE_TYPE_STRING_ARRAY)
		{
            otherVal->pString ?	count=strlen(otherVal->pString)+1 :
                    count = 1;
			data.pString=new char[count];
			otherVal->pString ? 
				memcpy(data.pString, otherVal->pString,count*sizeof(char)) : 
                memset(data.pString, 0,count*sizeof(char));
		}
		else if(type==VALUE_TYPE_MATRIX44)
		{
			data.pMatrix44=new float[16];
			otherVal->pMatrix44 ? 
				memcpy(data.pMatrix44, otherVal->pMatrix44,16*sizeof(float)) : 
				memset(data.pMatrix44, 0,16*sizeof(float));
		}
		else if(type==VALUE_TYPE_INT_ARRAY)
		{
			data.IntArray=new int[count];
			otherVal->IntArray ? 
				memcpy(data.IntArray, otherVal->IntArray,count*sizeof(int)) : 
				memset(data.IntArray, 0,count*sizeof(int));
		}
		else if(type==VALUE_TYPE_FLOAT_ARRAY)
		{
			data.FloatArray=new float[count];
			otherVal->FloatArray ?
				memcpy(data.FloatArray, otherVal->FloatArray,count*sizeof(float)) :
				memset(data.FloatArray, 0,count*sizeof(float));
		}
		else if(type==VALUE_TYPE_VEC2F_ARRAY)
		{
			data.FloatArray=new float[count*2];
			otherVal->FloatArray ?
				memcpy(data.FloatArray, otherVal->FloatArray,count*2*sizeof(float)) :
				memset(data.FloatArray, 0,count*2*sizeof(float));
		}
		else if(type==VALUE_TYPE_VEC3F_ARRAY)
		{
			data.FloatArray=new float[count*3];
			otherVal->FloatArray ?
				memcpy(data.FloatArray, otherVal->FloatArray,count*3*sizeof(float)) :
				memset(data.FloatArray, 0,count*3*sizeof(float));
		}
		else if(type==VALUE_TYPE_VEC4F_ARRAY)
		{
			data.FloatArray=new float[count*4];
			otherVal->FloatArray ?
				memcpy(data.FloatArray, otherVal->FloatArray,count*4*sizeof(float)) :
				memset(data.FloatArray, 0,count*4*sizeof(float));
		}
		else data=*otherVal;
	};

	std::string toStringValue();
	bool fromStringValue(eValueType type, const char* str);

	std::string toStringType()
	{
		return ToStrValueType(type);
	};

	bool Serialize(pugi::xml_node& node, bool bStore);


	eValueType type;
	uValueData data;
	UINT count;
};

class Value : public GSerializable
{

public:

	Value(const char* Name,bool Val)			{setName(Name); data.set(Val);};
	Value(const char* Name, int Val)			{setName(Name); data.set(Val);};
	Value(const char* Name, SHORT Val)			{setName(Name); data.set(Val);};
	Value(const char* Name, BYTE Val)			{setName(Name); data.set(Val);};
	Value(const char* Name, float Val)			{setName(Name); data.set(Val);};
	Value(const char* Name, Vec2& Val)			{setName(Name); data.set(Val);};
	Value(const char* Name, Vec3& Val)			{setName(Name); data.set(Val);};
	Value(const char* Name, Vec4& Val)			{setName(Name); data.set(Val);};
	Value(const char* Name, Matrix44& Val)		{setName(Name); data.set(Val);};
	Value(const char* Name, ColorF& Val)		{setName(Name); data.set(Val);};
	Value(const char* Name, GTexture* Val)		{setName(Name); data.set(Val);};
	Value(const char* Name, ValueData& Val)		{setName(Name); data.set(Val);};

	Value()
	{
		init_zero();
	};

	Value(const char* name, eValueType type, uValueData data, UINT count=0)
	{
		setValue(name,type,data,count);
	};

	Value(const char* name, eValueType type, void* pData, UINT count=0)
	{
		setValue(name,type,pData,count);
	};

	Value(const Value& other)
	{
		init_zero();
		memcpy(name,other.name,MAX_VALUE_NAME_SIZE);
		data=other.data;
	};

	virtual ~Value()
	{
		destroy();
	};

	Value& operator=(const Value& other)
	{
		destroy();
		memcpy(name,other.name,MAX_VALUE_NAME_SIZE);
		data=other.data;
		return *this;
	};

	void destroy()
	{
		data.destroy();
		init_zero();
	};

	void init_zero()
	{
		memset(&name, 0, MAX_VALUE_NAME_SIZE);
		data.init_zero();
		pData=0;
	};

	void setName(const char* name)
	{
		int nsize=MIN(MAX_VALUE_NAME_SIZE,strlen(name)+1);
		memcpy(this->name,name,nsize);
        this->name[nsize-1]=0;
	};

	void setValue(const char* name, eValueType type, uValueData data, UINT count=0)
	{
		destroy();
		
		setName(name);
		this->data.set(type,data,count);
	};

	void setValue(const char* name, eValueType type, void* pData, UINT count=0)
	{
		destroy();

		setName(name);
		this->data.set(type,pData,count);
	};

	eValueType getType()
	{
		return data.type;
	};

	uValueData& getData()
	{
		return data.data;
	};

	//serialize
	bool Serialize(pugi::xml_node& node, bool bStore);

	char name[MAX_VALUE_NAME_SIZE];
	ValueData data;
	void* pData;
};

typedef std::multimap<UINT,Value> ValueMapType;
class GTexture;

class ValueList : public GSerializable
{
public:
	ValueList(void);
	ValueList(ValueList& other);
	~ValueList(void);

	void add(ValueList& other);

	ValueList& operator=(ValueList& other);

	UINT CalcHash(const char* name);

	void setValue(const char* Name, bool Val)		{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, int Val)		{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, SHORT Val)		{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, BYTE Val)		{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, float Val)		{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, Vec2& Val)		{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, Vec3& Val)		{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, Vec4& Val)		{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, Matrix44& Val)	{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, ColorF& Val)	{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, GTexture* Val)	{setValue(Name,Value(Name,Val));};
	void setValue(const char* Name, Value& Val);
	void setValue(const char* Name, ValueData& Val)	{setValue(Name,Value(Name,Val));};
	void setValue(Value& val) {setValue(val.name,val);};

	bool getBool(const char* Name)				{return getValue(Name,VALUE_TYPE_BOOL).data.data.Bool;};
	int getInt(const char* Name)				{return getValue(Name,VALUE_TYPE_INT).data.data.Int;};
	SHORT getShort(const char* Name)			{return getValue(Name,VALUE_TYPE_SHORT).data.data.Short;};
	BYTE getByte(const char* Name)				{return getValue(Name,VALUE_TYPE_BYTE).data.data.Byte;};
	float getFloat(const char* Name)			{return getValue(Name,VALUE_TYPE_FLOAT).data.data.Float;};
	Vec2 getVec2(const char* Name)				{return *(Vec2*)getValue(Name,VALUE_TYPE_VEC2F).data.data.Vector;};
	Vec3 getVec3(const char* Name)				{return *(Vec3*)getValue(Name,VALUE_TYPE_VEC2F).data.data.Vector;};
	Vec4 getVec4(const char* Name)				{return *(Vec4*)getValue(Name,VALUE_TYPE_VEC2F).data.data.Vector;};
	Matrix44 getMatrix44(const char* Name)		{return *(Matrix44*)getValue(Name,VALUE_TYPE_VEC2F).data.data.pMatrix44;};
	ColorF getColorF(const char* Name)			{return *(ColorF*)getValue(Name,VALUE_TYPE_VEC2F).data.data.Color;};
	RCID getTextureID(const char* Name)			{return getValue(Name,VALUE_TYPE_TEXTURE_ID).data.data.TextureId;};
	GTexture* getTexture(const char* Name);

	Value& getValue(const char* Name,eValueType type, int arrSize=0);
	Value* getExistsValue(const char* Name);

	Value* find(const char* name);
	Value* find(UINT hash);
	void clear();

	//serialize
	bool Serialize(pugi::xml_node& node, bool bStore);


	ValueMapType value_array;
};

