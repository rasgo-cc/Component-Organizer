Introduction
============

Component Organizer (or CO for short) is a small, cross-platform and very easy to use application targeted at all those people who have electronics as (at least) an hobby. It's main purpose is to avoid to google for the same datasheet hundreds of times or "hey I know I already downloaded that datasheet but I can't find it now on my computer", so you spend less time gathering information and have more time to look at the hardware which off course is the best part :-). With Component Organizer you can easily search and manage your datasheets and application notes. You can as well manage your component's stock and be alerted when it's on low stock or without stock. A component can be an amplifier, a microcontroller, but also a development board, a compiler... All those things that have an heavy document describing it's features and/or how it works. It doesn't require installation and so it's very portable. You can use it in a PEN drive or with Dropbox allowing your data to be always acessible.

Official webpage: http://3xdigital.com/projects/comporg/

Building
==========================

CO is C++ with Qt4. 
The tools used for development:
QtCreator (2.4.1)
Qt (4.7.4)

The easiest way to build CO is by using QtCreator IDE. Download it and Qt 4.7.4 from Qt's website (http://qt.nokia.com/). Then:

1) Run QtCreator

2) Open CO's project file (comporg.pro)

3) Build > Run qmake

4) Build > Build All

Depending on the OS used (Win, Linux, Mac OS), an OS-specific file is created under "_build/release/bin directory". For example, if you are on Windows a file named "comporg_win.exe" will be created. 

Using 
==========================

CO it's pretty straight forward to use. Just start by adding components through File>Add new...>Component. By right-clicking on the table you'll get a context menu with everything you need to manage your items.

Contributing (!)
==========================

Git is a wonderful thing. If you aren't familiar with it I would recomend you to google for some tutorials, there are several good ones out there. Then, fork/clone CO! Do whatever modifications you find useful, don't be afraid to scratch on it. The next step is to make a pull request. And that's it. Simple, isn't it?
There are however some priorities in the CO's further development. See the wiki for more details.

Bug reports, suggestions and feature requests, please use the "Issues" tab. They are very welcomed!

"Hacking"
==========================

CO uses a XML file to store data. You can find it on /data/data.xml. Open it with a text editor (I like Notepad++) to see how info is organized. This file means that you can edit your data directly and then reflect those changes on CO. For example, to add a new package you just have to add a line like this: <package name="mynewpackage"/> between the <packages> ... <packages/> tags. The same principle applies to everything else, you just have to understand how data is organized. However, you can also mess up all your data this way! Before make any changes make sure you backup data.xml. If you are not sure if you are doing things right just don't edit data.xml and use CO's interface.
