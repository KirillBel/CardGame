#pragma once
template <typename T_StoredType>
struct NamedMapElem
{
	static UINT NamedMapHash(const char* name)
	{
		return strkit::calculateHash(name);
	};

	NamedMapElem(UINT id, const char* name, T_StoredType data)
	{
		this->name=name;
		this->name_hash=NamedMapHash(name);
		this->id=id;
		this->data=data;
	};

	NamedMapElem(const NamedMapElem<T_StoredType>& other)
	{
		name=other.name;
		name_hash=other.name_hash;
		id=other.id;
		data=other.data;
	};

	std::string name;
	UINT name_hash;
	UINT id;

	T_StoredType data;
};


template <typename T_StoredType>
struct NamedMapIter;

template <typename T_StoredType>
class NamedMap
{
public:
	typedef typename NamedMapIter<T_StoredType> iterator;


public:
	NamedMap() 
	{
		elements.reserve(128);
		totalReads=0;
		totalWrites=0;
	};
	NamedMap(NamedMap& other) 
	{
		elements.reserve(128);
		totalReads=0;
		totalWrites=0;

		this->operator=(other);
	};
	virtual ~NamedMap() 
	{
		clear();
	};


	UINT insert(const char* name, T_StoredType data)
	{
		totalWrites++;
		NamedMapElem<T_StoredType>* pElem=elem_find(name);

		if(pElem!=NULL)
		{
			pElem->data=data;
			return pElem->id;
		};

		pElem=new NamedMapElem<T_StoredType>(0,name,data);

		if(!availableIDs.empty())
		{
			UINT avId=availableIDs[availableIDs.size()-1];
			pElem->id=avId;
			elements[avId]=pElem;
			availableIDs.pop_back();
		}
		else
		{
			pElem->id=elements.size();
			elements.push_back(pElem);
		};

		elem_hashmap_insert(pElem);
		return pElem->id;
	};

	UINT insert_by_id(UINT id,const char* name, T_StoredType& data)
	{
		totalWrites++;
		NamedMapElem<T_StoredType>* pElem=elem_find(name);
		ASSERT(pElem==0);

		pElem=new NamedMapElem<T_StoredType>(0,name,data);

		if(!availableIDs.empty())
		{
			for(auto i=availableIDs.begin();i!=availableIDs.end();i++)
			{
				if((*i)==id)
				{
					pElem->id=(*i);
					elements[(*i)]=pElem;
					availableIDs.erase(i);
					return id;
				};
			};
		}
		
		if(elements.size()<=id)
		{
			int first_ins=elements.size();
			int end_ins=id-1;
			elements.resize(id+1,0);

			for(int i=first_ins;i<=end_ins;i++)
				availableIDs.push_back(i);
		};
		pElem->id=id;
		elements[id]=pElem;
		elem_hashmap_insert(pElem);
		return id;
	};
	
	NamedMapIter<T_StoredType> get(UINT id)
	{
		totalReads++;
		if(elements[id]==0)
			return end();
		return NamedMapIter<T_StoredType>(this,id);
	};

	NamedMapIter<T_StoredType> find(const char* name)
	{
		totalReads++;
		NamedMapElem<T_StoredType>* pElem=elem_find(name);
		if(pElem==NULL)
			return end();

		return NamedMapIter<T_StoredType>(this,pElem->id);
	};

	NamedMapIter<T_StoredType> begin()
	{
		return NamedMapIter<T_StoredType>(this,0);
	};

	NamedMapIter<T_StoredType> end()
	{
		return NamedMapIter<T_StoredType>(this,elements.size());
	};

	void iter_next(NamedMapIter<T_StoredType>& iter)
	{
		iter.id++;
		while((iter!=end()) && (elements[iter.id]==0))
		{
			iter.id++;
		};
	};

	void iter_prev(NamedMapIter<T_StoredType>& iter)
	{
		while((iter!=begin()) || (elements[iter.id]==0))
		{
			iter.id--;
		};
	};

	T_StoredType* iter_get(NamedMapIter<T_StoredType>& iter)
	{
		if((elements.empty()) || (elements[iter.id]==0))
			return NULL;
		return &(elements[iter.id]->data);
	};

