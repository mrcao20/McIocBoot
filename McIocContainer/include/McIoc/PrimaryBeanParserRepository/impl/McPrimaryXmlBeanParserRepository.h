#pragma once

#include "McAbstractPrimaryBeanParserRepository.h"

MC_FORWARD_DECL_PRIVATE_DATA(McPrimaryXmlBeanParserRepository);

class McPrimaryXmlBeanParserRepository
        : public McAbstractPrimaryBeanParserRepository
{
    Q_OBJECT
public:
    explicit McPrimaryXmlBeanParserRepository(QObject *parent = nullptr);
    ~McPrimaryXmlBeanParserRepository() override;
    
protected:
    void doParseBean(const QVariant &data) noexcept override;
    
private:
    MC_DECL_PRIVATE(McPrimaryXmlBeanParserRepository)
};

MC_DECL_POINTER(McPrimaryXmlBeanParserRepository)
