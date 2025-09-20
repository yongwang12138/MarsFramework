#ifndef MARSPROPERTY_H
#define MARSPROPERTY_H

// 普通属性快速创建

#define Q_PROPERTY_CREATE(TYPE, M)                          \
Q_PROPERTY(TYPE p##M MEMBER _p##M NOTIFY p##M##Changed)     \
    public:                                                 \
    Q_SIGNAL void p##M##Changed();                          \
    void set##M(TYPE M)                                     \
{                                                           \
        _p##M = std::move(M);                               \
        Q_EMIT p##M##Changed();                             \
}                                                           \
    TYPE get##M() const                                     \
{                                                           \
        return _p##M;                                       \
}                                                           \
    private:                                                \
    TYPE _p##M;

#define Q_PROPERTY_CREATE_H(TYPE, M)                                \
Q_PROPERTY(TYPE p##M READ get##M WRITE set##M NOTIFY p##M##Changed) \
    public:                                                         \
    Q_SIGNAL void p##M##Changed();                                  \
    void set##M(TYPE M);                                            \
    TYPE get##M() const;                                            \
    private:                                                        \
    TYPE _p##M;

#define Q_PROPERTY_CREATE_CPP(CLASS, TYPE, M)   \
void CLASS::set##M(TYPE M)                      \
{                                               \
        _p##M = M;                              \
        Q_EMIT p##M##Changed();                 \
}                                               \
    TYPE CLASS::get##M() const                  \
{                                               \
        return _p##M;                           \
}

#define Q_PRIVATE_CREATE(TYPE, M)   \
public:                             \
    void set##M(TYPE M)             \
{                                   \
        _p##M = std::move(M);       \
}                                   \
    TYPE get##M() const             \
{                                   \
        return _p##M;               \
}                                   \
    private:                        \
    TYPE _p##M;




#endif // MARSPROPERTY_H
