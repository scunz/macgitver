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

#ifndef DIFF_VIEW_H
#define DIFF_VIEW_H

#include <QWidget>

class QTreeWidget;
class QStackedWidget;
class QTextBrowser;
class QScrollBar;

#include "libHeaven/Views/View.h"

#include "Diff/View/DiffViewApi.h"

#include "Diff/Model/Patch.h"

class DiffWidget;
class DiffTreeFileList;
class DiffFrame;

class DIFF_VIEW_API DiffView : public Heaven::View
{
    Q_OBJECT
public:
    DiffView();
    ~DiffView();

public:
    void setPatch( Patch::Ptr patch );

private:
    void clearTree();
    void fillTree();

private:
    Patch::Ptr          mPatch;
    DiffTreeFileList*   mTree;
    QTreeWidget*        mDetails;
    QStackedWidget*     mDiffStack;
    QTextBrowser*       mRawDiffView;
    DiffFrame*          mDiffFrame;
    QScrollBar*         mScrollV;
    QScrollBar*         mScrollH;
};

#endif
