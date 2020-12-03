// SPDX-FileCopyrightText: 2020 Henri Chain <henri.chain@enioka.com>
// SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
//
// SPDX-License-Identifier: LGPL-2.1-or-later

#include "kapplicationscope.h"
#include "kapplicationscope_p.h"
#include "kcgroups_debug.h"
#include "managerinterface.h"
#include <limits>

static const Property<OptionalQULongLong> cpuQuotaProp = {&KApplicationScopePrivate::m_cpuQuota,
                                                          &KApplicationScope::cpuQuotaChanged,
                                                          QStringLiteral("CPUQuotaPerSecUSec"),
                                                          true,
                                                          std::numeric_limits<qulonglong>::max()};

static const Property<OptionalQULongLong> cpuQuotaPeriodProp = {&KApplicationScopePrivate::m_cpuQuotaPeriod,
                                                                &KApplicationScope::cpuQuotaPeriodChanged,
                                                                QStringLiteral("CPUQuotaPeriodUSec"),
                                                                true,
                                                                std::numeric_limits<qulonglong>::max()};

static const Property<OptionalQULongLong> cpuWeightProp = {&KApplicationScopePrivate::m_cpuWeight,
                                                           &KApplicationScope::cpuWeightChanged,
                                                           QStringLiteral("CPUWeight"),
                                                           true,
                                                           std::numeric_limits<qulonglong>::max()};

static const Property<OptionalQULongLong> ioWeightProp = {&KApplicationScopePrivate::m_ioWeight,
                                                          &KApplicationScope::ioWeightChanged,
                                                          QStringLiteral("IOWeight"),
                                                          true,
                                                          std::numeric_limits<qulonglong>::max()};

static const Property<OptionalQULongLong> memoryLowProp = {&KApplicationScopePrivate::m_memoryLow,
                                                           &KApplicationScope::memoryLowChanged,
                                                           QStringLiteral("MemoryLow"),
                                                           true,
                                                           std::numeric_limits<qulonglong>::max()};

static const Property<OptionalQULongLong> memoryHighProp = {&KApplicationScopePrivate::m_memoryHigh,
                                                            &KApplicationScope::memoryHighChanged,
                                                            QStringLiteral("MemoryHigh"),
                                                            true,
                                                            std::numeric_limits<qulonglong>::max()};

static const Property<OptionalQULongLong> memoryMinProp = {&KApplicationScopePrivate::m_memoryMin,
                                                           &KApplicationScope::memoryMinChanged,
                                                           QStringLiteral("MemoryMin"),
                                                           true,
                                                           std::numeric_limits<qulonglong>::max()};

static const Property<OptionalQULongLong> memoryMaxProp = {&KApplicationScopePrivate::m_memoryMax,
                                                           &KApplicationScope::memoryMaxChanged,
                                                           QStringLiteral("MemoryMax"),
                                                           true,
                                                           std::numeric_limits<qulonglong>::max()};

static const Property<OptionalQULongLong> memorySwapMaxProp = {&KApplicationScopePrivate::m_memorySwapMax,
                                                               &KApplicationScope::memorySwapMaxChanged,
                                                               QStringLiteral("MemorySwapMax"),
                                                               true,
                                                               std::numeric_limits<qulonglong>::max()};

static const QHash<QString, const Property<OptionalQULongLong> *> qullProps = {
    {cpuQuotaProp.systemdName, &cpuQuotaProp},
    {cpuQuotaPeriodProp.systemdName, &cpuQuotaPeriodProp},
    {cpuWeightProp.systemdName, &cpuWeightProp},
    {ioWeightProp.systemdName, &ioWeightProp},
    {memoryLowProp.systemdName, &memoryLowProp},
    {memoryHighProp.systemdName, &memoryHighProp},
    {memoryMinProp.systemdName, &memoryMinProp},
    {memoryMaxProp.systemdName, &memoryMaxProp},
    {memorySwapMaxProp.systemdName, &memorySwapMaxProp},
};

KApplicationScope::KApplicationScope(const QString &path, const QString &id, QObject *parent)
    : QObject(parent)
    , d_ptr(new KApplicationScopePrivate(path, id, this))
{
}

KApplicationScope::KApplicationScope(const QString &path, QObject *parent)
    : KApplicationScope::KApplicationScope(path, QString(), parent)
{
}

QString KApplicationScope::path() const
{
    return d_ptr->m_path;
}

QString KApplicationScope::id() const
{
    return d_ptr->m_id;
}

