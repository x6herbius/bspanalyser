#ifndef BSPSTRUCTGENERICBLOCK_H
#define BSPSTRUCTGENERICBLOCK_H

#include <QByteArray>
#include <QSharedPointer>

#include "bspstructitemtypes.h"
#include "bspstructitemtypeconverter.h"
#include "bspstructitemattributes.h"

#include "exceptions/bspstructexceptions.h"

class BSPStructGenericBlock
{
public:
    BSPStructGenericBlock(quint32 inOffset, BSPStructItemTypes::CoreItemType inType, quint32 inItemCount);

    quint32 offset() const;
    BSPStructItemTypes::CoreItemType itemType() const;
    quint32 itemSize() const;
    quint32 itemCount() const;
    quint32 totalSize() const;

    QString name() const;
    void setName(const QString& newName);

    // Only used for helping to generate display strings.
    // Corresponds to the individual item type (ie. should not be array).
    BSPStructItemTypes::PublicItemType publicItemType() const;
    void setPublicItemType(BSPStructItemTypes::PublicItemType publicType);

    QSharedPointer<BSPStructItemTypeConverter> typeConverter() const;

    BSPStructItemAttributes& attributes();
    const BSPStructItemAttributes& attributes() const;

    template<typename T>
    const T* itemPointer(const QByteArray& data, quint32 index = 0) const
    {
        quint32 sizeOfItem = itemSize();
        Q_ASSERT_X(sizeof(T) == sizeOfItem, Q_FUNC_INFO, "Size of requested type does not match item size!");

        if ( index >= m_nItemCount )
        {
            throw BSPStructOutOfRangeException(BSPStructOutOfRangeException::Type::Index,
                                               index,
                                               itemCount());
        }

        quint32 offset = m_nOffsetInStruct + (index * sizeOfItem);
        quint32 end = offset + qMax<quint32>(sizeOfItem, sizeof(T));

        if ( end > static_cast<quint32>(data.length()) )
        {
            throw BSPStructOutOfRangeException(BSPStructOutOfRangeException::Type::Offset,
                                               end,
                                               data.length());
        }

        const char* itemAsChar = data.constData() + offset;
        return reinterpret_cast<const T*>(itemAsChar);
    }

    template<typename T>
    T item(const QByteArray& data, quint32 index = 0) const
    {
        return *itemPointer<T>(data, index);
    }

private:
    quint32 m_nOffsetInStruct;
    BSPStructItemTypes::CoreItemType m_nItemType;
    BSPStructItemTypes::PublicItemType m_nPublicItemType;
    quint32 m_nItemCount;
    QString m_strName;
    QSharedPointer<BSPStructItemTypeConverter> m_pConverter;
    BSPStructItemAttributes m_Attributes;
};

#endif // BSPSTRUCTGENERICBLOCK_H
