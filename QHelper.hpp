/**
 *******************************************************************************
 * @file QHelper.hpp
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
#ifndef QHELPER_H
  #define QHELPER_H


  #include <QString>
  #include <QMessageBox>
  #include <QPushButton>
  #include <QLineEdit>

  #define MSGBOX_TITLE "FFmpeg"
  #define FFPLAY_BIN "ffplay"
  #define FFMPEG_BIN "ffmpeg"

  class QHelper {
    public:
      QHelper() { }
      ~QHelper() { }

      /**
       * @brief Test if the FFmpeg entry is valid (non empty), and display a warning message else.
       * @param parent The parent widget to display the message box.
       * @param path The path to test.
       * @return true if valid.
       */
      static bool isFFmpeg(QWidget* parent, const QString& path) {
        if(path.isEmpty()) {
          QMessageBox::warning(parent, MSGBOX_TITLE, "The FFMPEG binary folder was not found!");
          return false;
        }
        return true;
      }

      /**
       * @brief Validate the FFmpeg path.
       * @param parent The parent widget to display the message box.
       * @param dir The directory to test.
       * @param lineEdit The output component.
       * @param button The button to enabled.
       */
      static void validateFFmpegPath(QWidget* parent, const QString& dir, QLineEdit* lineEdit, QPushButton* button) {
        if (!dir.isEmpty()) {
          QStringList strList;
          QString f1 = dir + "/" FFMPEG_BIN;
          QString f2 = dir + "/" FFPLAY_BIN;
          if(!QFile::exists(f1)) strList << FFMPEG_BIN;
          if(!QFile::exists(f2)) strList << FFPLAY_BIN;
          if(!strList.empty()) {
            QString err = "The following file";
            if(strList.size() == 1)
              err += " " + strList[0] + " was ";
            else
              err += "s " + strList.join(",") + " are ";
            err += " not found";
            QMessageBox::critical(parent, MSGBOX_TITLE, err);
            lineEdit->setFocus(Qt::OtherFocusReason);
            return;
          }

          lineEdit->setText(dir);
          button->setEnabled(true);
        }
      }


      /**
       * @brief Start the media player.
       * @param parent The parent widget to display the message box.
       * @param process The process to start
       * @param path The file to play.
       * @return true is the process is started, else false.
       */
      static bool startPlayer(QWidget* parent, QProcess *process, QString& bin, const QString& path) {
        if(!QHelper::isFFmpeg(parent, bin)) return false;
        bin += "/" FFPLAY_BIN;
        QStringList args;
        args << path;
        process->start(bin, args);
        return true;
      }
  };

#endif // QHELPER_H