QString KApplicationScope::cgroup() const
{
    return d_ptr->m_cgroup;
}

QString KApplicationScope::description() const
{
    return d_ptr->m_description;
}

QString KApplicationScope::desktopName() const
{
    return d_ptr->m_desktopName;
}

QString KApplicationScope::desktopFilePath() const
{
    return d_ptr->m_desktopFilePath;
}

QString KApplicationScope::instance() const
{
    return d_ptr->m_instance;
}

KApplicationScope::ErrorCode KApplicationScope::lastError() const
{
    return d_ptr->m_lastError;
}

OptionalQULongLong KApplicationScope::cpuQuota() const
{
    return d_ptr->getProperty<OptionalQULongLong>(cpuQuotaProp);
}

OptionalQULongLong KApplicationScope::cpuQuotaPeriod() const
{
    return d_ptr->getProperty<OptionalQULongLong>(cpuQuotaPeriodProp);
}

OptionalQULongLong KApplicationScope::cpuWeight() const
{
    return d_ptr->getProperty<OptionalQULongLong>(cpuWeightProp);
}

OptionalQULongLong KApplicationScope::ioWeight() const
{
    return d_ptr->getProperty<OptionalQULongLong>(ioWeightProp);
}

OptionalQULongLong KApplicationScope::memoryLow() const
{
    return d_ptr->getProperty<OptionalQULongLong>(memoryLowProp);
}

OptionalQULongLong KApplicationScope::memoryHigh() const
{
    return d_ptr->getProperty<OptionalQULongLong>(memoryHighProp);
}

OptionalQULongLong KApplicationScope::memoryMin() const
{
    return d_ptr->getProperty<OptionalQULongLong>(memoryMinProp);
}

OptionalQULongLong KApplicationScope::memoryMax() const
{
    return d_ptr->getProperty<OptionalQULongLong>(memoryMaxProp);
}

OptionalQULongLong KApplicationScope::memorySwapMax() const
{
    return d_ptr->getProperty<OptionalQULongLong>(memorySwapMaxProp);
}

void KApplicationScope::setCpuQuota(const OptionalQULongLong &quota)
{
    d_ptr->trySetProperty<OptionalQULongLong>(cpuQuotaProp, quota);
}

void KApplicationScope::setCpuQuotaPeriod(const OptionalQULongLong &period)
{
    d_ptr->trySetProperty<OptionalQULongLong>(cpuQuotaPeriodProp, period);
}

void KApplicationScope::setCpuWeight(const OptionalQULongLong &weight)
{
    d_ptr->trySetProperty<OptionalQULongLong>(cpuWeightProp, weight);
}

void KApplicationScope::setIoWeight(const OptionalQULongLong &weight)
{
    d_ptr->trySetProperty<OptionalQULongLong>(ioWeightProp, weight);
}

void KApplicationScope::setMemoryLow(const OptionalQULongLong &memoryLow)
{
    d_ptr->trySetProperty<OptionalQULongLong>(memoryLowProp, memoryLow);
}

void KApplicationScope::setMemoryHigh(const OptionalQULongLong &memoryHigh)
{
    d_ptr->trySetProperty<OptionalQULongLong>(memoryHighProp, memoryHigh);
}

void KApplicationScope::setMemoryMin(const OptionalQULongLong &memoryMin)
{
    d_ptr->trySetProperty<OptionalQULongLong>(memoryMinProp, memoryMin);
}

void KApplicationScope::setMemoryMax(const OptionalQULongLong &memoryMax)
{
    d_ptr->trySetProperty<OptionalQULongLong>(memoryMaxProp, memoryMax);
}

void KApplicationScope::setMemorySwapMax(const OptionalQULongLong &memorySwapMax)
{
    d_ptr->trySetProperty<OptionalQULongLong>(memorySwapMaxProp, memorySwapMax);
}

void KApplicationScope::stop()
{
    d_ptr->stop();
}

KApplicationScope::~KApplicationScope()
{
    delete d_ptr;
}

static const auto systemd1 = QStringLiteral("org.freedesktop.systemd1");
static const auto systemd1Scope = QStringLiteral("org.freedesktop.systemd1.Scope");
static const auto systemd1Service = QStringLiteral("org.freedesktop.systemd1.Service");
static const auto systemd1Slice = QStringLiteral("org.freedesktop.systemd1.Slice");
static const auto systemd1Unit = QStringLiteral("org.freedesktop.systemd1.Unit");
static const auto systemd1Path = QStringLiteral("/org/freedesktop/systemd1");

