# nsh
nsh is yet another shell built mainly to better my understanding of operating systems, parsing, and the C language. Cleverly (not actually clever) named similarly to other popular shells like zsh, csh, ksh...nsh wants to get in on that sweet, sweet clickity-clack, black and white action.

## Getting Started
Firstly, I only guarantee that this program will run absolutely correctly on Ubuntu Linux 14.04. I'm sure it'll compile correctly on other versions of Linux. I was actually able to compile it with Cygwin on Windows 10 and basic functionality was there, though editing commands didn't seem to work.  

### Prerequisites
Besides the C standard library all nsh uses is `make` to actually build the project. Of course you need the `make` package to do this, so install it somehow. I recommend:
```
sudo apt-get install make
```

## Running nsh
Once you've created the executable for the project go ahead and just run it with `./nsh`. Hooray! It should work just like a normal shell like `bash`. You can run, and pipe, any number of commands built in with Linux like `ls`, `cd`, etc. The input isn't limited so you can type in many commands at once, although scripting isn't supported yet. I need to work a lot more on the parser and implement an interpreter before that happens. 

## Features!
Hey, how about a feature list to get your motor running!

### Limitless piping
Use as many pipes as you like to redirect input to the next commands output.
<p align="center">
  <img src="https://i.imgur.com/Sv24kP4.png">
</p>

### Unlimited input
Type as much as you like and feel your heart strings get pulled as the program doesn't crash.
<p align="center">
  <img src="https://i.imgur.com/qmYs49G.png" style="height=100px; width=100px;">
</p>

### Support for custom commands
As of right now the custom commands supported are:
* `merge`
* `pause`
* `history`
* `cd`
* `quit`
<p align="center">
  <img src="https://i.imgur.com/hUyhriO.png">
</p>

## Built with
Besides the C standard library and the `make` utility this entire project doesn't use any external libraries. I might look around and see if there's an easily available library that will interface with the terminal you're using independent of what OS it is so that it might work better on Windows.

## Contributing
If you're actually interested in contributing go ahead and fork the project and then submit a `pull` request.

Here are <b>some</b> bugs that exist and <b>some</b> features I'd like to add as of this moment.

### Bugs/Issues

  <ul>
    <li>
      <p>Tabs, among other special characters, do not register correctly</p>
      <p>All there is to do in this regard really is to add additional code which does what the corresponding button press should do. To remove this "bug", code could just be written to ignore unknown characters. Tabs are a little more complicated because we would like those to auto-complete our query but for now simply adding extra spaces correctly would be adequate. This shell only supports ASCII characters as well, so any unicode characters don't register correctly either. That would be nice to add eventually as well.</p>
    </li>
    <li>
      <p>Empty input isn't handled correctly</p>
      <p>When the user type nothing or just spaces it attempts to execute some file with that label it seems. Could probably just add a special case in the parser for this.</p>
    </li>
    <li>
      <del><p>Custom Commands Not Implemented</p>
        <p>As of right now the only commands implemented are "quit". That's pretty bad.</p></del>
  </li>
    <li>
      <p>Custom Commands Inefficient</p>
      <p>For parsing custom commands, the current method is to strcmp() the string entered by the user multiple times in an if-elseif-else chain until the correct command is found. This is horribly inefficient and a hash would do a much better job of it. I might implement this in C++ because I really don't want to write a hash library myself.</p>
    </li>
    <li>
      <del><p>Commands History Implemented Poorly</p>
      <p>The user's history of commands is currently stored in an array and each time the user adds a new command, the array copies each position to the next and places the newest commands in the [0] element. That's horrible. Clearly the commands history works a bit like a double-ended queue which is typically implemented with a linked list. Not only that but a linked list is just more efficient for this action. So ideally I would like to reimplement the commands history in this fashion.</p></del>
    </li>
  </ul>

### Features

  <ul>
    <li>
      <p>Make tab auto-complete as is expected in most shells</p>
      <p>Could probably do this just using <code>ls</code> and <code>grep</code> and then retrieve the output. If there's only one file just auto-complete the line with it's name, otherwise print the output.</p>
    </li>
    <li>
      <p>Support for turning on or off certain features</p>
      <p>With other shells certain shell features (such as shell history) can be turned on or off. Currently there is no setup at all for this.</p>
    </li>
    <li>
      <p>Parsing/Expanding Commands</p>
      <p>On a grander scale, such as interpreting a shell file, this will require a lot of effort and likely require a good amount of knowledge about compilers/interpreters. However, on a smaller scale (simply implementing custom commands), this isn't as difficult. For example, when a user types "merge <file1> <file2> ... <fileN> > <fileN+1>" for example, ideally we would like to convert this into "cat file1 file2 ... fileN" and then redirect the output from STDOUT to a file descriptor representing fileN+1. I actually took care of this already for several custom commands. It's not too hard to add additional custom commands as of this moment.<del>As of right now this string manipulation this requires is incompatible with the way the strings are stored and later freed in memory. This is easily fixable and will be taken care of soon by me.</del></p>
    </li>
  </ul>

##  Authors
Just me woohoo!
