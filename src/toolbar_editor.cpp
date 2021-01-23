/**

@author Mattia Basaglia

@section License

    Copyright (C) 2012-2020 Mattia Basaglia

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "toolbar_editor.hpp"
#include "ui_toolbar_editor.h"
#include <QMenu>
#include <QToolBar>

Q_DECLARE_METATYPE(QMenu*)
Q_DECLARE_METATYPE(QToolBar*)
Q_DECLARE_METATYPE(QAction*)

class Toolbar_Editor::Private : public Ui::Toolbar_Editor
{
public:

    QMainWindow* target = nullptr;
    QMap<QString,QList<QAction*> > toolbar_items;
    int custom_counter = 0;
};

Toolbar_Editor::Toolbar_Editor(QWidget *parent) :
    QWidget(parent), d(new Private)
{
    d->setupUi(this);

    foreach(QToolButton* b, findChildren<QToolButton*>())
        b->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
}

Toolbar_Editor::~Toolbar_Editor()
{
}

void Toolbar_Editor::setTargetWindow(QMainWindow *w)
{
    d->target = w;
    updateBars();
}


QMainWindow* Toolbar_Editor::targetWindow() const
{
    return d->target;
}

QSize Toolbar_Editor::sizeHint() const
{
    return QSize(462,220);
}

void Toolbar_Editor::apply() const
{
    if ( !d->target )
        return;


    QList<QToolBar*> new_toolbars;

    for(QMap<QString,QList<QAction*> >::const_iterator i = d->toolbar_items.begin(),
            e = d->toolbar_items.end(); i != e; ++i )
    {
        QToolBar *newtb = d->target->findChild<QToolBar*>(i.key());

        if ( !newtb )
        {
            newtb = new QToolBar(i.key(), d->target);
            newtb->setObjectName(i.key());
            new_toolbars.push_back(newtb);
        }

        newtb->clear();
        foreach(QAction* act, i.value() )
        {
            newtb->insertAction(0,act);
        }

    }


    foreach(QToolBar* toolbar, d->target->findChildren<QToolBar*>())
    {
        if ( !d->toolbar_items.contains(toolbar->objectName()) )
        {
            d->target->removeToolBar(toolbar);
        }
    }

    foreach(QToolBar* toolbar, new_toolbars )
    {
        d->target->addToolBar(Qt::TopToolBarArea,toolbar);
        toolbar->show();
    }
}


void Toolbar_Editor::updateBars()
{
    if ( !d->target )
        return;

    d->combo_menu->clear();
    d->list_menu->clear();
    d->combo_toolbar->clear();
    d->list_toolbar->clear();
    d->toolbar_items.clear();

    foreach(QMenu* menu, d->target->findChildren<QMenu*>())
    {
        d->combo_menu->addItem(menu->title().replace('&',""),QVariant::fromValue(menu));
    }


    foreach(QToolBar* toolbar, d->target->findChildren<QToolBar*>())
    {
        d->toolbar_items[toolbar->objectName()] = toolbar->actions();
        d->combo_toolbar->addItem(toolbar->objectName()/*,QVariant::fromValue(toolbar)*/);
    }
}


void Toolbar_Editor::on_combo_menu_currentIndexChanged(int index)
{
    d->list_menu->clear();
    QMenu* menu = d->combo_menu->itemData(index).value<QMenu*>();
    if ( !menu )
        return;

    foreach(QAction* act, menu->actions())
    {
        QListWidgetItem *item;
        if ( !act->isSeparator() )
            item = new QListWidgetItem(act->icon(),act->iconText());
        else
        {
            item = new QListWidgetItem(tr("--(separator)--"));
            item->setTextAlignment(Qt::AlignHCenter);
        }

        item->setData(Qt::UserRole,QVariant::fromValue(act));
        d->list_menu->addItem(item);
    }
}

