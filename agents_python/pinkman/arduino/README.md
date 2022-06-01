# Description of manipulator serial port protocol

**baudrate** = 115200
**line ending** - `\n`

Robot has 5 servos. First - lower carousel drive, last - gripper.
Memory of arduino stores min and max angle values of each servo as constants.
Start angle of each servos are stored in non-volatile memory.

## Behavior on connection (load)

Controller will send you line:

```
StartPos: 90  120  35  90  80  
```

It is start angles of each servo.

## Available commands

* 'm pos1 pos2 pos3 pos4 pos5 speedInPercentage\n' - move servos to given positions
* 's\n' - return current positions
* 'r\n' - stop moving
* 'e start1 start2 start3 start4 start5\n' - set servos start positions
* 'h speedInPercentage\n' - go to start position

Every command must ends with line ending symbol `\n`

If command was accepted you will recive text message `OK`.
In other case you will get text message `Wrong command` with list of available commands as above.

The answer on state request have a form:

```
pos1\tpos2\tpos3\tpos4\tpos5\tdistFromSonarIn_cm
```

Where *posN* - current value of angle of the corresponding servo drive, *distFromSonarIn_cm* - distance measured with sonar in cm. Delimiter symbol - `\t`

If you send any move command to controller (`m` or `h`) you will get message `END` when all servos will take the desired position.
