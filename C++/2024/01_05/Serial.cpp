#include <iostream>
#include <fcntl.h> 
#include <termios.h> 
#include <unistd.h> 
#include <string.h>
#include <stdio.h>


using namespace std;

class UartCommunication {
public:
    UartCommunication(const char* devicePath, speed_t baudRate);
    ~UartCommunication();

    bool openConnection();
    void closeConnection();
    bool send();
    bool receive();

private:
    const char* devicePath_;
    speed_t baudRate_;
    int uartFileDescriptor_;

    char* send_buff[256];
    char* receive_buff[256];

    bool setAttributes();
};




UartCommunication::UartCommunication(const char* devicePath, speed_t baudRate)
    : devicePath_( devicePath), baudRate_(baudRate), uartFileDescriptor_(-1) {}

UartCommunication::~UartCommunication() {
    closeConnection();
}

bool UartCommunication::openConnection() {
    uartFileDescriptor_ = open(devicePath_, O_RDWR | O_NOCTTY | O_NDELAY);

    if (uartFileDescriptor_ == -1) {
        cerr << "Error opening UART device: " << devicePath_ << endl;
        return false;
    }

    if (!setAttributes()) {
        cerr << "Error setting UART attributes." << endl;
        return false;
    }

    return true;
}

void UartCommunication::closeConnection() {
    if (uartFileDescriptor_ != -1) {
        close(uartFileDescriptor_);
        uartFileDescriptor_ = -1;
    }
}

bool UartCommunication::send() {
    if (uartFileDescriptor_ == -1) {
        cerr << "UART connection not open." << endl;
        return false;
    }

    ssize_t bytesSent = write(uartFileDescriptor_, send_buff, sizeof(send_buff));

    if (bytesSent == -1) {
        cerr << "Error sending data." << endl;
        return false;
    }

    return true;
}

bool UartCommunication::receive() {
    if (uartFileDescriptor_ == -1) {
        cerr << "UART connection not open." << endl;
        return false;
    }

    ssize_t bytesRead = read(uartFileDescriptor_, receive_buff, sizeof(receive_buff));

    if (bytesRead == -1) {
        cerr << "Error receiving data." << endl;
        return false;
    }
    receive_buff[bytesRead]="\0";
    printf("%s\n",receive_buff);
    memset(receive_buff, '\0', sizeof(receive_buff));
    
    return true;
}

bool UartCommunication::setAttributes() {
    struct termios tty;
    if (tcgetattr(uartFileDescriptor_, &tty) != 0) {
        cerr << "Error getting UART attributes." << endl;
        return false;
    }

    cfsetospeed(&tty, baudRate_);
    cfsetispeed(&tty, baudRate_);

	tty.c_cflag |= (CLOCAL | CREAD);  // 로컬 연결 및 수신 활성화
    tty.c_cflag &= ~PARENB;  // 패리티 비활성화
    tty.c_cflag &= ~CSTOPB;  // 1 stop bit
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;  // 8 데이터 비트
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // raw mode
	tty.c_cc[VTIME] = 10;
	tty.c_cc[VMIN] = 0;

    if (tcsetattr(uartFileDescriptor_, TCSANOW, &tty) != 0) {
        cerr << "Error setting UART attributes." << endl;
        return false;
    }

    return true;
}

int main() {
    const char* uartDevice = "/dev/ttyACM0";
    const char* uartDevice1 = "/dev/ttyACM1";
    speed_t baudRate = B9600;


    UartCommunication uart(uartDevice,baudRate);
    UartCommunication uart1(uartDevice1,baudRate);
	int _time=0;
    while (1) {


          cout<<"\n\t"<<_time<<endl;
        if (uart.openConnection()) {
            uart.receive();
            uart.closeConnection();
        }
        if (uart1.openConnection()) {
            uart1.receive();
            uart1.closeConnection();
        }
        _time++;
        sleep(1);      
    }

    return 0;
}
