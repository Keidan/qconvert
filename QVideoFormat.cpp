/**
 *******************************************************************************
 * @file QVideoFormat.cpp
 * @author Keidan
 * @date 25/08/2016
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
#include "QVideoFormat.hpp"

QVideoFormat::QVideoFormat(QObject *parent) :
    QObject(parent), m_name(""), m_extension("") {
}

/**
 * @brief Get the format name.
 * @return QString
 */
QString& QVideoFormat::getName() {
    return m_name;
}

/**
 * @brief Get the format extension (with the dot).
 * @return QString
 */
QString& QVideoFormat::getExtension() {
    return m_extension;
}

/**
 * @brief Format the nale and the extension (eg: name (*.ext))
 * @return QString
 */
QString QVideoFormat::format() {
    QString s = m_name + "(*" + m_extension + ")";
    return s;
}

/**
 * @brief Create a QVideoFormat instance.
 * @return QVideoFormat*
 */
QVideoFormat* QVideoFormat::create(const QString& name, const QString& extension) {
  QVideoFormat* qvf = new QVideoFormat();
  qvf->m_extension = extension;
  qvf->m_name = name;
  return qvf;
}

/**
 * @brief Build the default supported formats.
 * @return QList<QVideoFormat>
 */
QList<QVideoFormat*> QVideoFormat::makeDefaults() {
  QList<QVideoFormat*> list;
  list << create("3GPP", ".3gp");
  list << create("3GPP2", ".3gp2");
  list << create("Audio Video Interleave", ".avi");
  list << create("Flash Video", ".flv");
  list << create("Matroska", ".mkv");
  list << create("MPEG-4", ".mp4");
  list << create("Open video format", ".ogv");
  list << create("QuickTime", ".mov");
  list << create("ShockWave Flash", ".swf");
  return list;
}