	std::string get_name(NamedMapIter<T_StoredType>& iter)
	{
		if((elements.empty()) || (elements[iter.id]==0))
			return NULL;
		return (elements[iter.id]->name);
	};

	std::string get_id(NamedMapIter<T_StoredType>& iter)
	{
		if((elements.empty()) || (elements[iter.id]==0))
			return NULL;
		return iter.id;
	};

	NamedMapIter<T_StoredType> erase(NamedMapIter<T_StoredType> iter)
	{
		remove(iter.id);
		iter_next(iter);
		return iter;
	};

	void remove(UINT id)
	{
		if(elements.empty())
			return;
		if(id>=elements.size())
			return;

		if(elements[id])
		{
			auto it=hashMap.find(elements[id]->name_hash);
			if(it!=hashMap.end())
				hashMap.erase(it);

			delete elements[id];
			elements[id]=0;
			availableIDs.push_back(id);
		};
	};

	void remove(const char* name)
	{
		NamedMapElem<T_StoredType>* pElem=elem_find(name);
		if(pElem==NULL)
			return;
		remove(pElem->id);
	};

	void clear()
	{
		for(UINT i=0;i<elements.size();i++)
		{
			if(elements[i])
			{
				delete elements[i];
				elements[i]=0;
			};
		};

		elements.clear();
		availableIDs.clear();
		hashMap.clear();
	};

	NamedMap<T_StoredType>& operator = (const NamedMap<T_StoredType>& other)
	{
		clear();

		for(int i=0;i<other.elements.size();i++)
		{
			if(other.elements[i]==0)
			{
				elements.push_back(0);
				continue;
			};

			NamedMapElem<T_StoredType>* pElem=new NamedMapElem<T_StoredType>(*other.elements[i]);
			elements.push_back(pElem);
			elem_hashmap_insert(pElem);
		};

		return *this;
	};

private:
	NamedMapElem<T_StoredType>* elem_find(const char* name)
	{
		UINT hash=NamedMapElem<T_StoredType>::NamedMapHash(name);

		auto it=hashMap.find(hash);
		if(it==hashMap.end())
			return NULL;

		for(UINT i=0;i<it->second.size();i++)
		{
			if(it->second[i]->name==name)
				return it->second[i];
		};

		return NULL;
	};

	void elem_hashmap_insert(NamedMapElem<T_StoredType>* pElem)
	{
		
		auto it=hashMap.find(pElem->name_hash);

		if(it==hashMap.end())
			hashMap[pElem->name_hash].push_back(pElem);
		else
		{
			for(UINT i=0;i<it->second.size();i++)
			{
				if(it->second[i]->name==pElem->name)
				{
					it->second[i]=pElem;
					return;
				};
			};
			it->second.push_back(pElem);
		};
	};

	

private:
	//friend struct NamedMapIter <T_StoredType>;
	std::map<UINT,std::vector<NamedMapElem<T_StoredType>*>> hashMap;
	std::vector<NamedMapElem<T_StoredType>*> elements;
	std::vector<UINT> availableIDs;

	//only counts
	UINT totalReads;
	UINT totalWrites;
};











template <typename T_StoredType>
struct NamedMapIter
{
public:
	NamedMapIter(NamedMap<T_StoredType>* pParent, UINT id)
	{
		this->pParent=pParent;
		this->id=id;
	};

	std::string get_name()
	{
		return pParent->get_name(*this);
	};

	std::string get_id()
	{
		return id;
	};

	bool operator==(NamedMapIter<T_StoredType>& other)
	{
		if((this->pParent==other.pParent) && (this->id==other.id))
			return true;
		return false;
	};

	bool operator!=(NamedMapIter<T_StoredType>& other)
	{
		return !(this->operator==(other));
	};

	NamedMapIter& operator=(NamedMapIter& other)
	{
		this->id=other.id;
		return *this;
	};

	NamedMapIter& operator++()
	{
		pParent->iter_next(*this);
		return *this;
	};

	NamedMapIter& operator--()
	{
		pParent->iter_prev(*this);
		return *this;
	};

	T_StoredType* operator*()
	{
		return pParent->iter_get(*this);
	};


	
protected:
	friend class NamedMap<T_StoredType>;
	UINT id;
	NamedMap<T_StoredType>* pParent;
};