#include<MicroBit.h>

MicroBit uBit;
ManagedString result("");
ManagedString str("");

void receive(){
        while(true){
                str = uBit.serial.read(18, SYNC_SLEEP);
                //int checksum = str.charAt(16);
                result = result + str.substring(0,16);
                fiber_sleep(1);
        }
}

void send(){
        while(true){
                while(! uBit.buttonA.isPressed()) fiber_sleep(100);
                uBit.serial.printf("received:%d characters\n", result.length());
                uBit.serial.send(result, SYNC_SLEEP);
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
        create_fiber(receive);
        create_fiber(send);
        create_fiber(clear);
        release_fiber();
}
