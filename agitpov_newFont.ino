
int textCount = 1;
wchar_t* textArray[] = {
  L"I ♥ MondoMonde"};
int taml[] = {14};

  


unsigned long timeStamp;
int counter;

void setup()
{  
  Serial.begin(9600);
  povSetup();
  povSetMessage(textArray[counter], taml[counter]);    
  

}


void loop()
{
  
  /*
  if ( millis() - timeStamp >= 1000 ) {
    timeStamp = millis();    
    counter = (counter + 1) % textCount;
    povSetMessage(textArray[counter], taml[counter]);
   
  } */
  
  povWaitAndDisplay();
    
}