KApplicationScope *KApplicationScope::fromPid(uint pid, QObject *parent)
{
    org::freedesktop::systemd1::Manager manager(systemd1, systemd1Path, QDBusConnection::sessionBus());
    auto reply = manager.GetUnitByPID(pid);
    reply.waitForFinished();

    if (reply.isError()) {
        qCWarning(KCGROUPS_LOG) << "Cannot get app from pid:" << reply.error().message();
        return nullptr;
    } else {
        return new KApplicationScope(reply.argumentAt<0>().path(), parent);
    }
}

KApplicationScopePrivate::KApplicationScopePrivate(const QString &path, const QString &id, KApplicationScope *parent)
    : m_lastError(KApplicationScope::NoError)
    , m_path(path)
    , m_id(id)
    , q_ptr(parent)
    , m_unit(new org::freedesktop::systemd1::Unit(systemd1, path, QDBusConnection::sessionBus(), q_ptr))
    , m_properties(new org::freedesktop::DBus::Properties(systemd1, path, QDBusConnection::sessionBus(), q_ptr))
{
    parseId();
    qDBusRegisterMetaType<QVariantMultiMap>();
    qDBusRegisterMetaType<QVariantMultiItem>();

    // Try to fill cache for all properties.
    const auto interface = path.endsWith(QStringLiteral("_2escope")) ? systemd1Scope
        : path.endsWith(QStringLiteral("_2eslice"))                  ? systemd1Slice
                                                                     : systemd1Service;
    const auto *getAllWatcher = new QDBusPendingCallWatcher(m_properties->GetAll(interface), q_ptr);
    QObject::connect(getAllWatcher, &QDBusPendingCallWatcher::finished, q_ptr, [this](QDBusPendingCallWatcher *w) {
        handleGetAllCallFinished(w);
    });

    const auto *unitGetAllWatcher = new QDBusPendingCallWatcher(m_properties->GetAll(systemd1Unit));
    QObject::connect(unitGetAllWatcher, &QDBusPendingCallWatcher::finished, q_ptr, [this](QDBusPendingCallWatcher *w) {
        handleGetUnitCallFinished(w);
    });
}

static const QRegularExpression appPattern(QStringLiteral("^apps?-(.+?)(?:-([^-]+))?\\.(scope|service|slice)$"));

void KApplicationScopePrivate::parseId()
{
    const auto match = appPattern.match(m_id);
    if (match.hasMatch()) {
        auto name = match.captured(1);
        static const QRegularExpression escaped(QStringLiteral("\\\\x([0-9a-f]{2})"));
        auto escapedBytes = escaped.globalMatch(name);
        int offset = 0;
        while (escapedBytes.hasNext()) {
            bool ok;
            const auto escapedMatch = escapedBytes.next();
            const char byte = escapedMatch.captured(1).toUInt(&ok, 16);
            if (ok) {
                name.replace(offset + escapedMatch.capturedStart(), escapedMatch.capturedLength(), QLatin1Char(byte));
                offset -= escapedMatch.capturedLength() - 1;
            }
        }
        m_desktopName = name;
        m_instance = match.captured(2);
    }
}

template<typename T>
T KApplicationScopePrivate::nullIfDefault(const Property<T> &prop, const QVariant &variant)
{
    using value_type = typename T::value_type;
    const auto value = variant.value<value_type>();
    if (variant.isNull() || value == prop.defaultValue) {
        return T();
    }
    return value;
}

template<typename T>
QVariant KApplicationScopePrivate::defaultIfNull(const Property<T> &prop, const T &opt)
{
    // Convert null value to default if there is one
    return opt ? *opt : prop.hasDefault ? prop.defaultValue : QVariant();
}

template<typename T>
T KApplicationScopePrivate::getProperty(const Property<T> &prop)
{
    return this->*prop.privateMember;
}

template<typename T>
void KApplicationScopePrivate::saveProperty(const Property<T> &prop, const T &opt)
{
    this->*prop.privateMember = opt;
    emit(q_ptr->*prop.changedSignal)(opt);
    emit q_ptr->propertyChanged(prop.systemdName);
}

template<typename T>
void KApplicationScopePrivate::saveIfNull(const Property<T> &prop, const QVariant &variant)
{
    const auto opt = nullIfDefault(prop, variant);
    const auto cur = this->*prop.privateMember;
    // Avoid race condition when property was set in the meantime
    if (!cur && opt != cur) {
        saveProperty(prop, opt);
    }
}

