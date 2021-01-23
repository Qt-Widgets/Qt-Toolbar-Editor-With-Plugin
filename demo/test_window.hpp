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
#ifndef TEST_WINDOW_HPP
#define TEST_WINDOW_HPP

#include "ui_test_window.h"

class Test_Window : public QMainWindow, private Ui::Test_Window
{
    Q_OBJECT
    
public:
    explicit Test_Window(QWidget *parent = 0);
};

#endif // TEST_WINDOW_HPP
