/*
 * scopepointer.h
 *
 *  Created on: 2011-8-4
 *      Author: heidong
 */

#ifndef SCOPEPOINTER_H_
#define SCOPEPOINTER_H_

#include <assert.h>

template <class ObjectType>
class ScopedPointer
{
public:
    inline ScopedPointer()
        : m_pObj (0)
    {
    }
    inline ScopedPointer (ObjectType* const pObj)
        : m_pObj (pObj)
    {
    }
    ScopedPointer (ScopedPointer& pOther)
        : m_pObj (pOther.m_pObj)
    {
        pOther.m_pObj = 0;
    }
    inline ~ScopedPointer()
    {
        delete m_pObj;
    }

    ScopedPointer& operator= (ScopedPointer& pOther)
    {
        if (this != pOther.getAddress())
        {
            assert (m_pObj == 0 || m_pObj != pOther.m_pObj);

            ObjectType* const pOldObject = m_pObj;
            m_pObj = pOther.m_pObj;
            pOther.m_pObj = 0;
            delete pOldObject;
        }

        return *this;
    }
    ScopedPointer& operator= (ObjectType* const pObj)
    {
        if (m_pObj != pObj)
        {
            ObjectType* const pOldObject = m_pObj;
            m_pObj = pObj;
            delete pOldObject;
        }

        return *this;
    }
    inline operator ObjectType*() const
    {
        return m_pObj;
    }
    inline ObjectType& operator*() const
    {
         return *m_pObj;
    }
    inline ObjectType* operator->() const
    {
        return m_pObj;
    }
    ObjectType* release()
    {
        ObjectType* const pObj = m_pObj;
        m_pObj = 0;
        return pObj;
    }
    void swapWith (ScopedPointer <ObjectType>& pOther)
    {
        assert (m_pObj != pOther.m_pObj);
        ObjectType* pTmp = m_pObj;
        m_pObj = pOther.m_pObj;
        pOther.m_pObj = pTmp;
    }

private:
    ObjectType* m_pObj;
    const ScopedPointer* getAddress() const
    {
        return this;
    }
};

template <class ObjectType>
bool operator== (const ScopedPointer<ObjectType>& pointer1, ObjectType* const pointer2)
{
    return static_cast <ObjectType*> (pointer1) == pointer2;
}

template <class ObjectType>
bool operator!= (const ScopedPointer<ObjectType>& pointer1, ObjectType* const pointer2)
{
    return static_cast <ObjectType*> (pointer1) != pointer2;
}


#endif /* SCOPEPOINTER_H_ */
