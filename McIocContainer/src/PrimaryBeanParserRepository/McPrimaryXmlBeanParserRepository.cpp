#include "McIoc/PrimaryBeanParserRepository/impl/McPrimaryXmlBeanParserRepository.h"

#include <QDomNodeList>

MC_DECL_PRIVATE_DATA(McPrimaryXmlBeanParserRepository)
MC_DECL_PRIVATE_DATA_END

McPrimaryXmlBeanParserRepository::McPrimaryXmlBeanParserRepository(QObject *parent)
    : McAbstractPrimaryBeanParserRepository(parent)
{
    MC_NEW_PRIVATE_DATA(McPrimaryXmlBeanParserRepository)
}

McPrimaryXmlBeanParserRepository::~McPrimaryXmlBeanParserRepository() 
{
}

void McPrimaryXmlBeanParserRepository::doParseBean(const QVariant &data) noexcept
{
    QDomNodeList propNodes = data.value<QDomNodeList>();
    for (int i = 0; i < propNodes.size(); ++i) {
		QDomElement propEle = propNodes.at(i).toElement();
		if (propEle.isNull())
			continue;
        parsePrimaryBean(propEle.tagName(), QVariant::fromValue(propEle));
	}
}
