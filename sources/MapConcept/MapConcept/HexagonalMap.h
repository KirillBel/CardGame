#pragma once

template <typename tpl_Type> class HexagonalMap;

template <typename tpl_Type> 
struct HexagonalNode
{
	HexagonalNode(tpl_Type& val, Vec2i& coord)
	{
		this->data=val;
		this->coord=coord;
	};

	tpl_Type data;
	Vec2i coord;
};

template <typename tpl_Type> 
class HexagonalNodeIterator
{
	HexagonalMap<tpl_Type>* parent;
	HexagonalNode<tpl_Type>* ptr;
public:
	HexagonalNodeIterator(HexagonalNode<tpl_Type>* _ptr, HexagonalMap<tpl_Type>* _parent) : ptr(_ptr), parent(_parent) {}
	HexagonalNodeIterator(const HexagonalNodeIterator& mit) : ptr(mit.ptr), parent(mit.parent) {}
		
	HexagonalNodeIterator& operator++() {++ptr;return *this;}
	HexagonalNodeIterator& operator--() {--ptr;return *this;}
	HexagonalNodeIterator& operator++(int t) {ptr+=t;return *this;}
	HexagonalNodeIterator& operator--(int t) {ptr-=t;return *this;}

	Vec2i coord() {return parent->_getNode(offset)->coord;};

	bool operator==(const HexagonalNodeIterator& rhs) {return (ptr==rhs.ptr) && (parent==rhs.parent);}
	bool operator!=(const HexagonalNodeIterator& rhs) {return (ptr!=rhs.ptr) || (parent!=rhs.parent);}
		
	tpl_Type& operator*() {_ASSERT(parent->CheckValidId(ptr)); return ptr->data;}
};
	

template <typename tpl_Type> 
class HexagonalMap
{
public:
	HexagonalMap(void)
	{
		_myFirst=0;
		_myLast=0;
		_myEnd=0;
	};
	~HexagonalMap(void)
	{
	};

	typedef typename HexagonalNodeIterator<tpl_Type> iterator;
	typedef HexagonalNode<tpl_Type>* pointer_node;
	typedef const HexagonalNode<tpl_Type>* const_pointer_node;
	typedef HexagonalNode<tpl_Type>& reference_node;
	typedef const HexagonalNode<tpl_Type>& const_reference_node;

	typedef HexagonalNode<tpl_Type>* pointer_data;
	typedef const HexagonalNode<tpl_Type>* const_pointer_data;
	typedef HexagonalNode<tpl_Type>& reference_data;
	typedef const HexagonalNode<tpl_Type>& const_reference_data;

	UINT size() const {return nodes.size();};

	UINT calculate_hash(Vec2i& coord) {return pow(double(coord.x+coord.y),2)+pow(double(coord.y*coord.x),2);};

	tpl_Type& operator[](UINT index) {return nodes[index].data;};
	Vec2i coord(int index) {return nodes[index].coord;};

	iterator insert(tpl_Type& val, Vec2i coord)
	{
		UINT hash=calculate_hash(coord);
		auto it=coords.find(hash);
		if(it==coords.end())
		{
			nodes.push_back(HexagonalNode<tpl_Type>(val,coord));
			coords[hash]=nodes.size()-1;
		}
		else
		{
			nodes[(*it).first]=HexagonalNode<tpl_Type>(val,coord);
		};

		_myFirst=&nodes.front();
		_myLast=&nodes.back();
		_myEnd=_myLast+1;
		return get(coord);
	};

	iterator get(Vec2i coord)
	{
		UINT hash=calculate_hash(coord);
		auto it=coords.find(hash);
		if(it!=coords.end())
			return iterator(_myFirst+it->second,this);
		return end();
	};

	iterator begin()	{return iterator(_myFirst,this);};
	iterator end()		{return iterator(_myEnd,this);};

	reference_data front()	{return *begin();};
	const_reference_data front() const {return *begin();};
	reference_data back()	{return *(end()-1);};
	const_reference_data back() const {return *(end()-1);};

protected:
	friend class HexagonalNodeIterator<tpl_Type>;
	void AssertValidId(int id) {ASSERT(CheckValidId(id));};
	bool CheckValidId(pointer_node ptr) 
	{
		if(!nodes.size()) 
			return false; 
		return ((ptr>=_myFirst) && (ptr<=_myLast));
	};
protected:
	std::vector<HexagonalNode<tpl_Type>> nodes;
	std::map<UINT, UINT> coords;

	pointer_node _myFirst;
	pointer_node _myLast;
	pointer_node _myEnd;

};

