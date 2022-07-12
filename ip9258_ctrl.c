/*
* IP Power 9258 Controller from Serial Port ( using PIC12F683)
* 2022.July.11, 7M4MON
* Ref. https://gist.github.com/nrclark/e43ba5191eb3ac03c3c893c1964de973
* License : MIT

* How to use.  
* 1. Set 4 DIP Switchs to your desired state.
* 2. Press tactile switch (Green LED glows).
* 3. Hold button 2sec while brinking the red LED.
* 4. Done.
*/


#include	<12f683.h> 

#fuses INTRC_IO,NOWDT,PUT,NOPROTECT,NOMCLR
#use delay(CLOCK = 8000000)
#use rs232(baud=19200,parity=N,xmit=PIN_A5,rcv=PIN_A4)

void main() {
    
    char dip_sw_state;
    char cmd_string[32];
    int8 i;
    
    setup_adc_ports(NO_ANALOGS);
    port_a_pullups(0b010111);
    set_tris_a(0b011111);              // 1 = input
    
    delay_ms(500);                     // 安定待ち
    
    // DIP SWの状態を読み取る。0がON。
    dip_sw_state = input(PIN_A3) + input(PIN_A2) * 2 + input(PIN_A1) * 4 + input(PIN_A0) * 8;
    
    // コマンドの文字列を作成。"0\ADEBUG9258\Z0\AP060E\Z" で1chだけON。nullを含め25バイト。
    sprintf(cmd_string, "0\\ADEBUG9258\\Z0\\AP06%X\\Z", dip_sw_state);
    
    i = 0;
    
    // 一気に送ると反応しないので、60msに１文字ずつ送信する。
    while(cmd_string[i] != 0){
        putc(cmd_string[i]);
        delay_ms(60);
        i++;
    }
    while(true){;};
}
