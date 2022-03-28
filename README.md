# FT_IRC

## LAUNCH IRCII

```
irc <nickname> <server_ip>:<port>:<server_password>
```

## COMMAND TO TEST

/JOIN <chan_name,chan_name...>
/PART <chan_name,chan_name...>
/INVITE <nickname> <chan_name>
/TOPIC <topic of chan>
/PING <nickname>
/LIST
/NAMES <chan_name>
/NOTICE <nickname> <your_message>
/PASS <password>
/MODES <chan_name> <+|-><mode>
/MSG <chan_name> <your_message>
/NICK <name>
/USER <name>
/QUIT <reason>
/NAMES <chan_name>
/MOTD
/TIME
/INFO
/AWAY
/USERHOST <user_name,user_name...>
/ISON <user_name,user_name...>
/TOPIC <topic>
/WHO <chan_name>
/KICK <user_name>
/KILL <user_name>
/WHOIS <user_name>
/WHOWAS <user_name>
/ADMIN
/LUSERS
/OPER <oper_name>
/PASS <password>

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