/**
 *******************************************************************************
 * @file QVideoOptions.cpp
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
#include "QVideoOptions.hpp"

QVideoOptions::QVideoOptions(QObject *parent) :
    QObject(parent), m_input(""), m_format(NULL), m_scale("") {
}


/**
 * @brief Set the input file name.
 * @param input The path.
 */
void QVideoOptions::setInput(const QString &input) {
  m_input = input;
}

/**
 * @brief Set the ouput format.
 * @param format The format.
 */
void QVideoOptions::setFormat(QVideoFormat *format) {
  m_format = format;
}

/**
 * @brief Set the scaling options.
 * @param scaleThe scaling options.
 */
void QVideoOptions::setScale(const QString &scale) {
  m_scale = scale;
}


/**
 * @brief Get the output file name.
 * @return QString;
 */
QString QVideoOptions::output() {
  QString output;
  if(m_format) {
    output = m_input + m_format->getExtension();
  } else
    output = m_input + ".format_error";
  return output;
}

/**
 * @brief Return the options as option list
 * @return QStringList
 */
QStringList QVideoOptions::options() {
  QStringList li;
  li << "-i" << m_input;
  if(!m_scale.isEmpty())
    li << "-vf" << "scale=" + m_scale;

  if(m_format && m_format->getExtension() == ".mov")
      li << "-strict" << "-2";
  li << output();
  return li;
}
