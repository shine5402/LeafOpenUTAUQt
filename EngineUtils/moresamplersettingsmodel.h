﻿#ifndef MORESAMPLERSETTINGSMODEL_H
#define MORESAMPLERSETTINGSMODEL_H

#include <QAbstractTableModel>
#include "moresamplerconfigreader.h"
class MoresamplerSettingsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit MoresamplerSettingsModel(MoresamplerConfigReader *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    struct TableColumnsGlobal{
        constexpr static int Count = 5;
        constexpr static int Name = 0;
        constexpr static int Value = 1;
        constexpr static int Type = 2;
        constexpr static int Override = 3;
        constexpr static int Information = 4;
    };
    struct TableColumnsVoicebank{
        constexpr static int Count = 4;
        constexpr static int Name = 0;
        constexpr static int Value = 1;
        constexpr static int Type = 2;
        constexpr static int Information = 3;
    };

private:
    MoresamplerConfigReader* configReader = nullptr;
};

#endif // MORESAMPLERSETTINGSMODEL_H
