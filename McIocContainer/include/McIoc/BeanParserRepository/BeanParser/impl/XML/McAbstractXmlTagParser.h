#pragma once

#include "../../IMcBeanParser.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractXmlTagParser);

class McAbstractXmlTagParser : public QObject, public IMcBeanParser
{
    Q_OBJECT
public:
    McAbstractXmlTagParser() noexcept;
    ~McAbstractXmlTagParser() override;
    
private:
    MC_DECL_PRIVATE(McAbstractXmlTagParser)
};

MC_DECL_POINTER(McAbstractXmlTagParser)
