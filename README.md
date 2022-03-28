# FT_IRC

## LAUNCH IRCII

```
irc <nickname> <server_ip>:<port>:<server_password>
```

## COMMAND TO TEST

/JOIN <chan_name>
/PART
/INVITE <nickname> <chan_name>
/TOPIC <topic of chan>
/PING <nickname>
/LIST
/NAMES <chan_name>
/NOTICE <nickname> <your_message>
/PASS <password>
/MODES <chan_name> <+|-><mode>

### CHAN_MODES

t (locked topic)
i (invited only)
s (secret)
p (private)
a (anonymous)
n (receive message from chan only)
m (only moderators can speaks)
l (number of user limited)

## BONUS

To send
```
/DCC SEND <receiver_nickname> <path_to_file>
```

To receive
```
/DCC GET <sender_nickname> <filename>
```