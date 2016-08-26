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
#include "QConvert.hpp"
#include "ui_QConvert.h"
#include <QDebug>
#include <QScrollBar>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include "QVideoFormat.hpp"

#define PLAY_INPUT     "Play input"
#define PLAY_OUTPUT    "Play output"
#define STOP_INPUT     "Stop input"
#define STOP_OUTPUT    "Stop output"
#define START_CONVERT  "Convert"
#define STOP_CONVERT   "Stop"

QConvert::QConvert(QWidget *parent) :
    QDialog(parent), ui(new Ui::QConvert), m_encodingProcess(NULL),
    m_inputPlayProcess(NULL), m_outputPlayProcess(NULL), m_outputString(""),
    m_fileDialogFormats(""), m_resultFile("") {

  ui->setupUi(this);

  // focus out management
  ui->ffmpegLineEdit->installEventFilter(this);

  // Play button for output - initially disabled
  ui->playOutputButton->setEnabled(false);
  ui->playInputButton->setEnabled(false);
  ui->inputButton->setEnabled(false);
  ui->convertButton->setEnabled(false);

  // Create processes: 1.encoding, 2.input play 3.output play
  m_encodingProcess = new QProcess(this);
  m_inputPlayProcess = new QProcess(this);
  m_outputPlayProcess = new QProcess(this);

  connect(m_encodingProcess, SIGNAL(started()), this, SLOT(encodingStarted()));
  connect(m_encodingProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
  connect(m_encodingProcess, SIGNAL(finished(int)), this, SLOT(encodingFinished()));

  connect(m_inputPlayProcess, SIGNAL(finished(int)), this, SLOT(inputFinished()));
  connect(m_outputPlayProcess, SIGNAL(finished(int)), this, SLOT(outputFinished()));

#ifdef __linux__
  if(QFile::exists("/usr/bin/" FFMPEG_BIN) && QFile::exists("/usr/bin/" FFPLAY_BIN)) {
    ui->ffmpegLineEdit->setText("/usr/bin");
    ui->inputButton->setEnabled(true);
  }
#endif

  /* intialize the cb content */
  QList<QVideoFormat*> formats = QVideoFormat::makeDefaults();
  /* first time iterate to list all media files */
  int i = 0;
  m_fileDialogFormats = "All video files (";
  for(; i < formats.size(); ++i) {
    m_fileDialogFormats += "*" + formats.at(i)->getExtension();
    if(i < formats.size() - 1) m_fileDialogFormats += " ";
  }
  m_fileDialogFormats += ");;";
  for(i = 0; i < formats.size(); ++i) {
    QVideoFormat* qvf = formats.at(i);
    ui->formatCombobox->addItem(qvf->format(), QVariant::fromValue(qvf->getExtension()));
    m_fileDialogFormats += qvf->format();
    if(i < formats.size() - 1) m_fileDialogFormats += ";;";
    delete qvf;
  }
  formats.clear();
}

QConvert::~QConvert() {
  delete ui;
}

/**
 * @brief Method called by the encoding process when the encoding process is started.
 */
void QConvert::encodingStarted() {
  logAreaAppend("Encoding started\n");
}

/**
 * @brief Method called when an event is reached by the ffmpegLineEdit component (Focus lost).
 * @param object The owner object.
 * @param event The event type.
 */
bool QConvert::eventFilter(QObject *object, QEvent *event) {
  if (event->type() == QEvent::FocusOut) {
    if (object == ui->ffmpegLineEdit) {
      QString dir = ui->ffmpegLineEdit->text();
      QHelper::validateFFmpegPath(this, dir, ui->ffmpegLineEdit, ui->inputButton);
    }
  }
  return false;
}

/**
 * @brief Method called when the user click on the 'convert' button.
 */
void QConvert::on_convertButton_clicked() {

  ui->playOutputButton->setEnabled(false);
  if(ui->convertButton->text() == STOP_CONVERT) {
    m_encodingProcess->terminate();
    return;
  }
  QString bin = ui->ffmpegLineEdit->text();
  if(!QHelper::isFFmpeg(this, bin)) return;
  bin += "/" FFMPEG_BIN;

  QStringList args;
  QString input = ui->inputLineEdit->text();
  if(input.isEmpty()) {
    logAreaAppend("No input\n");
    QMessageBox::information(this, MSGBOX_TITLE, "Input file not specified");
    return;
  }
  QString ext = ui->formatCombobox->itemData(ui->formatCombobox->currentIndex()).toString();
  m_resultFile = input + ext;
  if(m_resultFile.isEmpty()) {
    logAreaAppend("No output\n");
    QMessageBox::information(this, MSGBOX_TITLE, "Output file not specified");
    return;
  }

  logAreaAppend("Output file check " + m_resultFile + "\n");
  if (QFile::exists(m_resultFile)) {
    if (QMessageBox::No == QMessageBox::question(this, MSGBOX_TITLE,
        "There already exists a file called " + m_resultFile + " in "
        "the current directory. Overwrite?",
        QMessageBox::Yes|QMessageBox::No, QMessageBox::No)) {
      m_resultFile = "";
      return;
    }
    QFile::remove(m_resultFile);
    while(QFile::exists(m_resultFile)) {
      qDebug() << "output file still there";
    }
  }

  args << "-i" << input;
  if(ext == ".mov")
    args << "-strict" << "-2";
  args << m_resultFile;


  logAreaAppend(bin + " -> " + args.join(" ") + "\n");
  ui->convertButton->setText(STOP_CONVERT);
  m_encodingProcess->setProcessChannelMode(QProcess::MergedChannels);
  m_encodingProcess->start(bin, args);
}

/**
 * @brief Method called by the encoding process to read the stdout of the ffmpeg process.
 */
void QConvert::readyReadStandardOutput() {
  logAreaAppend(m_encodingProcess->readAllStandardOutput());
}


/**
 * @brief Append a message to the log area.
 * @param str The message to append.
 */
void QConvert::logAreaAppend(const QString &str) {
  m_outputString.append(str);
  ui->logArea->setText(m_outputString);
  // put the slider at the bottom
  ui->logArea->verticalScrollBar()->setSliderPosition(ui->logArea->verticalScrollBar()->maximum());
}

/**
 * @brief Method called by the encoding process when the encoding process is finished.
 */
void QConvert::encodingFinished() {
  // Set the encoding status by checking output file's existence
  if (QFile::exists(m_resultFile)) {
    ui->statusLabel->setText("Transcoding Status: Successful!");
    ui->playOutputButton->setEnabled(true);
  }
  else
    ui->statusLabel->setText("Transcoding Status: Failed!");
  ui->convertButton->setText(START_CONVERT);
  logAreaAppend("Encoding finished\n");
}

/**
 * @brief Method called when the user click on the FFmpeg button.
 */
void QConvert::on_ffmpegButton_clicked() {
  QString home = QDir::homePath();
  if(!ui->ffmpegLineEdit->text().isEmpty())
      home = ui->ffmpegLineEdit->text();
  QString dir = QFileDialog::getExistingDirectory(
    this, tr("FFMPEG directory"), home,
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  QHelper::validateFFmpegPath(this, dir, ui->ffmpegLineEdit, ui->inputButton);
}

/**
 * @brief Method called when the user click on the 'input' button.
 */
void QConvert::on_inputButton_clicked() {
  QString fileName = QFileDialog::getOpenFileName(
    this, "Open File", QDir::homePath(),
    m_fileDialogFormats);
  if (!fileName.isEmpty()) {
    ui->inputLineEdit->setText(fileName);
    ui->playInputButton->setEnabled(true);
    ui->convertButton->setEnabled(true);
  }
}

/**
 * @brief Method called when the user click on the 'play Input' button.
 */
void QConvert::on_playInputButton_clicked() {
  QString bin = ui->ffmpegLineEdit->text();
  QString path = ui->inputLineEdit->text();
  if(ui->playInputButton->text() == STOP_INPUT)
    m_inputPlayProcess->terminate();
  else {
    if(QHelper::startPlayer(this, m_inputPlayProcess, bin, path))
      ui->playInputButton->setText(STOP_INPUT);
  }
}

/**
 * @brief Method called when the user click on the 'play Output' button.
 */
void QConvert::on_playOutputButton_clicked() {
  QString bin = ui->ffmpegLineEdit->text();
  if(ui->playOutputButton->text() == STOP_OUTPUT)
    m_outputPlayProcess->terminate();
  else {
    if(QHelper::startPlayer(this, m_outputPlayProcess, bin, m_resultFile))
      ui->playOutputButton->setText(STOP_OUTPUT);
  }
}

/**
 * @brief Callback called when the input process is stopped.
 */
void QConvert::inputFinished() {
  ui->playInputButton->setText(PLAY_INPUT);
}

/**
 * @brief Callback called when the output process is stopped.
 */
void QConvert::outputFinished() {
  ui->playOutputButton->setText(PLAY_OUTPUT);
}

