#include "serialwrite.h"
#include "ui_serialwrite.h"

SerialWrite::SerialWrite(QWidget *parent) : QMainWindow(parent),ui(new Ui::SerialWrite)
{
    ui->setupUi(this);

    connect(ui->input,SIGNAL(returnPressed()),this,SLOT(input_return_Pressed()));

    // We vragen alle aangesloten serial ports op voor in de lijst
    QList<QSerialPortInfo> poorten = QSerialPortInfo::availablePorts();
    for(int i=0; i< poorten.count(); i++)
    {
        ui->input_com->addItem(poorten.at(i).portName());
    }

    // We maken een serial object
    m_uart = new QSerialPort(this);

}

//
//  Destructor van het programma
//
SerialWrite::~SerialWrite()
{
    delete ui;
}

//
//  We sturen de data uit de input box
//
void SerialWrite::on_btn_send_clicked()
{
    QString commando = ui->input->text();

    // We sturen de data over de uart + de juiste regeleinde
    QTextStream uart(m_uart);
    uart << commando;

    switch(ui->input_einde->currentIndex())
    {
        case 1:
            uart << "\n";
            break;
        case 2:
            uart << "\n\r";
            break;
    }

    // We echoen de tekst ook lokaal
    if(ui->input_echo->checkState())
    {
        ui->output->appendPlainText("( " + commando + ui->input_einde->currentText() + " )");
    }

    ui->input->clearAndSave();
}

//
//  Open de verbinding met de serial port
//
void SerialWrite::on_btn_Open_clicked()
{
    // We pakken de gekozen poort + baudRate
    m_uart->setPortName(ui->input_com->currentText());
    m_uart->setBaudRate(ui->input_baud->currentText().toInt());
    if(m_uart->open(QIODevice::ReadWrite))
    {
        ui->btn_send->setEnabled(true);
        connect(m_uart,SIGNAL(readyRead()),this,SLOT(dataReadyToRead()));
    }
}

//
//  Slot als er op enter wordt gedrukt in de lineEdit
//
void SerialWrite::input_return_Pressed()
{
    ui->btn_send->animateClick();
}

//
//  De data in de uart is klaar om te lezen
//
void SerialWrite::dataReadyToRead(void)
{
    ui->output->appendPlainText(m_uart->readAll());
}

//
//  Het output veld clearen
//
void SerialWrite::on_btn_clear_clicked()
{
    ui->output->clear();
}
