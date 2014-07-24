#pragma once

template <typename F>
class QuadTree
{
public:
	QuadTree() 
	{
		level_count=0;
		max_level=-1;
	};
	virtual ~QuadTree() 
	{
		clear();
	};

	struct TreeNodeCrd
	{
		TreeNodeCrd()
		{
			level=0;
		};

		TreeNodeCrd(UINT level, Vec2i levelCoord)
		{
			this->level=level;
			this->childCoord.set(levelCoord.x%2,levelCoord.y%2);
			this->levelCoord=levelCoord;

			int lev_size=(int)pow(2.f,(float)level);
			this->worldCoord.set(float(levelCoord.x)/float(lev_size),float(levelCoord.y)/float(lev_size));
		};

		TreeNodeCrd GetParentCoord() const
		{
			_ASSERT(level!=0);
			Vec2i glob=Vec2i(levelCoord.x/2,levelCoord.y/2);
			return TreeNodeCrd(level-1,glob);
		};

		TreeNodeCrd GetChildCrd(UINT nChild) const
		{
			_ASSERT(nChild<4);
			
			return GetChildCrd(nChild%2,nChild/2);
		};

		TreeNodeCrd GetChildCrd(UINT x, UINT y) const
		{
			_ASSERT(x<=1);
			_ASSERT(y<=1);

			return TreeNodeCrd(level+1,Vec2i(levelCoord.x*2+x,levelCoord.y*2+y));
		};

		float getNodeSizeWorld() const
		{
			int lev_size=(int)pow(2.f,(float)level);
			return 1.f/float(lev_size);
		};

		UINT level;
		Vec2i childCoord;
		Vec2i levelCoord;
		Vec2 worldCoord;
	};

	static UINT GetLevelSize(UINT level) {return (int)pow(2.f,(float)level);};
	static UINT GetLevelFromNodeCount(UINT maxLevelNodes) {return matkit::logBase(maxLevelNodes,2);};
	static UINT GetLevelMemoryOffset(const TreeNodeCrd& crd) {return (pow(4.f,float(crd.level))-1)/3;};
	static UINT GetIdFromCoord(const TreeNodeCrd& crd) 
	{
		return GetLevelMemoryOffset(crd)+
							crd.levelCoord.x+
							crd.levelCoord.y*GetLevelSize(crd.level);
	};

	template <typename F>
	struct TreeNode
	{
		/*TreeNode()
		{
			memset(childs,0,4*sizeof(TreeNode*));
			parent=0;
			id=-1;
		};*/

		TreeNode(TreeNode* parent,int id,TreeNodeCrd coord)
		{
			this->id=id;
			this->coord=coord;
			this->parent=parent;
			memset(childs,0,4*sizeof(TreeNode*));
		};

		int id;
		TreeNodeCrd coord;

		TreeNode* parent;
		TreeNode* childs[4];

		F data;
	};

	template <typename F>
	struct TreeNodeIterator
	{
	public:
		TreeNodeIterator()
		{
			node_id=-1;
			parent=0;
		};

		TreeNodeIterator(const TreeNodeIterator<F>& other)
		{
			this->operator=(other);
		};

		TreeNodeIterator(int id, QuadTree<F>* parent)
		{
			node_id=id;
			this->p_parent_class=parent;
		};

		TreeNodeIterator<F> get_child(UINT nChild) const
		{
			return p_parent_class->get_child(*this,nChild);
		};

		TreeNodeIterator<F> get_child(UINT x, UINT y) const
		{
			_ASSERT(x<=1);
			_ASSERT(y<=1);

			return p_parent_class->get_child(*this,x+y*2);
		};

		TreeNodeIterator<F>& child(UINT nChild)
		{
			*this=p_parent_class->get_child(*this,nChild);
			return *this;
		};

		TreeNodeIterator& child(UINT x, UINT y)
		{
			*this=get_child(*this,x,y);
			return *this;
		};

		TreeNodeIterator<F>& parent()
		{
			*this=p_parent_class->get_parent(*this);
			return *this;
		};

		TreeNodeIterator<F> get_parent() const
		{
			return p_parent_class->get_parent(*this);
		};

		bool operator==(const TreeNodeIterator<F>& other)
		{
			if((this->p_parent_class==other.p_parent_class) && (this->node_id==other.node_id))
				return true;
			return false;
		};

		bool operator!=(const TreeNodeIterator<F>& other)
		{
			return !(this->operator==(other));
		};

		TreeNodeIterator<F>& operator=(const TreeNodeIterator<F>& other)
		{
			this->node_id=other.node_id;
			this->p_parent_class=other.p_parent_class;
			return *this;
		};

		F& operator*()
		{
			return p_parent_class->get_data(*this);
		};

	private:
		friend class QuadTree;
		int node_id;
		QuadTree<F>* p_parent_class;
	};

