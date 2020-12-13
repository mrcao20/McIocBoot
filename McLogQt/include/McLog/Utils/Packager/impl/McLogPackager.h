#pragma once

#include "../IMcLogPackager.h"

MC_FORWARD_DECL_PRIVATE_DATA(McLogPackager);

class McLogPackager : public QObject, public IMcLogPackager
{
    Q_OBJECT
    MC_DEFINE_TYPELIST(IMcLogPackager);
public:
    Q_INVOKABLE McLogPackager() noexcept;
    ~McLogPackager();
    
private:
    MC_DECL_PRIVATE(McLogPackager)
};

MC_DECL_METATYPE(McLogPackager)

