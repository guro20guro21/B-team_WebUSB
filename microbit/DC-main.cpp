#include<MicroBit.h>

MicroBit uBit;
ManagedString result("");
ManagedString sendstr(""); 
ManagedString result_c("");

int recL = 0; // receive lines
int countL = 0; // count lines
int countC = 4; // count character (4 = header size)
int splitValue = 64; // default
bool start = false;  // receiving start flag

void receive(){
	while(true){
		result = result + uBit.serial.read(uBit.serial.rxBufferedSize(), SYNC_SLEEP);
		fiber_sleep(1);
	}
}

void header(){
	while(true){
		if(result.charAt(3) == 255 && !start){
			recL = (result.charAt(0)-100)*100 + (result.charAt(1)-100); // receive lines
			splitValue = result.charAt(2);
			uBit.serial.printf("header OK\n"); // operation check
			uBit.serial.printf("%d%d\n",result.charAt(0)-100,result.charAt(1)-100);
			start = true;
		}
		fiber_sleep(1);
	}
}

void checksum(){
	while(true){
		if(result.length() >= (countC+splitValue+1)){
			result_c = result.substring(countC,splitValue+1);
			int c_sum = 0;

			for(int j=0; j<splitValue; j++){
				c_sum = c_sum + result_c.charAt(j); // sum

				// add start
				if(result_c.charAt(j-1) == 117 && result_c.charAt(j) == 112) // up
				{
					uBit.serial.printf("up\n");
					uBit.io.pin[14].setDigitalValue(0);
					uBit.io.pin[13].setAnalogValue(500);
					uBit.io.pin[16].setDigitalValue(0);
					uBit.io.pin[15].setAnalogValue(500);
					uBit.sleep(5000);
					uBit.io.pin[13].setAnalogValue(0);
					uBit.io.pin[15].setAnalogValue(0);
					fiber_sleep(1000);
				}else if(result_c.charAt(j-4) == 114 && result_c.charAt(j-3) == 105 && result_c.charAt(j-2) == 103 && result_c.charAt(j-1) == 104 && result_c.charAt(j) == 116) // right
				{
					uBit.serial.printf("right\n");
					uBit.io.pin[14].setDigitalValue(1);
					uBit.io.pin[13].setAnalogValue(500);
					uBit.io.pin[16].setDigitalValue(0);
					uBit.io.pin[15].setAnalogValue(500);
					fiber_sleep(500);
				}else if(result_c.charAt(j-3) == 108 && result_c.charAt(j-2) == 101 && result_c.charAt(j-1) == 102 && result_c.charAt(j) == 116) // left
				{
					uBit.serial.printf("left\n");
					uBit.io.pin[14].setDigitalValue(0);
					uBit.io.pin[13].setAnalogValue(500);
					uBit.io.pin[16].setDigitalValue(1);
					uBit.io.pin[15].setAnalogValue(500);
					fiber_sleep(500);
				}else if(result_c.charAt(j-3) == 100 && result_c.charAt(j-2) == 111 && result_c.charAt(j-1) == 119 && result_c.charAt(j) == 110) // down
				{
					uBit.serial.printf("down\n");
					uBit.io.pin[14].setDigitalValue(1);
					uBit.io.pin[13].setAnalogValue(500);
					uBit.io.pin[16].setDigitalValue(1);
					uBit.io.pin[15].setAnalogValue(500);
				}else if(result_c.charAt(j-3) == 115 && result_c.charAt(j-2) == 116 && result_c.charAt(j-1) == 111 && result_c.charAt(j) == 112) // stop
				{
					uBit.serial.printf("stop\n");
					uBit.io.pin[13].setAnalogValue(0);
					uBit.io.pin[15].setAnalogValue(0);
				}
				// add end

			}
			c_sum = c_sum % 254;
			if(c_sum == 0) c_sum = 254; // Exceptions

			uBit.serial.printf("%d %d ", result_c.charAt(splitValue), c_sum);
			if(c_sum == result_c.charAt(splitValue)){
				uBit.serial.printf("OK\n"); // checksum OK
			} else {
				uBit.serial.printf("Checksum Error!\n"); // Error!
			}

			sendstr = sendstr + result_c.substring(0,splitValue); // checksum remove
			countC = countC + splitValue+1; // start position +17
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
		fiber_sleep(1000);
	}
}

void reset(){
	while(true){
		while(! uBit.buttonB.isPressed()) fiber_sleep(100);
		result = "";
		sendstr = "";
		result_c = "";
		recL = 0;
		countL = 0;
		countC = 4;
		start = false;
		uBit.io.pin[1].setServoValue(90);
		uBit.io.pin[2].setServoValue(120);
		fiber_sleep(1);
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
	create_fiber(reset);
	release_fiber();
}
