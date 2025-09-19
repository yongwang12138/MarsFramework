#ifndef MARSSINGLETON_H
#define MARSSINGLETON_H

#define Q_SINGLETON_CREATE_H(Class) \
public:                            \
    static Class& instance();        \
    private:        \
    Class(const Class&) = delete;        \
    Class& operator=(const Class&) = delete;      \

#define Q_SINGLETON_CREATE_CPP(Class)  \
    Class& Class::instance()        \
{                                  \
        static Class instance;           \
        return instance;              \
}

#endif // MARSSINGLETON_H
