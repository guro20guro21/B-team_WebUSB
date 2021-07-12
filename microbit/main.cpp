#include<MicroBit.h>

MicroBit uBit;
ManagedString result("");
ManagedString str("");

void receive(){
        while(true){
                // 固定長ではなく現在バッファにあるデータ長を読み取るように変更
                // これでASYNCでもSYNC_SLEEPでもどちらでも正しく動く
                str = uBit.serial.read(18, SYNC_SLEEP);
                //int checksum = str.charAt(16);
                result = result + str.substring(0,16);
                //result = result + uBit.serial.read(uBit.serial.rxBufferedSize(), SYNC_SLEEP);
                //result = result + uBit.serial.read(SYNC_SLEEP);
                // 譲らないとコンテクストが切り替わらないことに注意
                fiber_sleep(1);
        }
}

void send(){
        while(true){
                while(! uBit.buttonA.isPressed()) fiber_sleep(100);
                uBit.serial.printf("received:%d characters\n", result.length());
                uBit.serial.send(result, SYNC_SLEEP);
                // チャタリング防止
                fiber_sleep(1000);
        }
}

void clear(){
        while(true){
                while(! uBit.buttonB.isPressed()) fiber_sleep(100);
                result = "";
        }
}

int main(){
        uBit.init();
        uBit.serial.setBaud(9600);
        uBit.serial.setRxBufferSize(255);
        create_fiber(receive);
        create_fiber(send);
        create_fiber(clear);
        release_fiber();
}
