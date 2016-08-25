/**
 *******************************************************************************
 * @file QConvert.hpp
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
#ifndef QCONVERT_HPP
  #define QCONVERT_HPP

  #include <QDialog>
  #include <QProcess>
  #include <QFile>
  #include <QTextEdit>
  #include <QEvent>
  #include <QObject>
  #include "QHelper.hpp"

  namespace Ui {
    class QConvert;
  }

  /**
   * @brief The QConvert UI class
   */
  class QConvert : public QDialog {
    Q_OBJECT

    public:
      explicit QConvert(QWidget *parent = 0);
      ~QConvert();

    private slots:
       /**
        * @brief Method called by the encoding process to read the output of the ffmpeg process.
        */
       void readEncodingStandardOutput();

       /**
        * @brief Method called by the encoding process when the encoding process is started.
        */
       void encodingStarted();

       /**
        * @brief Method called by the encoding process when the encoding process is finished.
        */
       void encodingFinished();

       /**
        * @brief Method called when the user click on the FFmpeg button.
        */
       void on_ffmpegButton_clicked();

       /**
        * @brief Method called when the user click on the 'input' button.
        */
       void on_inputButton_clicked();

       /**
        * @brief Method called when the user click on the 'convert' button.
        */
       void on_convertButton_clicked();

       /**
        * @brief Method called when the user click on the 'play Input' button.
        */
       void on_playInputButton_clicked();

       /**
        * @brief Method called when the user click on the 'play Output' button.
        */
       void on_playOutputButton_clicked();

       /**
        * @brief Method called when an event is reached by the ffmpegLineEdit component (Focus lost).
        * @param object The owner object.
        * @param event The event type.
        */
       bool eventFilter(QObject *object, QEvent *event);

       /**
        * @brief Callback called when the input process is stopped.
        */
       void inputFinished();

       /**
        * @brief Callback called when the output process is stopped.
        */
       void outputFinished();

    private:
      Ui::QConvert *ui;
      QProcess *m_encodingProcess;
      QProcess *m_inputPlayProcess;
      QProcess *m_outputPlayProcess;
      QString m_outputString;
      QString m_fileDialogFormats;
      QString m_resultFile;


      /**
       * @brief Append a message to the log area.
       * @param str The message to append.
       */
      void logAreaAppend(QString &str);

  };

#endif // QCONVERT_HPP
