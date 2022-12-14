#ifndef SINGLEHOLDER_H
#define SINGLEHOLDER_H
#include <mutex>


namespace PH {


template <typename T>
class SingleHolder{
public:
    static T* getInstance(){
        if(_ptr) return _ptr;
        std::unique_lock<std::mutex> lock(_lock);
        _ptr=new T();
        return _ptr;
    }
protected:
    SingleHolder(){

    }

    ~SingleHolder(){

    }
   private:
    class Deletor{
    public:
        ~Deletor(){
            if(nullptr!=SingleHolder::_ptr)
                delete SingleHolder::_ptr;
        }
    };
    static T *_ptr;
    static std::mutex _lock;
    static Deletor _deletor;
    SingleHolder(SingleHolder&)=delete;
};
template <typename T>
T* SingleHolder<T>::_ptr=nullptr;

template <typename T>
std::mutex SingleHolder<T>::_lock;
}


#endif // SINGLEHOLDER_H
