 /* Header Datei, um Tasten einzulesen
 *  Original: https://www.velleman.eu/downloads/29/vma203_a4v02.pdf
 * */
 
int lcd_key = 0;
enum{ btnRIGHT = 0, btnUP = 1, btnDOWN = 2, btnLEFT = 3, btnSELECT = 4, btnNONE = 5};


int read_LCD_buttons(){
	int adc_key_in = analogRead(0);
	if(adc_key_in > 1000) return btnNONE;
  else  delay(200);
	if(adc_key_in < 50) return btnRIGHT;
	if(adc_key_in < 250) return btnUP;
	if(adc_key_in < 400) return btnDOWN;
	if(adc_key_in < 600) return btnLEFT;
	if(adc_key_in < 800) return btnSELECT;
	return btnNONE;
}

boolean buttonPressed(int val){
		return val !=5;
}
