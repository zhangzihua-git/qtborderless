#ifndef HEAD_H
#define HEAD_H



#define ADD_PROPERTY(t, p) \
    Q_PROPERTY(t p READ p WRITE set_##p NOTIFY p##_changed)\
    t m_##p;\
public:\
    Q_SIGNAL void p##_changed(t p);\
    t p(){return m_##p;}\
    void set_##p(t p){m_##p = p;}\
private:


#endif // HEAD_H
