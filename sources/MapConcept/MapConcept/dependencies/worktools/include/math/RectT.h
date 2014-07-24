#pragma once
template <typename F> class RectT
{
public:
	inline RectT(void) : top(0),bottom(0),left(0),right(0)  {};
	inline RectT(F l, F t, F r, F b) : top(t),bottom(b),left(l),right(r) {};
	~RectT(void) {};

	inline void add(RectT<F>& other) {left=MIN(left,other.left);right=MAX(right,other.right);top=MIN(top,other.top);bottom=MAX(bottom,other.bottom);};

	inline void set(F l, F t, F r, F b) {top=t;bottom=b;left=l;right=r;};
	inline Vec2T<F> getTopLeft() {return Vec2T<F>(left,top);};
	inline Vec2T<F> getTopRight() {return Vec2T<F>(right,top);};
	inline Vec2T<F> getBottomLeft() {return Vec2T<F>(left,bottom);};
	inline Vec2T<F> getBottomRight() {return Vec2T<F>(right,bottom);};
	inline Vec2T<F> getSize() {return Vec2T<F>(abs(right-left),abs(top-bottom));};
	inline Vec2T<F> getCenter() {return Vec2T<F>(left+(right-left)/2,top+(bottom-top)/2);};
	inline F width() {return abs(right-left);};
	inline F height() {return abs(top-bottom);};
	
	inline void narrow(F val) {right-=val; left+=val; top-=val; bottom+=val;};
	inline void narrow(Vec2T<F> val) {right-=val.x; left+=val.x; top-=val.y; bottom+=val.y;};
	inline bool pointInRect(Vec2T<F> pt) {return matkit::in_range(pt.x,left,right) && matkit::in_range(pt.y,top,bottom);};
	inline void move(Vec2T<F> vec) {left+=vec.x;	right+=vec.x; top+=vec.y; bottom+=vec.y;};
	inline void moveTo(Vec2T<F> vec) {right=vec.x+(right-left); left=vec.x; top=vec.y+(top-bottom); bottom=vec.y;};
	inline void moveToZero() {right=(right-left); left=0; top=(top-bottom); bottom=0; };
	inline void setSize(Vec2T<F> vec) {right=left+vec.x; top=bottom+vec.y;};
	inline bool rectIn(RectT<F>& rect) {
        return (pointInRect(rect.getBottomLeft())) || (pointInRect(rect.getBottomRight())) ||
                            (pointInRect(rect.getTopLeft())) || (pointInRect(rect.getTopRight()));
	};

	inline bool intersects(RectT<F>& other)
	{
		return ((left < other.right) && (right > other.left) &&
			(top < other.bottom) && (bottom > other.top)); 
	};

	inline F getSquare()
	{
		Vec2 v=GetSize();
		if((v.x==0) || (v.y==0)) return 0;
		return v.x*v.y;
	};

	inline void operator = (RectT<F> & rect) {set(rect.left,rect.top,rect.right,rect.bottom);};
	inline bool operator == (RectT<F> & rect) 
		{ return top == rect.top && bottom == rect.bottom && left == rect.left && right==rect.right; }
	inline bool operator != (RectT<F> & rect) { return !(*this == rect); }

	inline RectT<F> operator + (Vec2T<F> & val) {return RectT<F>(left+val.x,top+val.y,right+val.x,bottom+val.y);};
	inline RectT<F> operator - (Vec2T<F> & val) {return RectT<F>(left-val.x,top-val.y,right-val.x,bottom-val.y);};
	inline RectT<F> operator * (Vec2T<F> & val) {return RectT<F>(left*val.x,top*val.y,right*val.x,bottom*val.y);};
	inline RectT<F> operator / (Vec2T<F> & val) {return RectT<F>(left/val.x,top/val.y,right/val.x,bottom/val.y);};

	F top;
	F bottom;
	F left;
	F right;
};

typedef RectT<f32> Rect4;
typedef RectT<int> Rect4i;
typedef RectT<double> Rect4r;
