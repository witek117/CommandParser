# CommandParser 
Project CLI to parse string commands

### How to use?
```cpp
// Create SimpleStream object based on SimpleStream_interface class.
Stream stream;

// Create callback functions
void callback0();
void callback1(const char* data);
void callback2(int data);
void callback3(float data1, int data2);

// Create commands
Command c0("c0", callback0);
Command c1("c1", callback1, false);
Command c2("c2", callback2, true);
Command c3("c3", callback3, false);

// Create command manager for maximum 4 commands
CommandManager<4> command_manager(stream);

// In setup function run init
command_manager.init();

// Add command to command manager
command_manager.addCommand(&c0);
command_manager.addCommand(&c1);
command_manager.addCommand(&c2);
command_manager.addCommand(&c3);

// In loop run
command_manager.run();
```