void Toolbar_Editor::update_list_toolbar(QString name)
{
    d->list_toolbar->clear();

    foreach(QAction* act, d->toolbar_items[name] )
    {
        QListWidgetItem *item;
        if ( !act->isSeparator() )
            item = new QListWidgetItem(act->icon(),act->iconText());
        else
        {
            item = new QListWidgetItem(tr("--(separator)--"));
            item->setTextAlignment(Qt::AlignHCenter);
        }

        item->setData(Qt::UserRole,QVariant::fromValue(act));
        d->list_toolbar->addItem(item);
    }

}

void Toolbar_Editor::on_button_up_clicked()
{
    int curr_index = d->list_toolbar->currentRow();

    QList<QAction*>& list = d->toolbar_items[d->combo_toolbar->currentText()];
    if ( curr_index < 1 || curr_index >= list.size() )
        return;

    qSwap(list[curr_index], list[curr_index-1]);

    update_list_toolbar(d->combo_toolbar->currentText());
    d->list_toolbar->setCurrentRow(curr_index-1);
}


void Toolbar_Editor::on_button_down_clicked()
{

    int curr_index = d->list_toolbar->currentRow();

    QList<QAction*>& list = d->toolbar_items[d->combo_toolbar->currentText()];
    if ( curr_index < 0 || curr_index >= list.size()-1 )
        return;

    qSwap(list[curr_index], list[curr_index+1]);

    update_list_toolbar(d->combo_toolbar->currentText());
    d->list_toolbar->setCurrentRow(curr_index+1);



}

void Toolbar_Editor::on_button_insert_clicked()
{
    insert_action( d->list_menu->currentItem()->data(Qt::UserRole).value<QAction*>() );
}

void Toolbar_Editor::on_button_remove_clicked()
{
    int to_rm = d->list_toolbar->currentRow();


    QList<QAction*>& list = d->toolbar_items[d->combo_toolbar->currentText()];
    if ( to_rm >= 0 && to_rm < list.size() )
    {
        list.removeAt(to_rm);
        update_list_toolbar(d->combo_toolbar->currentText());
        d->list_toolbar->setCurrentRow(to_rm-1);
    }

}

void Toolbar_Editor::on_button_insert_separator_clicked()
{
    QAction* act = new QAction(NULL);
    act->setSeparator(true);
    insert_action(act);
}

void Toolbar_Editor::insert_action(QAction *new_action)
{
    int act_before = d->list_toolbar->currentRow();

    if ( new_action )
    {
        QList<QAction*>& list = d->toolbar_items[d->combo_toolbar->currentText()];
        if ( act_before >= 0 && act_before < list.size() )
        {
            list.insert(act_before+1,new_action);
            update_list_toolbar(d->combo_toolbar->currentText());
            d->list_toolbar->setCurrentRow(act_before+1);
        }
        else
        {
            list.push_back(new_action);
            update_list_toolbar(d->combo_toolbar->currentText());
            d->list_toolbar->setCurrentRow(list.size()-1);
        }

    }
}

void Toolbar_Editor::on_button_remove_toolbar_clicked()
{
    d->toolbar_items.remove(d->combo_toolbar->currentText());
    d->combo_toolbar->removeItem(d->combo_toolbar->currentIndex());
}

void Toolbar_Editor::on_button_add_toolbar_clicked()
{
    QString name = QString("custom_toolbar_%1").arg(d->custom_counter++);
    d->toolbar_items.insert(name,QList<QAction*>());

    d->combo_toolbar->addItem(name);
    d->combo_toolbar->setCurrentIndex(d->combo_toolbar->count()-1);
}


void Toolbar_Editor::setButtonStyle(Qt::ToolButtonStyle style)
{
    foreach(QToolButton* b, findChildren<QToolButton*>())
        b->setToolButtonStyle(style);
}

Qt::ToolButtonStyle Toolbar_Editor::buttonStyle() const
{
    return d->button_insert->toolButtonStyle();
}
