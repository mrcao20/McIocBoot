#pragma once

#include "../IMcPrimaryBeanParserRepository.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractPrimaryBeanParserRepository);

MC_FORWARD_DECL_CLASS(IMcPrimaryBeanParser);

class McAbstractPrimaryBeanParserRepository
        : public QObject
        , public IMcPrimaryBeanParserRepository 
{
    Q_OBJECT
public:
    explicit McAbstractPrimaryBeanParserRepository(QObject *parent = nullptr);
    ~McAbstractPrimaryBeanParserRepository() override;
    
    void addParser(const QString &name, IMcPrimaryBeanParserConstPtrRef parser) noexcept;
    IMcBeanDefinitionRegistry *registry() const noexcept;
    
    /*!
     * \brief parseBean
     * 
     * data的类型可以改为一个接口，XML相关的类型可以单独实现该接口，然后将QDomElement包裹在
     * 该子类中，接口提供一个回调函数，不断返回属性的QVariant类型
     * \param data
     * \param beanDefinition
     * \param registry
     */
    void parseBean(const QVariant &data
                   , IMcBeanDefinitionConstPtrRef beanDefinition
                   , IMcBeanDefinitionRegistry *registry) noexcept override final;
    
protected:
    virtual void doParseBean(const QVariant &data) noexcept = 0;
    
    void parsePrimaryBean(const QString &parserName, const QVariant &data) noexcept;
    
private:
    MC_DECL_PRIVATE(McAbstractPrimaryBeanParserRepository)
};

MC_DECL_POINTER(McAbstractPrimaryBeanParserRepository)
