/**
 *******************************************************************************
 * @file main.cpp
 * @author Keidan
 * @date 24/08/2016
 * @par Project qconvert
 *
 * @par Copyright 2016 Keidan, all right reserved
 *
 *      This software is distributed in the hope that it will be useful, but
 *      WITHOUT ANY WARRANTY.
 *
 *      License summary : You can modify and redistribute the sources code and
 *      binaries. You can send me the bug-fix
 *
 *      Term of the license in in the file license.txt.
 *
 *******************************************************************************
 */
#include <QApplication>
#include "QConvert.hpp"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    QConvert w;
    w.setWindowTitle("FFmpeg converter");
    w.show();

    return a.exec();
}
