/***************************************************************************
 *     Copyright (C) 2004 by Sashmit Bhaduri                               *
 *     smt@vfemail.net                                                     *
 *                                                                         *
 *     Licensed under GPL.                                                 *
 ***************************************************************************/

#include "tabwidget.h"

#include <qstyle.h>
#include <qapplication.h>
#include <qiconset.h>

#include <kdebug.h>
#include <ktabwidget.h>
#include <ktabbar.h>
#include <kpopupmenu.h>
#include <krun.h>
#include <klocale.h>
#include <khtmlview.h>
#include <khtml_part.h>
#include <kiconloader.h>

using namespace Akregator;

TabWidget::TabWidget(QWidget * parent, const char *name)
        :KTabWidget(parent, name), m_CurrentMaxLength(30)
{
    setTabReorderingEnabled(true);
}

TabWidget::~TabWidget()
{
}

// copied wholesale from KonqFrameTabs

unsigned int TabWidget::tabBarWidthForMaxChars( uint maxLength )
{
    int hframe, overlap;
    hframe    = tabBar()->style().pixelMetric( QStyle::PM_TabBarTabHSpace, this );
    overlap = tabBar()->style().pixelMetric( QStyle::PM_TabBarTabOverlap, this );

    QFontMetrics fm = tabBar()->fontMetrics();
    int x = 0;
    for( int i=0; i < count(); ++i ) {
        QString newTitle;
        QDictIterator<QWidget> it( m_titleDict );
        for( ; it.current(); ++it )
        {
            if (it.current()==page(i))
            {
                newTitle=it.currentKey();
                break;
            }
        }
        if ( newTitle.length() > maxLength )
            newTitle = newTitle.left( maxLength-3 ) + "...";

        QTab* tab = tabBar()->tabAt( i );
        int lw = fm.width( newTitle );
        int iw = 0;
        if ( tab->iconSet() )
            iw = tab->iconSet()->pixmap( QIconSet::Small, QIconSet::Normal ).width() + 4;

        x += ( tabBar()->style().sizeFromContents( QStyle::CT_TabBarTab, this,                             QSize( QMAX( lw + hframe + iw, QApplication::globalStrut().width() ), 0 ), QStyleOption( tab ) ) ).width();
    }
    return x;
}

void TabWidget::setTitle( const QString &title , QWidget* sender)
{
    removeTabToolTip( sender );

    m_titleDict.insert( title, sender);

    uint lcw=0, rcw=0;
    int tabBarHeight = tabBar()->sizeHint().height();
    if ( cornerWidget( TopLeft ) && cornerWidget( TopLeft )->isVisible() )
        lcw = QMAX( cornerWidget( TopLeft )->width(), tabBarHeight );
    if ( cornerWidget( TopRight ) && cornerWidget( TopRight )->isVisible() )
        rcw = QMAX( cornerWidget( TopRight )->width(), tabBarHeight );
    uint maxTabBarWidth = width() - lcw - rcw;

    uint newMaxLength=30;
    for ( ; newMaxLength > 3; newMaxLength-- ) {
        if ( tabBarWidthForMaxChars( newMaxLength ) < maxTabBarWidth )
            break;
    }
    QString newTitle = title;
    if ( newTitle.length() > newMaxLength )
    {
        setTabToolTip( sender, newTitle );
        newTitle = newTitle.left( newMaxLength-3 ) + "...";
    }

    newTitle.replace( '&', "&&" );
    if ( tabLabel( sender ) != newTitle )
        changeTab( sender, newTitle );

    if( newMaxLength != m_CurrentMaxLength )
    {
        for( int i = 0; i < count(); ++i)
        {
            QDictIterator<QWidget> it( m_titleDict );
            for( ; it.current(); ++it )
            {
                if (it.current()==page(i))
                {
                    newTitle=it.currentKey();
                    break;
                }
            }
            removeTabToolTip( page( i ) );
            if ( newTitle.length() > newMaxLength )
            {
                setTabToolTip( page( i ), newTitle );
                newTitle = newTitle.left( newMaxLength-3 ) + "...";
            }

            newTitle.replace( '&', "&&" );
            if ( newTitle != tabLabel( page( i ) ) )
                    changeTab( page( i ), newTitle );
        }
        m_CurrentMaxLength = newMaxLength;
    }
}

void TabWidget::contextMenu(int i, const QPoint &p)
{
   currentItem = page(i);
   KPopupMenu popup;
   //popup.insertTitle(tabLabel(currentItem));
   int detachTab = popup.insertItem( SmallIcon("tab_breakoff"), i18n("Detach Tab"), this, SLOT( slotDetachTab() ) );
   //popup.insertSeparator();
   int closeTab = popup.insertItem( SmallIcon("tab_remove"), i18n("Close Tab"), this, SLOT( slotCloseTab() ) );
   if(indexOf(currentItem) == 0) { // you can't detach or close articles tab..
      popup.setItemEnabled(detachTab, false);
      popup.setItemEnabled(closeTab, false);
   }
   popup.exec(p);
}

void TabWidget::slotDetachTab()
{
   if(!currentItem) return;
   KURL url;
   if (KHTMLView *view = dynamic_cast<KHTMLView*>(currentItem)) url = view->part()->url();
   else return;
   KRun::runURL(url.prettyURL(),"text/html", false, false);
   removePage(currentItem);
}

void TabWidget::slotCloseTab()
{
   if(!currentItem) return;
   removePage(currentItem);
}
#include "tabwidget.moc"
