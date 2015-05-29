/*
  Copyright (c) 2015 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "akregratormigrateapplication.h"

#include <kdelibs4configmigrator.h>

AkregratorMigrateApplication::AkregratorMigrateApplication()
{
    initializeMigrator();
}

void AkregratorMigrateApplication::migrate()
{
    // Migrate to xdg.
    Kdelibs4ConfigMigrator migrate(QStringLiteral("akregator"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("akregatorrc") << QStringLiteral("akregator.notifyrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("akregator_shell.rc") << QStringLiteral("akregator_part.rc") << QStringLiteral("pageviewer.rc")
                       << QStringLiteral("articleviewer.rc") << QStringLiteral("akregator_sharemicroblog_plugin.rc") << QStringLiteral("akregator_onlinesync_plugin.rc"));
    migrate.migrate();

    // Migrate folders and files.
    if (mMigrator.checkIfNecessary()) {
        mMigrator.start();
    }
}

void AkregratorMigrateApplication::initializeMigrator()
{
    const int currentVersion = 1;

    mMigrator.setApplicationName(QStringLiteral("akreagator"));
    mMigrator.setConfigFileName(QStringLiteral("akregatorrc"));
    mMigrator.setCurrentConfigVersion(currentVersion);
    //TODO
}