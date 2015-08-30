/*
 * UtilHelper.h
 *
 *  Created on: 2011-8-5
 *      Author: heidong
 */

#ifndef UTILHELPER_H_
#define UTILHELPER_H_

typedef char        Char;
typedef Char*       CharPtr;
typedef const CharPtr ConstCharPtr;

#define NullPtr     (0)

#define DECLARE_SINGLETON(TheClass)             \
    public:                                     \
        static TheClass* GetInstancePtr()       \
        {                                       \
            if (sm_pInst == 0){                 \
                sm_pInst = new TheClass;        \
            }                                   \
            return sm_pInst;                    \
        }                                       \
        static TheClass& GetInstance()          \
        {                                       \
            TheClass* pCls = GetInstancePtr();  \
            return *pCls;                       \
        }                                       \
        static void ReleaseRC()                 \
        {                                       \
            if(sm_pInst != 0)                   \
            {                                   \
                delete sm_pInst;                \
                sm_pInst = 0;                   \
            }                                   \
        }                                       \
    private:                                    \
        TheClass(){}                            \
    private:                                    \
        static TheClass* sm_pInst;              \
                                                \

#define IMPLEMENT_SINGLETON(TheClass)           \
    TheClass* TheClass::sm_pInst = 0;           \
                                                \




#endif /* UTILHELPER_H_ */
