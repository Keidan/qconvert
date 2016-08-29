/**
 *******************************************************************************
 * @file QVideoOptions.hpp
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
#ifndef QVIDEOOPTIONS_HPP
  #define QVIDEOOPTIONS_HPP

  #include <QObject>
  #include <QString>
  #include <QStringList>
  #include "QVideoFormat.hpp"

  class QVideoOptions : public QObject {
    Q_OBJECT

    public:
      explicit QVideoOptions(QObject *parent = 0);

      /**
       * @brief Set the input file name.
       * @param input The path.
       */
      void setInput(const QString &input);

      /**
       * @brief Set the ouput format.
       * @param format The format.
       */
      void setFormat(QVideoFormat *format);

      /**
       * @brief Set the scaling options.
       * @param scaleThe scaling options.
       */
      void setScale(const QString &scale);


      /**
       * @brief Get the output file name.
       * @return QString;
       */
      QString output();

      /**
       * @brief Return the options as option list
       * @return QStringList
       */
      QStringList options();

    signals:
    public slots:
    private:
      QString m_input;
      QVideoFormat *m_format;
      QString m_scale;
  };
#endif // QVIDEOOPTIONS_HPP
