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

#include <QStringBuilder>
#include <QToolBar>
#include <QAction>
#include <QTreeView>
#include <QHeaderView>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

#include "libGitWrap/Result.h"
#include "libGitWrap/Index.h"
#include "libGitWrap/Reference.h"
#include "libGitWrap/ObjectTree.h"
#include "libGitWrap/DiffList.h"

#include "Diff/Model/GitPatchConsumer.h"
#include "Diff/RawView/DiffRawView.h"

#include "Widgets/SHA1Input.h"
#include "Widgets/ShortCommitModel.h"
#include "Widgets/FlatTreeComboBox.h"

#include "MacGitver/MacGitver.h"

#include "HistoryDiff.h"

HistoryDiff::HistoryDiff()
{
    mRawView = new DiffRawView();

    QVBoxLayout* l = new QVBoxLayout;
    l->setMargin( 0 );
    l->setSpacing( 0 );
    setLayout( l );

    mToolbar = new QToolBar;

    mDiffTo = new QComboBox();
    mDiffTo->addItem( trUtf8( "Workingtree" ), DTT_WT );
    mDiffTo->addItem( trUtf8( "Index" ), DTT_Index );
    mDiffTo->addItem( trUtf8( "Workingtree + Index" ), DTT_WT_and_Index );
    mDiffTo->addItem( trUtf8( "HEAD" ), DTT_HEAD );
    //mDiffTo->addItem( trUtf8( "SHA-1" ), DTT_AnySHA1 );
    mDiffTo->addItem( trUtf8( "Branch" ), DTT_Branch );
    mDiffTo->addItem( trUtf8( "Tag" ), DTT_Tag );
    mDiffTo->addItem( trUtf8( "All parents" ), DTT_AllParents );
    mDiffTo->addItem( trUtf8( "Parent" ), DTT_Parent );

    mToolbar->addWidget( new QLabel( trUtf8( "Diff to:" ) ) );
    mToolbar->addWidget( mDiffTo );

    mDiffToBranch = new FlatTreeComboBox;
    mDiffToActions.insert( DTT_Branch, mToolbar->addWidget( mDiffToBranch ) );

    mDiffToTag = new FlatTreeComboBox;
    mDiffToActions.insert( DTT_Tag, mToolbar->addWidget( mDiffToTag ) );

    mDiffToParent = new QComboBox;

    mParentsModel = new ShortCommitModel( this );
    mDiffToParent->setModel( mParentsModel );
    mParentsList = new QTreeView;
    mParentsList->setHeaderHidden( true );
    mParentsList->setRootIsDecorated( false );

    #if QT_VERSION < 0x050000
    // QT5: Recheck this.
    mParentsList->header()->setResizeMode( QHeaderView::ResizeToContents );
    #endif

    mParentsList->setModel( mParentsModel );
    mDiffToParent->setView( mParentsList );

    mDiffToActions.insert( DTT_Parent, mToolbar->addWidget( mDiffToParent ) );

    mDiffToSha1 = new SHA1Input;
    mDiffToActions.insert( DTT_AnySHA1, mToolbar->addWidget( mDiffToSha1 ) );

    foreach( QAction* act, mDiffToActions )
    {
        act->setVisible( false );
    }

    connect( mDiffTo, SIGNAL(currentIndexChanged(int)),
             this, SLOT(onDiffToChanged(int)) );

    connect( mDiffToParent, SIGNAL(currentIndexChanged(int)),
             this, SLOT(createPatch()) );

    connect( mDiffToBranch, SIGNAL(currentIndexChanged(int)),
             this, SLOT(createPatch()) );

    connect( mDiffToTag, SIGNAL(currentIndexChanged(int)),
             this, SLOT(createPatch()) );

    l->addWidget( mToolbar );
    l->addWidget( mRawView );
    setLayout( l );
}

void HistoryDiff::setRepository( Git::Repository repo )
{
    Git::Result r;

    mRepo = repo;

    if( mRepo.isValid() )
    {

        QStringList list = mRepo.allBranches( r );
        if( r )
        {
            mDiffToBranch->clear();
            mDiffToBranch->add( list );
        }
        else
        {
            r.clear();
        }

        list = mRepo.allTags( r );
        if( r )
        {
            mDiffToTag->clear();
            mDiffToTag->add( list );
        }
        else
        {
            r.clear();
        }

    }

}

void HistoryDiff::setCommitId( const Git::ObjectId& sha1 )
{
    setPatch( Patch::Ptr() );

    if( sha1.isNull() )
    {
        return;
    }

    Git::Result r;

    mCommit = mRepo.lookupCommit( sha1, r );
    mRepo.index( r ).read( r );

    mParentsModel->setCommitList( mCommit.parentCommits( r ) );

    if( !r )
    {
        MacGitver::self().log( ltError, r );
        return;
    }

    createPatch();
}

void HistoryDiff::onDiffToChanged( int index )
{
    if( index == -1 )
    {
        setPatch( Patch::Ptr() );
        return;
    }

    DiffToTypes dtt = DiffToTypes( mDiffTo->itemData( index ).toInt() );
    foreach( DiffToTypes type, mDiffToActions.keys() )
    {
        mDiffToActions[ type ]->setVisible( dtt == type );
    }

    createPatch();
}

Git::DiffList HistoryDiff::makePatchTo( const QString& ref )
{
    Git::Result r;
    Git::ObjectId oid = mRepo.lookupRef( ref, r ).resolveToObjectId( r );
    if( !r )
    {
        return Git::DiffList();
    }

    Git::ObjectCommit commit = mRepo.lookupCommit( oid, r );
    if( !r )
    {
        return Git::DiffList();
    }

    return mRepo.diffCommitToCommit( mCommit, commit, r );
}

void HistoryDiff::createPatch()
{
    if( !mCommit.isValid() )
    {
        return;
    }

    int index = mDiffTo->currentIndex();
    DiffToTypes dtt = DiffToTypes( mDiffTo->itemData( index ).toInt() );
    Git::Result r;
    Git::DiffList dl, dl2;
    Git::ObjectTree tree = mCommit.tree( r );

    switch( dtt )
    {
    case DTT_WT:
        dl = tree.diffToWorkingDir( r );
        break;

    case DTT_WT_and_Index:
        dl = tree.diffToWorkingDir( r );
        dl2 = tree.diffToIndex( r );
        dl2.mergeOnto( dl, r );
        break;

    case DTT_AllParents:
        dl = mCommit.diffFromAllParents( r );
        break;

    case DTT_Parent:
        index = mDiffToParent->currentIndex();
        if( index != -1 )
        {
            dl = mCommit.diffFromParent( (unsigned int) index, r );
        }
        break;

    case DTT_HEAD:
        dl = makePatchTo( QLatin1String( "HEAD" ) );
        break;

    case DTT_Branch:
        dl = makePatchTo( QLatin1String( "refs/heads/" ) % mDiffToBranch->currentText() );
        break;

    case DTT_Tag:
        dl = makePatchTo( QLatin1String( "refs/tags/" ) % mDiffToTag->currentText() );
        break;

    default:
        break;
    }

    if( dl.isValid() )
    {
        Git::Result r;
        GitPatchConsumer p;
        dl.consumePatch( &p, r );
        setPatch( p.patch() );
    }
    else
    {
        setPatch( Patch::Ptr() );
    }
}

void HistoryDiff::setPatch( Patch::Ptr patch )
{
    mRawView->setPatch( patch );
}
