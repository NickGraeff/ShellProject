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

But seriously here are <b>some</b> bugs that exist and <b>some</b> features I'd like to add as of this moment.

### Bugs/Issues

  <ul>
    <li>
      <p>Tabs, among other special characters, do not register correctly</p>
      <p>All there is to do in this regard really is to add additional code which does what the corresponding button press should do. To remove this "bug", code could just be written to ignore unknown characters. Tabs are a little more complicated because we would like those to auto-complete our query but for now simply adding extra spaces correctly would be adequate. This shell only supports ASCII characters as well, so any unicode characters don't register correctly either. </p>
    </li>
    <li>
      <p>Custom Commands Not Implemented</p>
      <p>As of right now the only commands implemented are "quit". That's pretty bad.</p>
  </li>
    <li>
      <p>Custom Commands Inefficient</p>
      <p>For parsing custom commands, the current method is to strcmp() the string entered by the user multiple times in an if-elseif-else chain until the correct command is found. This is horribly inefficient and a hash would do a much better job of it.</p>
    </li>
    <li>
      <p>Commands History Implemented Poorly</p>
      <p>The user's history of commands is currently stored in an array and each time the user adds a new command, the array copies each position to the next and places the newest commands in the [0] element. That's horrible. Clearly the commands history works a bit like a double-ended queue which is typically implemented with a linked list. Not only that but a linked list is just more efficient for this action. So ideally I would like to reimplement the commands history in this fashion.</p>
    </li>
  </ul>

### Features

  <ul>
    <li>
      <p>Make tab auto-complete as is expected in most shells</p>
      <p>I believe to do this manually (in C without external library support) you would need to have some knowledge of inodes and how file linking works.</p>
    </li>
    <li>
      <p>Support for turning on or off certain features</p>
      <p>With other shells certain shell features (such as shell history) can be turned on or off. Currently there is no setup at all for this.</p>
    </li>
    <li>
      <p>Parsing/Expanding Commands</p>
      <p>On a grander scale, such as interpreting a shell file, this will require a lot of effort and likely require a good amount of knowledge about compilers. However, on a smaller scale (simply implementing custom commands), this isn't as difficult. For example, when a user types "merge <file1> <file2> > <file3>" for example, ideally we would like to convert this into "cat file1 | cat file2" and then redirect the output from STDOUT to a file descriptor representing file3. As of right now this string manipulation this requires is incompatible with the way the strings are stored and later freed in memory. This is easily fixable and will be taken care of soon by me. </p>
    </li>
  </ul>

##  Authors
Just me woohoo!
