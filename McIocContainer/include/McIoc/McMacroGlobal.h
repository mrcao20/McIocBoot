#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MCIOCCONTAINER_LIBRARY)
#  define MCIOCCONTAINER_EXPORT Q_DECL_EXPORT
# else
#  define MCIOCCONTAINER_EXPORT Q_DECL_IMPORT
# endif
#else
# define MCIOCCONTAINER_EXPORT
#endif

#define MC_MACRO_STR(m) #m

#define MC_DECL_POINTER(Class) \
    using Class##Ptr = QSharedPointer<Class>; \
    using Class##ConstPtrRef = const QSharedPointer<Class> &;

#define MC_FORWARD_DECL_CLASS(Class) \
    class Class; \
    MC_DECL_POINTER(Class)

#define MC_FORWARD_DECL_STRUCT(Class) \
    struct Class; \
    using Class##Ptr = QSharedPointer<Class>; \
    using Class##ConstPtrRef = const QSharedPointer<Class> &;

#define MC_PRIVATE_DATA_NAME(Class) \
    Class##Data

#define MC_DECL_PRIVATE_DATA(Class) \
    struct Class##Data {

#define MC_DECL_PRIVATE_DATA_END };

#define MC_DECL_PRIVATE(Class) \
    QScopedPointer<Class##Data> d;

#define MC_NEW_PRIVATE_DATA(Class) \
    d.reset(new Class##Data());

#define MC_FORWARD_DECL_PRIVATE_DATA(Class) \
    struct Class##Data;

#define MC_PADDING_CLANG(size)  \
    char ___clang_padding___[size];


#ifndef Q_MOC_RUN			//!< 这行语句必须加，只有包围在这行语句之中的宏才能被识别为tag

# define MC_AUTOWIRED		//!< 用于表示函数是否需要自动注入

# define MC_BEAN_START      //!< 当bean被构造，但还未注入属性时调用
# define MC_BEAN_FINISHED   //!< 当bean完全被构造完成之后调用
# define MC_THREAD_FINISHED   //!< 当bean的线程被移动之后调用

#endif //! !Q_MOC_RUN


//! Q_CLASSINFO
#define MC_SINGLETON "isSingleton"
#define MC_BEANNAME "beanName"
//!< Q_CLASSINFO

//! 父标签
#define MC_BEANS "beans"
#define MC_BEAN "bean"
//! 一级标签
#define MC_PROPERTY "property"
#define MC_CONNECT "connect"
#define MC_CONSTRUCTOR "constructor"
//! 二级标签
#define MC_SEC_VALUE "value"
#define MC_SEC_REF "ref"
#define MC_SEC_LIST "list"
#define MC_SEC_MAP "map"
#define MC_SEC_BEAN "bean"

#define MC_THIS "this"
#define MC_SENDER "sender"
#define MC_SIGNAL "signal"
#define MC_RECEIVER "receiver"
#define MC_SLOT "slot"
#define MC_CONNECTION_TYPE "ConnectionType"
