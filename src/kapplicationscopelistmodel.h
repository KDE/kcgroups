// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef KAPPLICATIONSCOPELISTMODEL_H
#define KAPPLICATIONSCOPELISTMODEL_H

#include "kapplicationscope.h"
#include "kapplicationscopelister.h"
#include "kcgroups_export.h"
#include <QAbstractListModel>

class KApplicationScopeListModelPrivate;

/**
 * @brief A ListModel for use in QML to interact with an updated list of KApplicationScope's.
 * Implements QAbstractListModel
 * @implements QAbstractListModel
 */
class KCGROUPS_EXPORT KApplicationScopeListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /**
     * @brief The exposed data roles
     */
    enum KApplicationScopeRoles {
        /**
         * Corresponds to KApplicationScope object
         */
        ObjectRole = Qt::UserRole + 1,
    };
    Q_ENUM(KApplicationScopeRoles)

    explicit KApplicationScopeListModel(QObject *parent = nullptr);

    /**
     * @brief Used when a KApplicationScopeLister is already on hand
     * @param lister
     * @param parent
     */
    explicit KApplicationScopeListModel(KApplicationScopeLister *lister, QObject *parent = nullptr);
    ~KApplicationScopeListModel() override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    KApplicationScopeListModelPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(KApplicationScopeListModel)
};

#endif // KAPPLICATIONSCOPELISTMODEL_H
