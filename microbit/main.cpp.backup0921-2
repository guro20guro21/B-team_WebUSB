#include<MicroBit.h>

MicroBit uBit;
ManagedString result("");
ManagedString sendstr(""); 
ManagedString result_c("");
int recL = 0; // receive lines
int countL = 0; // count lines
bool start = false;  // receiving start flag
//bool finish = false; // receiving finish flag

void receive(){
	while(true){
		result = result + uBit.serial.read(uBit.serial.rxBufferedSize(), SYNC_SLEEP);
		fiber_sleep(1);
	}
}

void header(){
	while(true){
		if(result.charAt(2) == 255 && !start){
			recL = (result.charAt(0)-100)*100 + (result.charAt(1)-100); // receive lines
			uBit.serial.printf("header OK\n"); // operation check
			uBit.serial.printf("%d%d\n",result.charAt(0)-100,result.charAt(1)-100);
			start = true;
		}
		fiber_sleep(1);
	}
}

void checksum(){
	int i=3;
	while(true){
		if(result.length() >= (i+17)){
			result_c = result.substring(i,17);
			int c_sum = 0;

			for(int j=0; j<16; j++) c_sum = c_sum + result_c.charAt(j); // sum
			c_sum = c_sum % 254;
			if(c_sum == 0) c_sum = 254; // Exceptions

			uBit.serial.printf("%d %d ", result_c.charAt(16), c_sum);
			if(c_sum == result_c.charAt(16)){
				uBit.serial.printf("OK\n"); // checksum OK
			} else {
				uBit.serial.printf("Checksum Error!\n"); // Error!
			}

			sendstr = sendstr + result_c.substring(0,16); // checksum remove
			i = i + 17; // start position +17
			countL++;   // line count
		}
		fiber_sleep(1);
	}
}

void send(){
	while(true){
		while(! uBit.buttonA.isPressed()) fiber_sleep(100);
		//while(!finish)fiber_sleep(1); // Until receiving is complete.
		uBit.serial.printf("received:%d characters\n", sendstr.length());
		uBit.serial.send(sendstr + "\n", SYNC_SLEEP);
		result = "";    // clear
		//finish = false; // reset
		fiber_sleep(1000);
	}
}

int main(){
	uBit.init();
	uBit.serial.setBaud(9600);
	uBit.serial.setRxBufferSize(255);
	create_fiber(receive);
	create_fiber(checksum);
	create_fiber(header);
	create_fiber(send);
	release_fiber();
}
