/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QPainter>
#include <QDebug>
#include <QVBoxLayout>
#include <QAction>
#include <QToolBar>

#include "libGitWrap/Reference.h"
#include "libGitWrap/ObjectCommit.h"

#include "Diff/RawView/DiffRawView.h"

#include "libHeaven/Widgets/MiniSplitter.h"

#include "MacGitver/MacGitver.h"

#include "HistoryView.h"
#include "HistoryEntry.h"
#include "HistoryModel.h"
#include "HistoryBuilder.h"
#include "HistoryDiff.h"
#include "HistoryList.h"
#include "HistoryListDelegate.h"
#include "HistoryDetails.h"

HistoryView::HistoryView()
    : GlobalView( QLatin1String( "History" ) )
    , ConfigUser( "History" )
{
    setupActions( this );

    mVertSplit = mHorzSplit = NULL;

    setViewName( trUtf8( "History" ) );
    setSizePolicy( QSizePolicy::MinimumExpanding,
                   QSizePolicy::MinimumExpanding );

    mList = new HistoryList;
    mList->setFrameShape( QFrame::NoFrame );

    mModel = new HistoryModel( this );
    mList->setModel( mModel );

    connect( mList, SIGNAL(currentCommitChanged(Git::ObjectId)),
             this, SLOT(currentCommitChanged(Git::ObjectId)) );

    mList->setItemDelegate( new HistoryListDelegate );

    mDetails = new HistoryDetails;

    mDiff = new HistoryDiff;

    QVBoxLayout* l = new QVBoxLayout;
    l->setSpacing( 0 );
    l->setMargin( 0 );
    l->addWidget( tbHistoryViewToolBar->toolBarFor( this ) );
    setLayout( l );
    initSplitters();

    mBuilder = NULL;

    connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
             this, SLOT(repositoryChanged(Git::Repository)) );

    Git::Repository repo = MacGitver::self().repository();
    if( repo.isValid() )
    {
        repositoryChanged( repo );
    }
}

void HistoryView::repositoryChanged( Git::Repository repo )
{
    if( mBuilder )
    {
        delete mBuilder;
        mBuilder = NULL;
    }

    mRepo = repo;
    mModel->setRepository( repo );
    mDetails->setRepository( repo );
    mDiff->setRepository( repo );

    if( mRepo.isValid() )
    {
        buildHistory();
    }
}

void HistoryView::buildHistory()
{
    if( !mRepo.isValid() )
    {
        return;
    }

    if( !mBuilder )
    {
        mBuilder = new HistoryBuilder( mRepo, mModel );
    }

    mBuilder->addHEAD();
    mBuilder->start();
}

void HistoryView::currentCommitChanged( const Git::ObjectId& sha1 )
{
    mDetails->setCommit( sha1 );
    mDiff->setCommitId( sha1 );
}

void HistoryView::configChanged( const QString& subPath, const QVariant& value )
{
    if( subPath == QLatin1String( "SplitLayout" ) )
    {
        initSplitters();
    }
}

void HistoryView::initSplitters()
{
    if( !mVertSplit )
    {
        mVertSplit = new Heaven::MiniSplitter( Qt::Vertical );
    }
    else
    {
        mVertSplit->hide();
        mVertSplit->setParent( NULL );
    }

    if( !mHorzSplit )
    {
        mHorzSplit = new Heaven::MiniSplitter( Qt::Horizontal );
    }
    else
    {
        mHorzSplit->hide();
        mHorzSplit->setParent( NULL );
    }

    mDiff->hide();
    mDiff->setParent( NULL );

    mList->hide();
    mList->setParent( NULL );

    mDetails->hide();
    mDetails->setParent( NULL );

    int i = configGet( "SplitLayout", 0 );
    switch( i )
    {
    default:
    case 0:
        mHorzSplit->addWidget( mDetails );
        mHorzSplit->addWidget( mDiff );
        mVertSplit->addWidget( mList );
        mVertSplit->addWidget( mHorzSplit );
        layout()->addWidget( mVertSplit );
        break;
    case 1:
        mVertSplit->addWidget( mDetails );
        mVertSplit->addWidget( mDiff );
        mHorzSplit->addWidget( mList );
        mHorzSplit->addWidget( mVertSplit );
        layout()->addWidget( mHorzSplit );
        break;
    case 2:
        mVertSplit->addWidget( mList );
        mVertSplit->addWidget( mDetails );
        mHorzSplit->addWidget( mVertSplit );
        mHorzSplit->addWidget( mDiff );
        layout()->addWidget( mHorzSplit );
        break;
    }

    mVertSplit->show();
    mHorzSplit->show();
    mDiff->show();
    mList->show();
    mDetails->show();
}
