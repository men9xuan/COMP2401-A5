## Fair Simulator

This c program simulates a Fair that allows Guests to go onto Rides in the Fair. 

This program utilizes threads to handle incoming requests. ```fairApp.c``` spawns a thread for each ride and a display thread to display the fair information. 

```generator.c``` calls ```guest.c``` to create a thread for each guest that connects to the fair server.

A guest thread simulates the process of a guest taking rides until he/she runs out of tickets.

### Screenshots

<img src="https://github.com/men9xuan/Fair-simulator/raw/main/screenshot.png" width="500" >

<img src="https://github.com/men9xuan/Fair-simulator/raw/main/screenrecord.gif" width="500" >

### Usage:

Compile Instruction

> ```make```

Run Instructions:
1. start up fair server in the background
  ```./fairApp & ```
  
2. generate guests 
  ```./generator```
  
3. quit Fair Simulator
  ```./stop```
