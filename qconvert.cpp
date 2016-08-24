/**
 *******************************************************************************
 * @file qconvert.cpp
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
#include "qconvert.h"
#include "ui_qconvert.h"
#include <QDebug>
#include <QScrollBar>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>

#define MSGBOX_TITLE "FFmpeg"
#define FFPLAY_BIN "ffplay"
#define FFMPEG_BIN "ffmpeg"

QConvert::QConvert(QWidget *parent) :
    QDialog(parent), ui(new Ui::QConvert), m_encodingProcess(NULL),
    m_inputPlayProcess(NULL), m_outputPlayProcess(NULL), m_outputString("") {

  ui->setupUi(this);

  // focus out management
  ui->ffmpegLineEdit->installEventFilter(this);

  // Play button for output - initially disabled
  ui->playOutputButton->setEnabled(false);
  ui->playInputButton->setEnabled(false);
  ui->fromButton->setEnabled(false);
  ui->convertButton->setEnabled(false);

  // Create processes: 1.encoding, 2.input play 3.output play
  m_encodingProcess = new QProcess(this);
  m_inputPlayProcess = new QProcess(this);
  m_outputPlayProcess = new QProcess(this);

  connect(m_encodingProcess, SIGNAL(started()), this, SLOT(encodingStarted()));
  connect(m_encodingProcess,SIGNAL(readEncodingStandardOutput()), this, SLOT(readEncodingStandardOutput()));
  connect(m_encodingProcess, SIGNAL(finished(int)), this, SLOT(encodingFinished()));

#ifdef __linux__
  if(QFile::exists("/usr/bin/"FFMPEG_BIN) && QFile::exists("/usr/bin/"FFPLAY_BIN)) {
    ui->ffmpegLineEdit->setText("/usr/bin");
    ui->fromButton->setEnabled(true);
  }
#endif
}

QConvert::~QConvert() {
  delete ui;
}


void QConvert::encodingStarted() {
  qDebug() << "encodingStarted()";
}

bool QConvert::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::FocusOut) {
    if (object == ui->ffmpegLineEdit) {
      QString dir = ui->ffmpegLineEdit->text();
      validateFFmpegPath(dir);
    }
  }
  return false;
}

// conversion start
void QConvert::on_convertButton_clicked() {
  if(!isFFmpeg()) return;
  QString bin = ui->ffmpegLineEdit->text() + "/"FFMPEG_BIN;

  QStringList args;
  QString input = ui->fromLineEdit->text();
  if(input.isEmpty()) {
    qDebug() << "No input";
    QMessageBox::information(this, MSGBOX_TITLE, "Input file not specified");
    return;
  }
  QString output = ui->toLineEdit->text();
  if(output.isEmpty()) {
    qDebug() << "No output";
    QMessageBox::information(this, MSGBOX_TITLE, "Output file not specified");
    return;
  }

  QString fileName = ui->toLineEdit->text();
  qDebug() << "output file check " << fileName;
  qDebug() << "QFile::exists(fileName) = " << QFile::exists(fileName);
  if (QFile::exists(fileName)) {
    if (QMessageBox::No == QMessageBox::question(this, MSGBOX_TITLE,
        "There already exists a file called " + fileName + " in "
        "the current directory. Overwrite?",
        QMessageBox::Yes|QMessageBox::No, QMessageBox::No))
      return;
    QFile::remove(fileName);
    while(QFile::exists(fileName)) {
      qDebug() << "output file still there";
    }
  }

  args << "-i" << input << output;

  qDebug() << args;

  m_encodingProcess->setProcessChannelMode(QProcess::MergedChannels);
  m_encodingProcess->start(bin, args);
}

void QConvert::readEncodingStandardOutput() {
  QString str = m_encodingProcess->readAllStandardOutput();
  logAreaAppend(str);
}


void QConvert::logAreaAppend(QString &str) {
  m_outputString.append(str);
  ui->logArea->setText(m_outputString);
  // put the slider at the bottom
  ui->logArea->verticalScrollBar()->setSliderPosition(ui->logArea->verticalScrollBar()->maximum());
}

void QConvert::encodingFinished() {
  // Set the encoding status by checking output file's existence
  QString fileName = ui->toLineEdit->text();

  if (QFile::exists(fileName)) {
    ui->statusLabel->setText("Transcoding Status: Successful!");
    ui->playOutputButton->setEnabled(true);
  }
  else
    ui->statusLabel->setText("Transcoding Status: Failed!");
}

// Browse... button clicked - this is for ffmpeg binary
void QConvert::on_ffmpegButton_clicked() {
  QString dir = QFileDialog::getExistingDirectory(
    this, tr("FFMPEG directory"), QDir::homePath(),
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  validateFFmpegPath(dir);
}

// Browse... button clicked - this is for input file
void QConvert::on_fromButton_clicked() {
  QString fileName =
  QFileDialog::getOpenFileName(
    this, "Open File", QDir::homePath(),
    "videos (*.mp4 *.mov *.avi *.mkv)");
  if (!fileName.isEmpty()) {
    ui->fromLineEdit->setText(fileName);
    ui->playInputButton->setEnabled(true);
    ui->convertButton->setEnabled(true);
  }
}

void QConvert::on_playInputButton_clicked() {
  if(!isFFmpeg()) return;
  QString bin = ui->ffmpegLineEdit->text() + "/"FFPLAY_BIN;
  QStringList args;
  args << ui->fromLineEdit->text();
  m_inputPlayProcess->start(bin, args);
}

void QConvert::on_playOutputButton_clicked() {
  if(!isFFmpeg()) return;
  QString bin = ui->ffmpegLineEdit->text() + "/"FFPLAY_BIN;
  QStringList args;
  args << ui->toLineEdit->text();
  m_outputPlayProcess->start(bin, args);
}


bool QConvert::isFFmpeg() {
  QString str = ui->ffmpegLineEdit->text();
  if(str.isEmpty()) {
    QMessageBox::warning(this, MSGBOX_TITLE, "The FFMPEG binary folder was not found!");
    return false;
  }
  return true;
}

void QConvert::validateFFmpegPath(QString &dir) {
  if (!dir.isEmpty()) {
    QStringList strList;
    QString f1 = dir + "/"FFMPEG_BIN;
    QString f2 = dir + "/"FFPLAY_BIN;
    if(!QFile::exists(f1)) strList << FFMPEG_BIN;
    if(!QFile::exists(f2)) strList << FFPLAY_BIN;
    if(!strList.empty()) {
      QString err = "The following file";
      if(strList.size() == 1)
        err += " " + strList[0] + " was ";
      else
        err += "s " + strList.join(",") + " are ";
      err += " not found";
      QMessageBox::critical(this, MSGBOX_TITLE, err);
      ui->ffmpegLineEdit->setFocus(Qt::OtherFocusReason);
      return;
    }

    ui->ffmpegLineEdit->setText(dir);
    ui->fromButton->setEnabled(true);
  }
}
