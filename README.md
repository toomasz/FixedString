# Arduino Fixed String

String library for embedded systems that won't make your heap fragmented

###### Why this library was created?

 - Arduino String uses malloc internally and will eventually make heap fragmented and program will crash
 - C strings are too hard to use and it's easy to overflow string buffer that will be hard to debug
 -Heap fragmentation is especially dangerous in Embedded systems du to low memory, e.g Arduino has 2k bytes only, esp8266
 - I recommend reading following articles:
  - [The evils of arduino strings](https://hackingmajenkoblog.wordpress.com/2016/02/04/the-evils-of-arduino-strings/)
  - [What is heap fragmentation] (http://blog.quasardb.net/what-is-heap-fragmentation/)
###### Features:
  - String size need to be specified upfront, like in C string:
    So instead of using:
    `char[20] str = "some string"`
    use:
    `SimpleString<20> str = "some string"`
  - c_str() method will return null terminated string
  - When string buffer overrun occurs, program won't crash, instead glabl variable FixedString_OverflowDetected will be set to true.
  
###### Example:
 
 Following program:
 
```
#include <FixedString.h>


void setup() 
{
   Serial.begin(115200);   
   delay(500);
    FixedString<100>  str= "abc";
    str.debug();
    str.append('x');
    str.debug();
    str.append('y');
    str += "12345";
    str.debug();
    str += "12";
    str.debug();

    FixedString<100> str1 = FixedString<100>("Some ") + " may never live";
    str1.debug();

    FixedString<50> str3 = " but the crazy will never die";
    str1 += str3;

    str1.debug();

    FixedString<8> formatString;
    formatString.appendFormat("%d %d %d", 10, 20, 30);
    
    formatString.debug();
    // string is full, this append will fail and set FixedString_OverflowDetected to true
    formatString.append('X'); 
    formatString.debug();
}

void loop() 
{
   if(FixedString_OverflowDetected)
   {
    Serial.println("Fatal error, one of FixedString were to small to fit content!");
    for(;;) delay(500);
   }  
}
```
Will output:
```
content: 'abc' length: 3, free = 97
content: 'abcx' length: 4, free = 96
content: 'abcxy12345' length: 10, free = 90
content: 'abcxy1234512' length: 12, free = 88
content: 'Some  may never live' length: 20, free = 80
content: 'Some  may never live but the crazy will never die' length: 49, free = 51
content: '10 20 3' length: 8, free = 0
content: '10 20 3' length: 8, free = 0
Fatal error, one of FixedString were to small to fit content!
```
