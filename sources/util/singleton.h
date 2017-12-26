/*! \brief A Singleton base class template.
 */

#pragma once

namespace util {

template <typename T>
class Singleton {
  public:
    Singleton() {
        if (s_instance) {
            throw;
        }

        s_instance = static_cast<T*>(this);
    }

    static inline T& instance() {
        return *s_instance;
    }

    static inline bool exists() {
        return s_instance != 0;
    }

    virtual ~Singleton() {
        s_instance = 0;
    }

  protected:
    static T* s_instance;
};

template <typename T>
T* Singleton<T>::s_instance = 0;

}
