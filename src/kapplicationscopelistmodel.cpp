// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "kapplicationscopelistmodel.h"
#include "kapplicationscopelistmodel_p.h"
#include "kcgroups_debug.h"
#include <QDebug>
#include <algorithm>

KApplicationScopeListModel::KApplicationScopeListModel(QObject *parent)
    : KApplicationScopeListModel(nullptr, parent)
{
}

KApplicationScopeListModel::KApplicationScopeListModel(KApplicationScopeLister *lister, QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new KApplicationScopeListModelPrivate(lister, this))
{
}

KApplicationScopeListModel::~KApplicationScopeListModel()
{
    delete d_ptr;
}

QVariant KApplicationScopeListModel::data(const QModelIndex &index, int role) const
{
    const auto row = index.row();
    if (row < d_ptr->m_apps.length()) {
        auto *app = d_ptr->m_apps[row];
        switch (role) {
        case Qt::DisplayRole:
            return !app->description().isEmpty() ? app->description() : !app->desktopName().isEmpty() ? app->desktopName() : app->id();
        case ObjectRole:
            return QVariant::fromValue(app);
        }
    }
    return QVariant();
}

int KApplicationScopeListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return d_ptr->m_apps.size();
}

QHash<int, QByteArray> KApplicationScopeListModel::roleNames() const
{
    return {{Qt::DisplayRole, "display"}, {ObjectRole, "object"}};
}

KApplicationScopeListModelPrivate::KApplicationScopeListModelPrivate(KApplicationScopeLister *lister,
                                                                     KApplicationScopeListModel *parent)
    : m_apps({})
    , q_ptr(parent)
    , m_lister(lister == nullptr ? new KApplicationScopeLister(parent) : lister)
{
    QObject::connect(m_lister,
                     &KApplicationScopeLister::pathAdded,
                     q_ptr,
                     [this](const QString &path, const QString &id) { handleNewApp(path, id); });
    QObject::connect(m_lister, &KApplicationScopeLister::pathRemoved, q_ptr, [this](const QString &path) {
        handleRemovedApp(path);
    });
}

void KApplicationScopeListModelPrivate::handleNewApp(const QString &path, const QString &id)
{
    const auto row = m_apps.size();
    q_ptr->beginInsertRows(QModelIndex(), row, row);
    const auto app = new KApplicationScope(path, id, q_ptr);
    QObject::connect(app, &KApplicationScope::propertyChanged, q_ptr, [this, app]() {
        const auto row = m_apps.indexOf(app);
        if (row >= 0) {
            const auto idx = q_ptr->index(row, 0);
            emit q_ptr->dataChanged(idx, idx);
        }
    });
    m_apps.append(app);
    q_ptr->endInsertRows();
}

void KApplicationScopeListModelPrivate::handleRemovedApp(const QString &path)
{
    const auto it = std::find_if(
        m_apps.begin(), m_apps.end(), [&path](const KApplicationScope *app) { return app->path() == path; });
    if (it != m_apps.end()) {
        const auto *app = *it;
        const auto row = static_cast<int>(std::distance(m_apps.begin(), it));
        q_ptr->beginRemoveRows(QModelIndex(), row, row);
        delete app;
        m_apps.removeAt(row);
        q_ptr->endRemoveRows();
    } else {
        qCDebug(KCGROUPS_LOG) << "app not found:" << path;
    }
}
