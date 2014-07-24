#pragma once

struct ICustType
{
	virtual const std::type_info & type() = 0;

	template<typename OtherType>
	OtherType* get()
	{
		//std::string s1=type().name();
		//std::string s2=typeid(OtherType).name();

		size_t tid=type().hash_code();
		size_t other_tid=typeid(OtherType).hash_code();

		if(tid==other_tid)
			return (OtherType*)pData;
		
		LOGWARNING("Wrong property value: need - %s, trying to get - %s",type().name(),typeid(OtherType).name());
		return 0;
	};

	ICustType& operator=(ICustType& other)
	{
		if(type().hash_code()==other.type().hash_code())
		{
			this->eq(&other);
		};
		return *this;
	};

	template<typename Type>
	void set(Type value)
	{
		if(type().hash_code()==typeid(Type).hash_code())
		{
			(*(Type*)pData)=value;
		};
	};

	virtual void eq(ICustType* pOther) = 0;
	virtual ICustType* clone() = 0;
protected:
	int size;
	void* pData;
};

template<typename Type>
struct CustType : public ICustType
{
	CustType(Type val) : data(val)
	{
		pData=&data;
		size=sizeof(data);
	};

	Type data;


	void eq(ICustType* pOther)
	{
		if(type().hash_code()==pOther->type().hash_code())
		{
			CustType<Type>* pVal=(CustType<Type>*)pOther;
			this->data=pVal->data;
			this->size=pVal->size;
		};
	};

	/*CustType<Type>& operator=(CustType<Type>& other)
	{
		this->data=other.data;
		this->size=other.size;
	};*/

	const std::type_info & type()
	{
		return typeid(data);
	};

	ICustType* clone()
	{
		return new CustType<Type>(data);
	};
};

struct sPropertyListEntry
{
	sPropertyListEntry()
	{
		data = 0;
	};

	sPropertyListEntry(const sPropertyListEntry& other)
	{
		data = 0;
		this->operator=(other);
	};

	~sPropertyListEntry()
	{
		release();
	};

	template<typename ValueType>
	void create(ValueType& val)
	{
		data=new CustType<ValueType>(val);
	};

	void release()
	{
		if(data)
		{
			delete data;
			data=0;
		};
	};

	sPropertyListEntry& operator=(const sPropertyListEntry& other)
	{
		release();

		data=other.data->clone();
		return *this;
	};

	ICustType* data;
};

class PropertyList
{
public:
	PropertyList(void);
	~PropertyList(void);

	template<class F>
	int SetValue(const char* name, F data)
	{
		//std::string s1=typeid(F).name();
		sPropertyListEntry entry;
		entry.create<F>(data);
		return regData.insert(name, entry);
	};

	template<class F>
	F GetValue(const char* name)
	{
		NamedMap<sPropertyListEntry>::iterator it=regData.find(name);
		if(it!=regData.end())
		{
			F* pVal=(*it)->data->get<F>();
			if(pVal)
				return *pVal;
			
		};

		LOGWARNING("Failed to get property value \"%s\"!",name);
		return NULL;
	};


	template<class F>
	F GetValueById(UINT id)
	{
		NamedMap<sPropertyListEntry>::iterator it=regData.get(id);
		if(it!=regData.end())
		{
			F* pVal=(*it)->data->get<F>();
			if(pVal)
				return *pVal;
			return NULL;
		};
		return NULL;
	};

	bool loadXML(const char* path);
	bool saveXML(const char* path);

	void release();
	
protected:
	template<typename F>
	void insert_value(UINT id, const char* name, F data)
	{
		regData.insert_by_id(id,name, sPropertyListEntry(data));
	};
private:
	NamedMap<sPropertyListEntry> regData;
};


