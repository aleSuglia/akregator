/*
   Copyright (C) 2016-2017 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "frame.h"
#include "config-akregator.h"
namespace Akregator
{
class ArticleViewerWidget;
class AKREGATOR_EXPORT MainFrame : public Frame
{
    Q_OBJECT

public:

    explicit MainFrame(QWidget *parent, QWidget *widget);
    virtual ~MainFrame();

    QUrl url() const Q_DECL_OVERRIDE;

    bool openUrl(const OpenUrlRequest &) Q_DECL_OVERRIDE {
        return false;
    }

    void setArticleViewer(Akregator::ArticleViewerWidget *articleViewer);

    qreal zoomFactor() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void slotCopyLinkAsInFrame(int frameId);
    void slotSaveLinkAsInFrame(int frameId);
    void slotZoomChangeInFrame(int frameId, qreal value);
    void slotCopyInFrame(int frameId);
    void slotPrintInFrame(int frameId);
    void slotPrintPreviewInFrame(int frameId);
    void slotTextToSpeechInFrame(int frameId);
    void slotFindTextInFrame(int frameId);
    void slotCopyImageLocationInFrame(int frameId);
    void slotSaveImageOnDiskInFrame(int frameId);
    void slotMute(int frameId, bool mute);
private:
    Akregator::ArticleViewerWidget *mArticleViewer;
};

} // namespace Akregator

#endif // MAINFRAME_H
