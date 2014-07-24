#pragma once


inline bool NumberValid(const float x)
{
	return (x==x) && (!_isnan(x)) && _finite(x);
};

inline bool NumberValid(const double x)
{
	return (x==x) && (!_isnan(x)) && _finite(x);
};


///////////////////////////////////////////////////////

inline bool NumberValid(const int8 x)
{	
	return true; //integers are always valid
}
inline bool NumberValid(const uint8 x)
{	
	return true; //integers are always valid
}
inline bool NumberValid(const int16 x)
{	
	return true; //integers are always valid
}
inline bool NumberValid(const uint16 x)
{	
	return true; //integers are always valid
}
inline bool NumberValid(const int32 x)
{	
	return true; //integers are always valid
}
inline bool NumberValid(const uint32 x)
{	
	return true; //integers are always valid
}
inline bool NumberValid(const int64 x)
{	
	return true; //integers are always valid
}
inline bool NumberValid(const uint64 x)
{	
	return true; //integers are always valid
}