### 21/05/06 (목)
---------------------------
# 1. UART 통신 과정

0. UART란?

    - UART ( 범용 비동기화 송수신기: Universal Asynchronous Receiver and Transmitter )

        > 범용이 의미하는 바 :  자료 형태나 전송속도를 직접 구성할 수 있다. 
        >
        > 비동기 : 데이터가 Clock 신호에 동기되지 않고 전송되는 것. 

    - 시리얼 통신으로 (병렬 데이터의 형태를 직렬 방식으로 전환하는) **데이터를 전송하는 컴퓨터 하드웨어 통신 규약**

        > 직렬 통신 : 하나의 line으로 데이터를 모두 보낸다.

    - 메모리나 레지스터에 있는 데이터를 차례로 읽어 직렬화하여 통신한다. 최대 8비트가 기본 단위이다.

    - 비동기식 통신이라 데이터를 수신하고 수신받은 신호를 해석하는 게 중요하다.

        데이터 해석을 위해서 필요한 정보는 다음과 같다.

        1. Baud Rate(보레이트) : 초당 얼마나 많은 심볼(의미있는 데이터)을 전송할 지에 대한 정보

        2. 데이터의 길이 : ~ 8 bit
        
        3. 패리티비트 : 오류 검출을 위한 것.

        4. start bit(시작비트), stop bit(정지비트) : 비동기 데이터 전송이므로 언제 데이터가 시작되는지 알기위해 시작비트가 있어야하고 언제 데이터 전송이 끝나는지 알기위해 정지비트가 있어야 한다. 

1. UART 연결

    - UART는 4가지의 핀으로 구성되어 있다.

        1. TX : 데이터 **송신** 핀

        2. RX : 데이터 **수신** 핀

        3. VCC and GND

    
    - UART에서 사용하는 시리얼 통신은 구현이 매우 간단하다.

    - 우리는 USB-to-Serial 장비로 통신한다.

2. 통신 과정

    우선 레지스터에서 데이터의 길이, 패리티비트, 시작비트, 정지비트를 세팅해준다. 그리고 USART의 SR(상태 레지스터)을 검사하여 현재 송/수신이 가능한지 확인한다. DR(데이터 레지스터)에 데이터가 있어 송신이 가능한 상태이면(TX) 시작비트 - 데이터 - 정지비트를 전송하게 되며 받는쪽은 수신 가능한 상태여야 한다. 수신쪽은 이전에 설정한 시작비트를 생각하여 시작비트가 들어오게 되면 그 다음 데이터부터 읽으며 정지비트가 들어오면 수신을 멈춘다. 이때 송/수신하는 속도는 Boud rate가 결정하는데 이는 초당 몇 비트를 전송하는지에 대한 단위이다.




---------------------------

# 2. 구현 Mission

- 시리얼 터미널로 LED 제어하는 셸 만들기
- 동작하는 시리얼 터미널 창 (gtkterm) example 사진

- 위 사진처럼 명령어를 반복적으로 수신하여 보드의 LED를 제어할 수 있는 UART 통신 기반 쉘 프로그램
- Ubuntu의 bash 쉘처럼 현재 사용자가 입력하는 문자가 터미널에 출력되어야 함
- 한 번 명령어를 입력하고 종료되는 것이 아니라, Ubuntu의 bash 쉘처럼 연속해서 명령어를 입력받을 수 있도록 구현
- 명령어 + 인자를 결합하여 수신하도록 구현

## 0. 결과

![command](./image/command.PNG)

- 기능

    1. help

    2. LED 제어 : 개별 On/Off, 전부 On/Off, 깜빡이기

    3. 잘못 입력 시 안내 문구 출력

## 1. MCU UART Setting

    ```c
    void set_usart2() {  // uart has 3 models
        // use UART PA2, PA3 pin 
        RCC_AHB1ENR |= 1<<0;
        GPIOA_MODER |= (1<<5) | (1<<7);
        GPIOA_AFRL |= (7<<8) | (7<<12);


        // set USART2
        RCC_APB1ENR |= (1<<17);     //  USART2 clock enable
        USART2_CR1 |= (0<<12);
        USART2_CR2 |= (0<<13) | (0<<12);

        USART2_BRR = (unsigned int)(42000000/115200);   // set Bour rate

        USART2_CR1 |= (1<<3) | (1<<2);
        USART2_CR1 |= (1<<5);
        USART2_CR1 |= (1<<13);


        // USART Interrupt enable
        NVIC_ISER1 |= 1<<6;

    }
    ```

- RCC_AHB1ENR : GPIO포트를 활성화하기 위해 해당 레지스터를 만져줘야한다. 0번 레지스터가 GPIO A를 담당하고 있고 UART는 포트 A에서 사용하므로 포트 A를 활성화해준다. 

- GPIOA_MODER : A포트의 2번핀 3번핀을 Alternate function mode로 활성화한다.

    > Alternate function mode? 연결된 주변장치(여기서는 UART)를 선택하는 모드이다.
    
- GPIOA_AFRL : A포트의 2번핀 3번핀의 주변기기에서 입출력을 하게 해준다.

- RCC_APB1ENR : USART2 클락을 사용하기위해 활성화하는 것이다.

