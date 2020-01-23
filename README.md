# KCGroups

This library allows easy access to currently running applications and their control groups resources, using the [systemd dbus API](https://www.freedesktop.org/wiki/Software/systemd/dbus/).

More info here: https://www.freedesktop.org/software/systemd/man/systemd.resource-control.html

# Introduction

To control an application's cgroup resources, use KApplicationScope.

To find a list of currently running applications, use KApplicationScopeLister.

For use in QML, KApplicationScopeListModel is provided to interact with an updated list of KApplicationScope's.

[List of Classes](annotated.html)

## Requirements

- systemd >= 238 (for neon I used this PPA: https://launchpad.net/~ddstreet/+archive/ubuntu/systemd-upstream)
- `systemd.unified_cgroup_hierarchy=1` on the kernel command line (in /etc/default/grub)
- `Delegate=cpu` in `/etc/systemd/system/user@.service.d/override.conf` to be able to control CPU resources from user systemd
- install `dbus-user-session` to get the user systemd instance to connect to the session bus
- run applications in systemd scopes by starting them with `systemd-run --user --scope --unit org.kde.krun-<app_name>-<uuid>.desktop -- <app>`. There is a currently a patch to let `KRun` launch apps this way
