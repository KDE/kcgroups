// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#ifndef KAPPLICATIONSCOPELISTER_H
#define KAPPLICATIONSCOPELISTER_H

#include "kcgroups_export.h"
#include <QObject>

class KApplicationScopeListerPrivate;

/**
 * @brief Keeps an updated list of desktop application systemd scopes
 */
class KCGROUPS_EXPORT KApplicationScopeLister : public QObject
{
    Q_OBJECT

    /**
     * @brief code of the last error that occurred (NoError if none)
     * @accessors lastError()
     * @notifySignal errorOccurred
     */
    Q_PROPERTY(ErrorCode lastError READ lastError NOTIFY errorOccurred)

    /**
     * @brief list of dbus paths that corresponds to currently running applications
     * @accessors paths()
     * @notifySignal pathsChanged()
     */
    Q_PROPERTY(QStringList paths READ paths NOTIFY pathsChanged)

public:
    /**
     * @brief The types of error that can occur
     */
    enum ErrorCode {
        /**
         * Default value. no error occurred
         */
        NoError,

        /**
         * Error while listing applications
         */
        CannotListError,

        /**
         * Error while trying to subscribe to systemd manager signals
         */
        CannotSubscribeError
    };
    Q_ENUM(ErrorCode)

    explicit KApplicationScopeLister(QObject *parent = nullptr);
    ~KApplicationScopeLister() override;

    QStringList paths() const;
    ErrorCode lastError() const;

Q_SIGNALS:
    /**
     * @brief emitted when there is an error
     * @param lastError: the error code
     */
    void errorOccurred(ErrorCode lastError);

    /**
     * @brief emiited when a path is added or removed
     * @param paths: the new list of paths
     */
    void pathsChanged(const QStringList &paths);

    /**
     * @brief emitted when a new application is launched
     * @param path: dbus path of new application
     * @param id: unit id of new application
     */
    void pathAdded(const QString &path, const QString &id);

    /**
     * @brief emitted when an application is stopped
     * @param path: dbus path of stopped application
     */
    void pathRemoved(const QString &path);

private:
    KApplicationScopeListerPrivate *const d;
};

#endif // KAPPLICATIONSCOPELISTER_H