- USART2_CR1

    - Bit 2 : RE(Receive Enable)을 설정하는 비트로 0으로 설정되면 받을 수 없고 1로 설정되면 데이터를 받는 모드로 활성화되어 start bit를 찾는다.

    - Bit 3 : TE(Transmitter Enable)을 설정하는 비트로 0으로 설정되면 보낼 수 없고, 1로 설정되면 보낼 수 있다.

    - Bit 5 : RXNE Interrupt enable 비트로 0이면 인터럽트를 무시하고 1이면 인터럽트를 받는다.

    - Bit 12 : 통신하는 데이터의 길이를 정하는데 0으로 설정하면 start bit = 1, 데이터 길이 = 8, stop bit = n으로 설정

    - Bit 13 : UE(USART Enable)를 설정하는 비트로 0이면 사용안하고 1이면 사용한다.

- USART2_CR2

    - Bit 12, 13 : stop bit로 00이면 1 stop bit를 의미한다.

- NVIC_ISER1 : Global Interrupt를 받게 한다.

- USART2_SR

    - Bit 5 : 0이면 읽을 데이터가 없고 1이면 읽을 데이터가 있다. 이 비트는 하드웨어에 의해서만 쓰여질 수 있는데, USART2_DR, 즉 데이터 레지스터에 값이 있다면 1로 쓰여진다.

    - Bit 6 : 0이면 인터럽트를 받지 않고 1이면 인터럽트를 받는 상태로 한다. 

    - Bit 7 : 0이면 데이터를 받을 게 없고 1이면 데이터를 받는 것이다. 이는 하드웨어에서만 제어가 된다.

## 2. USART2_Handler

```c
void USART2_IRQHandler() {
    if( USART2_SR & (1<<5) ) {
        rec = USART2_DR;        // read
        USART2_DR = rec;        // write

        if(rec != 13){          // 13 == Enter
            buffer[count++] = rec;
        }
        else{
            int i;
            USART2_DR = 13;
            while( !(USART2_SR & (1<<7) ));
            while( !(USART2_SR & (1<<6) ));


            // store command 
            char command[3][BUF_SIZE];
            char *ptr = strtok(buffer, " ");
            int j = 0;
            while( ptr != NULL ){
                strncpy(command[j], "", BUF_SIZE);
                strncpy(command[j], ptr, strlen(ptr));
                ptr = strtok(NULL, " ");
                j++;
            }

            // Functions
            if( strcmp(command[0], "help") == 0){           // help
                print_help();
            }
            else if( strcmp(command[0], "LEDON") == 0){     // LED ON
                if( strcmp(command[1], "1") == 0 ){
                    GPIOD_ODR |= 1 << 12;
                }
                else if( strcmp(command[1], "2") == 0 ){
                    GPIOD_ODR |= 1 << 13;
                }
                else if( strcmp(command[1], "3") == 0 ){
                    GPIOD_ODR |= 1 << 14;
                }
                else if( strcmp(command[1], "4") == 0 ){
                    GPIOD_ODR |= 1 << 15;
                }
                else{
                    print_error();
                }
            }
            else if( strcmp(command[0], "LEDOFF") == 0){    // LED OFF
                if( strcmp(command[1], "1") == 0 ){
                    GPIOD_ODR &= ~(1 << 12);
                }
                else if( strcmp(command[1], "2") == 0 ){
                    GPIOD_ODR &= ~(1 << 13);
                }
                else if( strcmp(command[1], "3") == 0 ){
                    GPIOD_ODR &= ~(1 << 14);
                }
                else if( strcmp(command[1], "4") == 0 ){
                    GPIOD_ODR &= ~(1 << 15);
                }
                else{
                    print_error();
                }
            }
            else if( strcmp(command[0], "LEDALL") == 0 ){   // LED ON ALL
                if( strcmp(command[1], "0") == 0 ){
                    GPIOD_ODR &= ~(1 << 12);
                    GPIOD_ODR &= ~(1 << 13);
                    GPIOD_ODR &= ~(1 << 14);
                    GPIOD_ODR &= ~(1 << 15);
                }
                else if( strcmp(command[1], "1") == 0 ){
                    GPIOD_ODR |= 1 << 12;
                    GPIOD_ODR |= 1 << 13;
                    GPIOD_ODR |= 1 << 14;
                    GPIOD_ODR |= 1 << 15;
                }
                else{
                    print_error();
                }
            }
            else if( strcmp(command[0], "FLASH") == 0 ){    // FLASH LED
                for(i = 0; i < 10; i++){
                    for(int j = 0; j < 100000; j++);
                    GPIOD_ODR ^= 1 << 12;
                    
                    for(int j = 0; j < 100000; j++);
                    GPIOD_ODR ^= 1 << 13;
                    
                    for(int j = 0; j < 100000; j++);
                    GPIOD_ODR ^= 1 << 14;
                    
                    for(int j = 0; j < 100000; j++);
                    GPIOD_ODR ^= 1 << 15;
                }
            }
            else{
                // ERROR
                print_error();
            }


            // remove Buffer content
            strncpy(buffer, "", BUF_SIZE);      
            for(i = 0; i < 3; i++){
                strncpy(command[i], "", strlen(command[i]));
            }

            for(i = 0; i < strlen(question); i++){      // print question
                USART2_DR = question[i];
                while( !(USART2_SR & (1<<7) ));
                while( !(USART2_SR & (1<<6) ));
            }
            count = 0;
        }
   
        
        USART2_CR1 |= (1<<5);
    }
}
```
