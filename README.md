# winnice
Command line tool for setting Process Priority in Windows

# Usage

```
> winnice --cpu-idle --pid 12345
```
This sets cpu priority of process '''12345''' to 'idle'.


```
> winnice --cpu-idle --pid 12345,7890
```
You can specify many pids.


```
> winnice --cpu-idle --executable notepad.exe
```
All notepad.exe instances will be 'idle'.


```
> winnice.exe --io-idle --executable app.exe
```
This sets ipu IO priority of process 'app.exe' to 'idle.

```
> winnice.exe --all-idle --detach-newprocess --new-process notepad
```
Launch notepad, set all prorities to 'idle' and detach it(not wait it to finish).
