#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
public:
    Singleton()
    {
        if(s_Instance)
        {
            throw;
        }
        s_Instance = static_cast<T*>(this);
    }

    static inline T& instance()
    {
        return *s_Instance;
    }

    static inline bool exists()
    {
        return s_Instance != 0;
    }

    virtual ~Singleton()
    {
        s_Instance = 0;
    }

protected:
    static T* s_Instance;
};

template <typename T>
T* Singleton<T>::s_Instance = 0;

#endif // SINGLETON_H
