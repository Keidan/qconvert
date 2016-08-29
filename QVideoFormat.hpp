/**
 *******************************************************************************
 * @file QVideoFormat.hpp
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
#ifndef QVIDEOFORMAT_HPP
  #define QVIDEOFORMAT_HPP

  #include <QObject>
  #include <QString>
  #include <QList>

  /**
   * @brief The video format representation.
   */
  class QVideoFormat : public QObject {
    Q_OBJECT

    public:
      explicit QVideoFormat(QObject *parent = 0);

      /**
       * @brief Get the format name.
       * @return QString
       */
      QString& getName();

      /**
       * @brief Get the format extension (with the dot).
       * @return QString
       */
      QString& getExtension();

      /**
       * @brief Format the nale and the extension (eg: name (ext))
       * @return QString
       */
      QString format();

      /**
       * @brief Build the default supported formats.
       * @return QList<QVideoFormat*>
       */
      static QList<QVideoFormat*> makeDefaults();

      /**
       * @brief Create a QVideoFormat instance.
       * @return QVideoFormat*
       */
      static QVideoFormat* create(const QString& name, const QString& extension);

    signals:

    public slots:

    private:

      QString m_name;
      QString m_extension;
  };

  Q_DECLARE_METATYPE(QVideoFormat*)

#endif // QVIDEOFORMAT_HPP
