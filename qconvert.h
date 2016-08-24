/**
 *******************************************************************************
 * @file qconvert.h
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
#ifndef QCONVERT_H
  #define QCONVERT_H

  #include <QDialog>
  #include <QProcess>
  #include <QFile>
  #include <QTextEdit>
  #include <QEvent>
  #include <QObject>

  namespace Ui {
    class QConvert;
  }

  class QConvert : public QDialog {
    Q_OBJECT

    public:
      explicit QConvert(QWidget *parent = 0);
      ~QConvert();

    private slots:
       void readEncodingStandardOutput();
       void encodingStarted();
       void encodingFinished();
       void on_ffmpegButton_clicked();
       void on_fromButton_clicked();
       void on_convertButton_clicked();
       void on_playInputButton_clicked();
       void on_playOutputButton_clicked();
       bool eventFilter(QObject *object, QEvent *event);

    private:
      Ui::QConvert *ui;
      QProcess *m_encodingProcess;
      QProcess *m_inputPlayProcess;
      QProcess *m_outputPlayProcess;
      QString m_outputString;

      bool isFFmpeg();
      void validateFFmpegPath(QString &dir);
      void logAreaAppend(QString &str);
  };

#endif // QCONVERT_H
