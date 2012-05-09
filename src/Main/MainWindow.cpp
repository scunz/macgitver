
#include <QStatusBar>
#include <QFileDialog>
#include <QApplication>
#include <QMenuBar>
#include <QDockWidget>

#include "Core/ObjectId.h"
#include "Core/Reference.h"

#include "Main/MainWindow.h"
#include "Dlgs/Repository/CreateRepositoryDlg.h"

#include "Heaven/HTopLevelWidget.h"

#include "Views/Diff/DiffView.h"
#include "Views/Branches/BranchesView.h"
#include "Views/Tags/TagsView.h"
#include "Views/History/HistoryView.h"
#include "Views/Refs/RefsView.h"
#include "Views/WorkingTree/IndexWidget.h"

MainWindow* MainWindow::sSelf = NULL;

MainWindow::MainWindow()
	: mRepo()
{
	sSelf = this;
	setupUi();
}

MainWindow::~MainWindow()
{
	closeRepository();
}

void MainWindow::onRepositoryCreate()
{
	CreateRepositoryDlg().exec();
}

void MainWindow::onRepositoryOpen()
{
	QString fn = QFileDialog::getExistingDirectory( this, trUtf8( "Open repository" ) );
	if( fn.isEmpty() )
	{
		return;
	}

	Git::Repository repo = Git::Repository::open( fn );
	if( repo.isValid() )
	{
		switchToRepo( repo );
	}
}

void MainWindow::setupUi()
{
	QFile styleFile( ":/MacGitver.qss" );
	styleFile.open( QFile::ReadOnly );
	setStyleSheet( styleFile.readAll() );

	mmuRepository			= menuBar()->addMenu( trUtf8( "&Repository" ) );
	macRepositoryCreate		= mmuRepository->addAction( trUtf8( "&Open..." ),
														this, SLOT(onRepositoryOpen()) );
							  mmuRepository->addSeparator();
	macRepositoryCreate		= mmuRepository->addAction( trUtf8( "Create &new..." ),
														this, SLOT(onRepositoryCreate()) );
							  mmuRepository->addSeparator();
	macRepositoryQuit		= mmuRepository->addAction( trUtf8( "&Quit" ),
														qApp, SLOT(quit()) );

	mmuWorkingTree			= menuBar()->addMenu( trUtf8( "&Working Tree" ) );
	//mmuWorkingTree->addAction()

	setWindowTitle( trUtf8( "MacGitver" ) );

	statusBar()->addPermanentWidget( mLblCurrentBranch = new QLabel() );

	/*

	ReferenceView* rv = new ReferenceView;

	QDockWidget* dw = new QDockWidget( trUtf8( "Refs" ) );
	dw->setWidget( rv );
	addDockWidget( Qt::RightDockWidgetArea, dw );

	connect( this, SIGNAL(repositoryChanged(Git::Repository)),
		rv, SLOT(repositoryChanged(Git::Repository)) );

	IndexWidget* iw = new IndexWidget;

	QDockWidget* dwIndex = new QDockWidget( trUtf8( "Working Tree" ) );
	dwIndex->setWidget( iw );
	addDockWidget( Qt::RightDockWidgetArea, dwIndex );

	connect( this, SIGNAL(repositoryChanged(Git::Repository)),
		iw, SLOT(repositoryChanged(Git::Repository)) );

	*/

	mTop = new HeavenTopLevelWidget();
	setCentralWidget( mTop );

	RefsView* rv = new RefsView;
	mTop->addView( rv );
	connect( this, SIGNAL(repositoryChanged(Git::Repository)),
		rv, SLOT(repositoryChanged(Git::Repository)) );

	BranchesView* branches = new BranchesView;
	mTop->addView( branches );
	connect( this, SIGNAL(repositoryChanged(Git::Repository)),
		branches, SLOT(repositoryChanged(Git::Repository)) );

	TagsView* tags = new TagsView;
	mTop->addView( tags );
	connect( this, SIGNAL(repositoryChanged(Git::Repository)),
		tags, SLOT(repositoryChanged(Git::Repository)) );

	HistoryView* hv = new HistoryView;
	mTop->addView( hv, Heaven::Central );
	connect( this, SIGNAL(repositoryChanged(Git::Repository)),
		hv, SLOT(repositoryChanged(Git::Repository)) );

	IndexWidget* iw = new IndexWidget;
	mTop->addView( iw, Heaven::Central );
	connect( this, SIGNAL(repositoryChanged(Git::Repository)),
		iw, SLOT(repositoryChanged(Git::Repository)) );

	mTop->addView( new DiffView, Heaven::Central );
}

void MainWindow::closeRepository()
{
	if( !mRepo.isValid() )
	{
		return;
	}

	emit repositoryChanged( Git::Repository() );

	mRepo = Git::Repository();
}

void MainWindow::switchToRepo( const Git::Repository& repo )
{
	closeRepository();

	mRepo = repo;
	emit repositoryChanged( mRepo );

	if( mRepo.isValid() )
	{
		Git::Reference HEAD = mRepo.HEAD();
		QString curBranch;
		if( HEAD.name() != "HEAD" )
		{
			curBranch = trUtf8( "on branch <b>%1</b>" ).arg( HEAD.name().mid( 11 ).constData() );
		}
		else
		{
			curBranch = trUtf8( "HEAD detached: <b>%1</b>" ).arg( HEAD.objectId().toString() );
		}

		mLblCurrentBranch->setText( curBranch );
	}
}