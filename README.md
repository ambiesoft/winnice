# winnice
Command line tool for setting Process Priority in Windows

# Environment
Visual Studio 2019 runtime library 

# Install 
Extract the downloaded archive and run *winnice_x.x.x.exe*

# Two executables
* *winnice.exe* is console app.
* *winnicew.exe* is GUI app (shows no console).

# Uninstall
Remove files

# Usage
### Set cpu priority of process 12345 to *idle*.

```
> winnice --cpu-idle --pid 12345
```


### You can specify many pids.
```
> winnice --cpu-idle --pid 12345,7890
```


### All notepad.exe instances will be *idle*.
```
> winnice --cpu-idle --executable notepad.exe
```


### This sets IO priority of process 'app.exe' to *idle*.
```
> winnice.exe --io-idle --executable app.exe
```


### Launch notepad, set all prorities (CPU,IO,Memory) to *idle* and detach it(not wait for it to finish).
```
> winnice.exe --all-idle --detach-newprocess --new-process notepad
```

# Support
If you have troubles, post *Issue* on <https://github.com/ambiesoft/winnice/issues>.

# Contact
- Author: Ambiesoft trueff
- E-mail: <ambiesoft.trueff@gmail.com>
- Webpage: <https://ambiesoft.com/>
- Forum: <https://ambiesoft.com/minibbs/>
- Development: <https://github.com/ambiesoft/winnice>