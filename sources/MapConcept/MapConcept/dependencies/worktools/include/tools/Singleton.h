#pragma once

template<class T>
class Singleton
{
public:
	static T& Instance()
    {
		if(_self.get()==0)
			_self.reset(new T);
		return *_self.get();
    }

protected:
	Singleton() {};
	virtual ~Singleton() {_self.release();}
private:
	static std::auto_ptr<T> _self;
};

template <class T>
std::auto_ptr<T> Singleton<T>::_self(0);