#pragma once

#include "McAbstractPrimaryXmlTagParser.h"

MC_FORWARD_DECL_PRIVATE_DATA(McPropertyTagParser);

class McPropertyTagParser
        : public McAbstractPrimaryXmlTagParser 
{
    Q_OBJECT
public:
    explicit McPropertyTagParser(QObject *parent = nullptr);
    ~McPropertyTagParser() override;
    
private:
    MC_DECL_PRIVATE(McPropertyTagParser)
};

MC_DECL_POINTER(McPropertyTagParser)