	typedef typename TreeNodeIterator<F> iterator;

	void clear()
	{
		for(int i=0;i<nodes_map.size();i++)
		{
			if(!nodes_map[i])
				continue;
			delete nodes_map[i];
			nodes_map[i]=0;
		};
		nodes_map.clear();
	};

	iterator begin()
	{
		if(nodes_map.empty())
			return end();
		return iterator(0,this);
	};

	iterator end()
	{
		return iterator(-1,this);
	};

	iterator get_child(const iterator& it, UINT nChild)
	{
		_ASSERT(it.p_parent_class==this);
		_ASSERT(nChild<4);
		_ASSERT((it.node_id>=0) && (it.node_id<nodes_map.size()));
		_ASSERT(nodes_map[it.node_id]);

		if(nodes_map[it.node_id]->childs[nChild])
			return iterator(nodes_map[it.node_id]->childs[nChild]->id,this);
		return end();
	};

	iterator get_parent(const iterator& it)
	{
		_ASSERT(it.p_parent_class==this);
		_ASSERT((it.node_id>=0) && (it.node_id<nodes_map.size()));
		_ASSERT(nodes_map[it.node_id]);

		if(nodes_map[it.node_id]->parent==0)
			return begin();//ACHTUNG
		return iterator(nodes_map[it.node_id]->parent->id,this);
	};

	F& get_data(const iterator& it)
	{
		_ASSERT(it.p_parent_class==this);
		_ASSERT((it.node_id>=0) && (it.node_id<nodes_map.size()));
		_ASSERT(nodes_map[it.node_id]);

		return nodes_map[it.node_id]->data;
	};

	iterator insert_root(F& data)
	{
		if(nodes_map.empty())
		{
			nodes_map.resize(1,0);
			nodes_map[0]=new TreeNode<F>(0,0,TreeNodeCrd(0,Vec2i(0,0)));
		}
		nodes_map[0]->data=data;
		
		return iterator(0,this);
	};

	iterator insert_node(const TreeNodeCrd& crd, F& data)
	{
		if(crd.level==0)
			return insert_root(data);

		int parent_id=GetIdFromCoord(crd.GetParentCoord());
		iterator it=_get_node(parent_id);
		if(it==end())
			return end();
		return insert_child(it,crd.childCoord.x,crd.childCoord.y,data);
	};

	iterator insert_child(const iterator& it, UINT nChild, F& data)
	{
		_ASSERT(it.p_parent_class==this);
		_ASSERT((it.node_id>=0) && (it.node_id<nodes_map.size()));
		_ASSERT(nodes_map[it.node_id]);

		if(it.get_child(nChild)!=end())
			return end();

		return _insert_node_child(nodes_map[it.node_id]->coord.GetChildCrd(nChild),data);
	};

	iterator insert_child(const iterator& it, UINT x, UINT y,F& data)
	{
		_ASSERT(x<2);
		_ASSERT(y<2);

		return insert_child(it,y*2+x,data);
	};

	iterator get(UINT level, Vec2i levelCoord)
	{
		int id=GetIdFromCoord(TreeNodeCrd(level,levelCoord));
		return _get_node(id);
	};

	TreeNodeCrd get_coord(const iterator& it)
	{
		_ASSERT(it.p_parent_class==this);
		_ASSERT((it.node_id>=0) && (it.node_id<nodes_map.size()));
		_ASSERT(nodes_map[it.node_id]);

		return nodes_map[it.node_id]->coord;
	};
protected:
	iterator _insert_node_child(const TreeNodeCrd& crd,F& data)
	{
		if((max_level!=-1) && (crd.level>=max_level))
			return end();

		int offset=GetLevelMemoryOffset(crd);
		int id=offset+crd.levelCoord.x+crd.levelCoord.y*GetLevelSize(crd.level);
		int parent_id=GetIdFromCoord(crd.GetParentCoord());

		_ASSERT(parent_id>=0);
		_ASSERT(nodes_map[parent_id]!=0);

		if(nodes_map.size()<=id)
		{
			int lev_size=GetLevelSize(crd.level);
			nodes_map.resize(offset+lev_size*lev_size,0);
		}
		
		_ASSERT(nodes_map[id]==0);

		nodes_map[id]=new TreeNode<F>(nodes_map[parent_id],id,crd);
		nodes_map[id]->data=data;
		nodes_map[parent_id]->childs[crd.childCoord.x+crd.childCoord.y*2]=nodes_map[id];
		return iterator(id,this);
	};

	iterator _get_node(int id)
	{
		if((id<0) || (id>=nodes_map.size()))
			return end();
		if(nodes_map[id]==0)
			return end();
		return iterator(id,this);
	};

protected:
	std::vector<TreeNode<F>*> nodes_map;
	UINT level_count;
	int max_level;
};