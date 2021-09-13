#include<MicroBit.h>

MicroBit uBit;
ManagedString result("");
int recL = 0; // receive Length
bool start = false;  // receiving start flag
bool finish = false; // receiving finish flag

void header(){
	recL = (result.charAt(0)-100)*10000 + (result.charAt(1)-100)*100 + (result.charAt(2)-100); // receive characters
	uBit.serial.printf("header OK\n"); // operation check
	uBit.serial.printf("%d %d %d\n",result.charAt(0),result.charAt(1),result.charAt(2)); 
}

void receive(){
	while(true){
		while(!start && !finish){ // Until the header is called
			result = result + uBit.serial.read(uBit.serial.rxBufferedSize(), SYNC_SLEEP);
			if(result.charAt(3) == 255) {
				start = true;
				header();
			}
			fiber_sleep(1);
		}
		while(start){ // After calling the header
			// Variable length
			result = result + uBit.serial.read(uBit.serial.rxBufferedSize(), SYNC_SLEEP);
			fiber_sleep(1);
		}
		fiber_sleep(1);
	}
}

void send(){
	while(true){
		while(!finish)fiber_sleep(1); // Until receiving is complete.
		uBit.serial.printf("received:%d characters\n", result.length());
		uBit.serial.send(result + "\n", SYNC_SLEEP);
		result = "";    // clear
		finish = false; // reset
		fiber_sleep(1000);
	}
}

/*
void receive_L(){
	while(true){
		while(! uBit.buttonB.isPressed()) fiber_sleep(100);
		start = false;
		if(recL == result.length()){
			uBit.serial.printf("Complete\n");
		}else{
			uBit.serial.printf("%d %d\n",recL,result.length());
			result = "";
			uBit.serial.printf("Receive Error!\n");
		}
		fiber_sleep(1000);
	}
}
*/

void checksum(){
	while(true){
		while(start && !finish){ // receiving 
			if(!uBit.serial.isReadable()){ // No data waiting in the buffer
				if(recL == result.length()){ // All data received
					start = false;
					// checksum
					int c_sum = 0;
					for(int i=4; i < result.length()-1; i++){
						c_sum = c_sum + result.charAt(i);
					}
					c_sum = c_sum%254;
					if(c_sum == 0){ // Exceptions
						c_sum = 254;
					}
					if(c_sum == result.charAt(result.length()-1)){
						uBit.serial.printf("Complete\n"); // receive complete
						finish = true;
					} else { // Error!
						result = "";
						uBit.serial.printf("Receive Error!\n");
					}
				}
			}
			fiber_sleep(1);
		}
		fiber_sleep(1);
	}
}

int main(){
	uBit.init();
	uBit.serial.setBaud(9600);
	uBit.serial.setRxBufferSize(255);
	create_fiber(receive);
	create_fiber(send);
	//create_fiber(receive_L);
	create_fiber(checksum);
	release_fiber();
}