template<typename T>
void KApplicationScopePrivate::trySetProperty(const Property<T> &prop, T opt)
{
    if (this->*prop.privateMember != opt) {
        saveProperty(prop, opt);
        const auto reply = m_unit->SetProperties(true, {{prop.systemdName, defaultIfNull(prop, opt)}});
        const auto *watcher = new QDBusPendingCallWatcher(reply, q_ptr);
        QObject::connect(watcher, &QDBusPendingCallWatcher::finished, q_ptr, [this](QDBusPendingCallWatcher *watcher) {
            handleVoidCallFinished(watcher, KApplicationScope::SetFailedError);
        });
    }
}

void KApplicationScopePrivate::stop()
{
    const auto *watcher = new QDBusPendingCallWatcher(m_unit->Stop(QStringLiteral("replace")), q_ptr);
    QObject::connect(watcher, &QDBusPendingCallWatcher::finished, q_ptr, [this](QDBusPendingCallWatcher *watcher) {
        handleVoidCallFinished(watcher, KApplicationScope::StopFailedError);
    });
}

void KApplicationScopePrivate::handleVoidCallFinished(QDBusPendingCallWatcher *call, KApplicationScope::ErrorCode code)
{
    QDBusPendingReply<> reply = *call;
    if (reply.isError()) {
        setError(code, reply.error().message());
    } else {
        qCDebug(KCGROUPS_LOG) << "void call finished async";
    }
    call->deleteLater();
}

void KApplicationScopePrivate::handleGetAllCallFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QVariantMap> reply = *call;
    if (reply.isError()) {
        setError(KApplicationScope::CacheFillError, reply.error().message());
    } else {
        qCDebug(KCGROUPS_LOG) << "getall finished async";
        const QVariantMap properties = reply.argumentAt<0>();
        for (auto kv = properties.constKeyValueBegin(); kv != properties.constKeyValueEnd(); kv++) {
            auto k = (*kv).first;
            auto v = (*kv).second;
            if (qullProps.contains(k)) {
                saveIfNull<OptionalQULongLong>(*qullProps[k], v);
            } else if (k == QStringLiteral("ControlGroup")) {
                m_cgroup = QStringLiteral("/sys/fs/cgroup/systemd") + v.toString();
                emit q_ptr->cgroupChanged(m_cgroup);
                emit q_ptr->propertyChanged(k);
            }
        }
    }
    call->deleteLater();
}

void KApplicationScopePrivate::handleGetUnitCallFinished(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QVariantMap> reply = *call;
    if (reply.isError()) {
        setError(KApplicationScope::CacheFillError, reply.error().message());
    } else {
        qCDebug(KCGROUPS_LOG) << "getall unit finished async";
        const QVariantMap properties = reply.argumentAt<0>();
        for (auto kv = properties.constKeyValueBegin(); kv != properties.constKeyValueEnd(); kv++) {
            auto k = (*kv).first;
            auto v = (*kv).second.toString();
            if (k == QStringLiteral("Id")) {
                // If id not passed as constructor argument
                if (m_id.isNull()) {
                    m_id = v;
                    parseId();
                    emit q_ptr->idChanged(m_id);
                    emit q_ptr->propertyChanged(k);
                    if (!m_desktopName.isNull()) {
                        emit q_ptr->desktopNameChanged(m_desktopName);
                    }
                }
            } else if (k == QStringLiteral("Description")) {
                if (v != m_id) {
                    m_description = v;
                    emit q_ptr->descriptionChanged(v);
                    emit q_ptr->propertyChanged(k);
                }
            } else if (k == QStringLiteral("SourcePath")) {
                if (v.endsWith(QStringLiteral(".desktop"))) {
                    m_desktopFilePath = v;
                    m_desktopName = QFileInfo(v).fileName().chopped(strlen(".desktop"));
                    emit q_ptr->desktopFilePathChanged(v);
                    emit q_ptr->propertyChanged(k);
                    emit q_ptr->desktopNameChanged(m_desktopName);
                }
            }
        }
    }
    call->deleteLater();
}

void KApplicationScopePrivate::setError(KApplicationScope::ErrorCode code, const QString &message, const bool warning)
{
    m_lastError = code;
    if (warning) {
        qCWarning(KCGROUPS_LOG) << message;
    } else {
        qCDebug(KCGROUPS_LOG) << "ERROR: " << message;
    }
    emit q_ptr->errorOccurred(m_lastError);
}
