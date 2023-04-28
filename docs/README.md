# GUI Libraries
## What are GUI
The GUI, or graphical user interface, is a type of user interface that uses images and graphic objects, such as icons, buttons, windows, and menus, to represent the information and actions available on a computer system. In a GUI, users can interact with the computer by selecting and manipulating these visual elements through the use of a mouse, keyboard, or other input devices, instead of having to type text commands as in command line-based interfaces.

The GUI has become widely used in modern computing as it provides an intuitive and visual way to interact with software applications and operating systems. They allow users to take action by selecting visual elements and provide real-time visual feedback on system status and actions taken.

GUIs have been a significant improvement over command line interfaces in that they have made computing more accessible to non-technical users and have simplified the overall user experience. GUIs are used in a wide variety of devices, from personal computers and mobile devices, to home appliances, entertainment systems, and more.

## Origin
![image](https://user-images.githubusercontent.com/99950497/235006848-ed006d9a-6361-476d-9d45-c6ac50abdc8b.png)

The history of the graphical user interface (GUI) begins in the 1970s, when Xerox's research center in Palo Alto, California, developed the first GUI called the PARC User Interface. This interface included elements such as windows, checkboxes, buttons, menus, and scroll bars, which are still in use today. Xerox developed several computers that included this GUI, such as the Xerox Alto model and the Xerox Star 8010.

In 1979, a delegation from Apple visited Xerox PARC and was impressed by the capabilities of the GUI. Apple decided to go with this interface instead of the traditional text mode and formed a team of developers who worked on their own version of the GUI. This team developed the Apple Lisa operating system, which was designed specifically for a graphical environment and allowed for multitasking.

![image](https://user-images.githubusercontent.com/99950497/235007073-01688dd8-161a-40f7-a36b-d3d92aa3833f.png)
The Apple Lisa included things like the desktop, icons, windows, and an options menu at the top of the screen, which have become standard features of GUIs.

The Apple Smooth GUI laid the foundation for the development of the modern GUI, and while its design is now considered dated, its elements and concepts have evolved and been refined in today's operating systems such as macOS, Windows, and Linux. The GUI has become the de facto standard for interacting with most electronic devices and has changed the way people use and interact with technology.
## What are the options to develop the GUI for C++
There are many options that we have for developing GUI in C++, each of these options offers its own features and advantages in terms of tools, widgets and compatibility with different operating systems. The most popular are the following:
### Qt
Qt is a widely used GUI application development framework in C++. It is developed by the Qt Company and offers a rich set of tools and widgets for creating feature-rich user interfaces such as windows, buttons, dialogs, graphics, animations, and more. In addition, Qt has a large developer community and extensive documentation, making it easy to learn and use in GUI development for games. Recommended for large projects.
### GTK
GTK (GIMP Toolkit) is a set of open source development tools for creating graphical user interfaces such as windows, buttons, dialogs, lists, and more. It also has a modular architecture that allows you to customize the appearance and behavior of the widgets. It is especially popular in the Linux development environment and is used in projects such as the GNOME desktop environment. GTK is written in C and has bindings for various programming languages, including C++, allowing it to be used in developing GUI applications in C++.
### Dear ImGui
Dear ImGui is a lightweight and easy to use GUI library for C++. It is specifically designed for the development of real-time debugging tools and applications. It provides a simple and fast API for creating user interfaces, and is easily integrated into any C++ application. Dear ImGui stands out for its ease of use, its minimalist approach and its good compatibility with other development tools.
### wxWidgets
wxWidgets is a C++ library that allows you to develop cross-platform GUI applications. It provides a set of tools and widgets for creating user interfaces such as windows, buttons, dialogs, and more. wxWidgets is known for its native appearance on different operating systems and its broad platform support, which allows you to develop GUI applications that run on Windows, macOS, Linux, and other platforms.
### FLTK
FLTK (Fast Light Toolkit) is an open source GUI library for C++. It is characterized by being light and fast, and provides a set of tools and widgets to create user interfaces. FLTK is designed to be easy to integrate into C++ applications and has an open source license that is compatible with most projects.
### Ultimate++
Ultimate++ is a GUI application development framework in C++. It provides a comprehensive set of tools and widgets for creating user interfaces, and comes with a wide range of features, including cross-platform support, visual design capabilities, and a library of pre-built app templates. Ultimate++ is designed to be easy to use and offers a high level of productivity in developing C++ GUI applications. It also comes with a library of pre-built app templates that makes it easy to create professional user interfaces in games.

## GUI library that can be used and why
SDL_GUI_API is a programming library that provides a set of tools and functions for creating a graphical user interface (GUI) for applications in SDL. This library is one of the easiest options I've found for implementing a GUI in SDL projects.

If we are working on a project with SDL that requires a user interface, it is best to use libraries such as SDL_GUI_API due to the various functions it offers us to create an attractive and easy-to-use interface. For example, we can use this library to create buttons, menus, progress bars, and other interface elements. We can also customize the appearance of these elements to fit the aesthetics of our project.

In addition, this library is easy to use thanks to its documentation, which allows a faster and more effective implementation. The library is also cross-platform compatible, which means we can create consistent, high-quality user interfaces across different operating systems.

It is because of all this, that if we need to create a GUI for our SDL project, SDL_GUI_API is a recommended library to use due to its various functions, ease of use and compatibility with different platforms. With this library, we can create an attractive and effective user interface that will enhance the user experience of our project.

## Differences regarding developing the GUI with libraries
GUI libraries provide a wide range of tools that simplify the creation of a user interface. These tools can include buttons, text boxes, dropdown menus, progress bars, graphics, animations, and many other features that are essential in a modern GUI. With these libraries, developers can create beautiful user interfaces with just a few lines of code.

In addition, these libraries also offer a large number of customization options for the user interface. Developers can customize the appearance of GUI elements to match the overall design of the application and improve the user experience. For example, they can customize the color, shape, size, and animations of buttons and other interface elements.

Another benefit of using GUI libraries is that they often include clear documentation and code examples so that developers can easily learn how to use them. This speeds up the development process and helps developers resolve common issues quickly.

## TODOs
The first thing to do is look for some libraries that we like, once we have it, we have to see if it is compatible with our project and if so, look at tutorials if they exist and start implementing it in our project.

TODO 1: We create a folder called SDL_GUI with the same structure as the others.

![image](https://user-images.githubusercontent.com/99950497/235032467-77fcfc01-bc8c-4e02-9eee-b3393bfc41cf.png)

TODO 2: We put the #include in the main

TODO 3: We fix the errors of the includes so that they access the corresponding folders and we add SDL_ttf if necessary.
![image](https://user-images.githubusercontent.com/99950497/235032441-7d4a2ecd-af62-4eb8-970d-6163551e6ec6.png)

TODO 4: We go to the project properties and fix the linker
![image](https://user-images.githubusercontent.com/99950497/235032358-c0371798-f6f8-4925-88d8-4e18713b360e.png)


## Conclusions
My conclusions is that if we get a library that we can easily implement in our project, it is advisable to do so, since it would save us a lot of time. But with the project that we are doing, I think that SDL is enough, since the GUI libraries are very difficult to implement, since you have to know several languages, or know how to use the CMD of the PC. There can also be many bugs and for very large libraries, it is difficult to implement them in already started projects, that's why I recommend using the GUI libraries before starting the project.

## References
[Definition](https://www.computerweekly.com/es/definicion/Interfaz-grafica-de-usuario-o-GUI#:~:text=Una%20GUI%20es%20una%20interfaz,puramente%20textual%20para%20una%20computadora)

[Make a GUI](https://es.quora.com/C%C3%B3mo-puedo-hacer-una-GUI-en-C)

[Tutorial FML](https://www.youtube.com/playlist?list=PL-EPeghw5sXg3t0egTspfHdUYMDqyOLwf)

[GUI programs](https://www.quora.com/What-is-the-way-to-start-developing-GUI-programs-apps-in-C-for-PCs)

[GUI Libraries](https://en.wikipedia.org/wiki/List_of_platform-independent_GUI_libraries)

[SDL_GUI_API](https://github.com/ARahimKhan/SDL_GUI_API)

[Definition Wiki](https://es.wikipedia.org/wiki/Interfaz_gr%C3%A1fica_de_usuario)

[Origin](https://blogthinkbig.com/del-texto-a-las-ventanas-como-llegamos-a-la-interfaz-grafica-de-usuario)

[Apple Lisa](https://es.wikipedia.org/wiki/Apple_Lisa)


