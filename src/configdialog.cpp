/*
    This file is part of Akregator.

    Copyright (C) 2005 Frank Osterfeld <frank.osterfeld@kdemail.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

    As a special exception, permission is given to link this program
    with any edition of Qt, and distribute the resulting executable,
    without including the source code for Qt in the source distribution.
*/

#include <qstringlist.h>

#include <kcombobox.h>
#include <klocale.h>

#include "configdialog.h"

#include "akregatorconfig.h"
#include "settings_advanced.h"
#include "settings_appearance.h"
#include "settings_archive.h"
#include "settings_browser.h"
#include "settings_general.h"

namespace Akregator
{

ConfigDialog::ConfigDialog(QWidget* parent, const char* name, KConfigSkeleton* config, DialogType dialogType, int dialogButtons, ButtonCode defaultButton, bool modal) : KConfigDialog(parent, name, config, dialogType, dialogButtons, defaultButton, modal)
{
    addPage(new SettingsGeneral(this, "General"), i18n("General"), "package_settings");
    addPage(new SettingsArchive(this, "Archive"), i18n("Archive"), "package_settings");
    addPage(new SettingsAppearance(this, "Appearance"), i18n("Appearance"), "fonts");
    addPage(new SettingsBrowser(this, "Browser"), i18n("Browser"), "package_network");
    m_settingsAdvanced = new SettingsAdvanced(this, "Advanced");
    addPage(m_settingsAdvanced, i18n("Advanced"), "package_network");
    m_settingsAdvanced->selectFactory(Settings::archiveBackend());
}

void ConfigDialog::updateSettings()
{
    Settings::setArchiveBackend(m_settingsAdvanced->selectedFactory());
    KConfigDialog::updateSettings();
}
        
void ConfigDialog::updateWidgets()
{
    m_settingsAdvanced->selectFactory(Settings::archiveBackend());
    KConfigDialog::updateSettings();
}
        
ConfigDialog::~ConfigDialog() {}

} // namespace Akregator

//#include "configdialog.moc"
