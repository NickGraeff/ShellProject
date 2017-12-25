# nsh
nsh is yet another shell built solely to exist. Cleverly (not actually clever) named similarly to other popular shells like zsh, csh, ksh...nsh wants to get in on that sweet, sweet clickity-clack, black and white action.

## Getting Started
First, make sure you're on Ubuntu Linux. This thing hasn't been tested on anything but that. If you're on something else and it works send a screenshot to me and I'll hang it on my wall. If you're on Ubuntu Linux and it doesn't work then figure out why, I won't be of any help. 

### Prerequisites
Besides the C standard library all nsh uses is `make` to actually build the project. Of course you need the `make` package to do this, so install it somehow. I recommend:
```
sudo apt-get install make
```

## Running nsh
Once you've `make`'d the project go ahead and just run it with `./nsh`. Hooray! It should work just like a normal shell like `bash` except it lacks any meaningful, distinct features. You can run (and even pipe, wow) any commands built in with Linux like `ls`, `cd`, etc.

## Features!
Hey, how about a feature list to get your motor running!

### Color-Support
Stare on in amazement as the console prints out text in a different color from the background!
<p align="center">
  <img src="https://i.imgur.com/Ci1Bn8q.png">
</p>

### Shell-Scripting Support
Feel your knees weaken as this shell calls on another shell to do something it isn't capable of.
<p align="center">
  <img src="https://i.imgur.com/K5Zz240.png">
</p>

### Black Magic
Prepare your jimmies for a rustling as you backspace your tabs and see that your input was completely malformed.
<p align="center">
  <img src="https://i.imgur.com/CJi1Fdu.png">
</p>

## Built with
Besides the C standard library and the `make` utility this entire project is devoid of any sort of external library support. 

## Contributing
If somehow you're actually interested in contributing go ahead and fork the project and then submit a `pull` request or however that works. I'll figure it all out later. 

##  Authors
Just me woohoo!
