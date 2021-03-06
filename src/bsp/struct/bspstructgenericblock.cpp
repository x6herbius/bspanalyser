#include "bspstructgenericblock.h"

#include "bspstructitemtypeconverterfactory.h"

BSPStructGenericBlock::BSPStructGenericBlock(quint32 inOffset, BSPStructItemTypes::CoreItemType inType, quint32 inItemCount)
    : m_nOffsetInStruct(inOffset),
      m_nItemType(inType),
      m_nPublicItemType(BSPStructItemTypes::PublicItemType::Int32),
      m_nItemCount(inItemCount),
      m_strName(),
      m_pConverter(),
      m_Attributes()
{
    // Make sure this is done after the rest of the initialisation.
    m_pConverter = BSPStructItemTypeConverterFactory::createConverter(*this);
}

quint32 BSPStructGenericBlock::offset() const
{
    return m_nOffsetInStruct;
}

BSPStructItemTypes::CoreItemType BSPStructGenericBlock::itemType() const
{
    return m_nItemType;
}

quint32 BSPStructGenericBlock::itemSize() const
{
    return BSPStructItemTypes::sizeOfCoreType(m_nItemType);
}

quint32 BSPStructGenericBlock::itemCount() const
{
    return m_nItemCount;
}

quint32 BSPStructGenericBlock::totalSize() const
{
    return itemCount() * itemSize();
}

QSharedPointer<BSPStructItemTypeConverter> BSPStructGenericBlock::typeConverter() const
{
    return m_pConverter;
}

QString BSPStructGenericBlock::name() const
{
    return m_strName;
}

void BSPStructGenericBlock::setName(const QString &newName)
{
    m_strName = newName;
}

BSPStructItemAttributes& BSPStructGenericBlock::attributes()
{
    return m_Attributes;
}

const BSPStructItemAttributes& BSPStructGenericBlock::attributes() const
{
    return m_Attributes;
}

BSPStructItemTypes::PublicItemType BSPStructGenericBlock::publicItemType() const
{
    return m_nPublicItemType;
}

void BSPStructGenericBlock::setPublicItemType(BSPStructItemTypes::PublicItemType publicType)
{
    m_nPublicItemType = publicType;
}
