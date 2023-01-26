const int pin = 6;
//Recuerda que debes usar el pin correcto que hayas utilizado en el esquema eléctrico de tu proyecto
  
void setup() {
  pinMode(pin, OUTPUT);  //definir pin como salida
}
  
void loop(){
  digitalWrite(pin, HIGH);   // poner el Pin en HIGH para activar el electroimán
  delay(10000);               // esperar un segundo
  digitalWrite(pin, LOW);    // poner el Pin en LOW para desactivar el electroimán
  delay(10000);               // esperar un segundo
}
