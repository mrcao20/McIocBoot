#pragma once

#include "McAbstractResponse.h"

MC_FORWARD_DECL_PRIVATE_DATA(McCppResponse);

class MCQUICKBOOT_EXPORT McCppResponse : public McAbstractResponse
{
    Q_OBJECT
    MC_DECL_INIT(McCppResponse)

public:
    explicit McCppResponse(QObject *parent = nullptr);
    ~McCppResponse() override;

    template<typename Func>
    McCppResponse &then(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                        Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 1,
                          "The callback can only be one parameter.");

        return thenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                        recever,
                        new QtPrivate::QSlotObject<Func,
                                                   typename FuncType::Arguments,
                                                   typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            McCppResponse &>::type
    then(Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 1,
                          "The callback can only be one parameter.");

        return thenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                        nullptr,
                        new QtPrivate::QStaticSlotObject<Func,
                                                         typename FuncType::Arguments,
                                                         typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1,
                            McCppResponse &>::type
    then(Func callback) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 1,
                          "The callback can only be one parameter.");

        return thenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                        nullptr,
                        new QtPrivate::QFunctorSlotObject<Func,
                                                          int(FuncType::ArgumentCount),
                                                          typename FuncType::Arguments,
                                                          typename FuncType::ReturnType>(
                            std::move(callback)));
    }

    template<typename Func>
    McCppResponse &syncThen(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                            Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 1,
                          "The callback can only be one parameter.");

        return syncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                            recever,
                            new QtPrivate::QSlotObject<Func,
                                                       typename FuncType::Arguments,
                                                       typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            McCppResponse &>::type
    syncThen(Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 1,
                          "The callback can only be one parameter.");

        return syncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                            nullptr,
                            new QtPrivate::QStaticSlotObject<Func,
                                                             typename FuncType::Arguments,
                                                             typename FuncType::ReturnType>(
                                callback));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1,
                            McCppResponse &>::type
    syncThen(Func callback) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 1,
                          "The callback can only be one parameter.");

        return syncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                            nullptr,
                            new QtPrivate::QFunctorSlotObject<Func,
                                                              int(FuncType::ArgumentCount),
                                                              typename FuncType::Arguments,
                                                              typename FuncType::ReturnType>(
                                std::move(callback)));
    }

    template<typename Func>
    McCppResponse &asyncThen(const typename QtPrivate::FunctionPointer<Func>::Object *recever,
                             Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 1,
                          "The callback can only be one parameter.");

        return asyncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                             recever,
                             new QtPrivate::QSlotObject<Func,
                                                        typename FuncType::Arguments,
                                                        typename FuncType::ReturnType>(callback));
    }
    template<typename Func>
    typename std::enable_if<int(QtPrivate::FunctionPointer<Func>::ArgumentCount) >= 0
                                && !QtPrivate::FunctionPointer<Func>::IsPointerToMemberFunction,
                            McCppResponse &>::type
    asyncThen(Func callback) noexcept
    {
        typedef QtPrivate::FunctionPointer<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 1,
                          "The callback can only be one parameter.");

        return asyncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                             nullptr,
                             new QtPrivate::QStaticSlotObject<Func,
                                                              typename FuncType::Arguments,
                                                              typename FuncType::ReturnType>(
                                 callback));
    }
    template<typename Func>
    typename std::enable_if<QtPrivate::FunctionPointer<Func>::ArgumentCount == -1,
                            McCppResponse &>::type
    asyncThen(Func callback) noexcept
    {
        typedef McPrivate::LambdaType<Func> FuncType;

        Q_STATIC_ASSERT_X(int(FuncType::ArgumentCount) == 1,
                          "The callback can only be one parameter.");

        return asyncThenImpl(McPrivate::QVariantSelector<typename FuncType::Arguments>::Value,
                             nullptr,
                             new QtPrivate::QFunctorSlotObject<Func,
                                                               int(FuncType::ArgumentCount),
                                                               typename FuncType::Arguments,
                                                               typename FuncType::ReturnType>(
                                 std::move(callback)));
    }

protected:
    void callCallback() noexcept override;

private:
    McCppResponse &thenImpl(bool isQVariant,
                            const QObject *recever,
                            QtPrivate::QSlotObjectBase *callback) noexcept;
    McCppResponse &syncThenImpl(bool isQVariant,
                                const QObject *recever,
                                QtPrivate::QSlotObjectBase *callback) noexcept;
    McCppResponse &asyncThenImpl(bool isQVariant,
                                 const QObject *recever,
                                 QtPrivate::QSlotObjectBase *callback) noexcept;

private:
    MC_DECL_PRIVATE(McCppResponse)
};