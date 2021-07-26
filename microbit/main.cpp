#include<MicroBit.h>

MicroBit uBit;
ManagedString result("");
ManagedString str("");
ManagedString delimeter("\n"); // delimetar
int count = 0; // line count
int recL = 0; // receive Line

void header(){
	recL = str.charAt(1); // receive Line
	result = ""; // clear
        count = 0; // count reset
	uBit.serial.printf("^^\n"); // operation check
}

void receive(){
	while(true){
		// Variable length start
		str = uBit.serial.readUntil(delimeter, SYNC_SLEEP);
		int sizeL = str.length();
		int checksum = str.charAt((sizeL - 1));
		result = result + str.substring(0, (sizeL - 1));
		if(str.charAt(0) == 255) header();
		int c_sum = 0;
		for(int i=0; i<(sizeL-1); i++){
			int sum_p = str.charAt(i);
			c_sum = c_sum + sum_p;
		}
		// Variable length finish


		c_sum = c_sum%254;
                if(c_sum == 0){
                        c_sum = 254;
                }

		uBit.serial.printf("%d %d ", checksum, c_sum);
		if(checksum == c_sum){
			uBit.serial.printf("OK\n");
		}else{
			uBit.serial.printf("A\n");
		}
                count++;
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

void receive_L(){
	while(true){
		while(! uBit.buttonB.isPressed()) fiber_sleep(100);
                if(recL == count){
                        uBit.serial.printf("Complete\n");
                }else{
                        uBit.serial.printf("A\n", count);
                }
                fiber_sleep(1000);
	}
}

int main(){
	uBit.init();
	uBit.serial.setBaud(9600);
	create_fiber(receive);
	create_fiber(send);
	create_fiber(receive_L);
	release_fiber();
}
