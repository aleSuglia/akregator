/*
    This file is part of Akregator.

    Copyright (C) 2008 Frank Osterfeld <osterfeld@kde.org>

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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

    As a special exception, permission is given to link this program
    with any edition of Qt, and distribute the resulting executable,
    without including the source code for Qt in the source distribution.
*/

#include "createfoldercommand.h"

#include "folder.h"
#include "subscriptionlistview.h"

#include <QInputDialog>
#include <KLocalizedString>

#include <QTimer>

#include <cassert>

using namespace Akregator;

class CreateFolderCommand::Private
{
    CreateFolderCommand *const q;
public:
    explicit Private(CreateFolderCommand *qq);

    void doCreate();

    TreeNode *m_selectedSubscription;
    Folder *m_rootFolder;
    SubscriptionListView *m_subscriptionListView;
};

CreateFolderCommand::Private::Private(CreateFolderCommand *qq)
    : q(qq),
      m_selectedSubscription(0),
      m_rootFolder(0),
      m_subscriptionListView(0)
{

}

void CreateFolderCommand::Private::doCreate()
{
    Q_ASSERT(m_rootFolder);
    Q_ASSERT(m_subscriptionListView);
    bool ok;
    const QString name = QInputDialog::getText(q->parentWidget(), i18n("Add Folder"),
                         i18n("Folder name:"), QLineEdit::Normal,
                         QString(),
                         &ok);
    if (!ok || name.trimmed().isEmpty()) {
        q->done();
        return;
    }

    Folder *parentFolder = qobject_cast<Folder *>(m_selectedSubscription);
    if (!parentFolder) {
        parentFolder = m_selectedSubscription ? m_selectedSubscription->parent() : nullptr;
    }
    if (!parentFolder) {
        parentFolder = m_rootFolder;
    }

    TreeNode *const after = (m_selectedSubscription && m_selectedSubscription->isGroup()) ? m_selectedSubscription : nullptr;

    Folder *const newFolder = new Folder(name);
    parentFolder->insertChild(newFolder, after);
    m_subscriptionListView->ensureNodeVisible(newFolder);
    q->done();
}

CreateFolderCommand::CreateFolderCommand(QObject *parent) : Command(parent), d(new Private(this))
{

}

CreateFolderCommand::~CreateFolderCommand()
{
    delete d;
}

void CreateFolderCommand::setSubscriptionListView(SubscriptionListView *view)
{
    d->m_subscriptionListView = view;
}

void CreateFolderCommand::setSelectedSubscription(TreeNode *selected)
{
    d->m_selectedSubscription = selected;
}

void CreateFolderCommand::setRootFolder(Folder *rootFolder)
{
    d->m_rootFolder = rootFolder;
}

void CreateFolderCommand::doStart()
{
    QTimer::singleShot(0, this, SLOT(doCreate()));
}

void CreateFolderCommand::doAbort()
{

}
#include "moc_createfoldercommand.cpp"
