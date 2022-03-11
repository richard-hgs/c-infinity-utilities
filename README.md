# C-Infinity-Utilities
C++ Utilities, DLL for GUI, Layout Managers, PDB Symbol Parser, Input(Mouse double click in less than 100ms fix), Raw disk data writer, And much more.

## InfinityGui - Library created to manage UI in C++ like the Android LayoutManager
 - I'm only created the LinearLayout and ImageView simple bare minimum implementation.

## InfinityPdbParser, InfinityPdbInspector, InfinityCrashlytics - Crash report using PDB Symbol file using Streamed DIA(Debug Access Interface)
 - Working if the PDB file bundled with the application, but can be used in server, but i didn't implemented this option yet.

## InfinityPcre  - Sample of PCRE usage
 - I created a console application that require this library

## InfinityPcreTools - Console CLI application that can filter a string using regex and get the result as JsonObject
 - Usage example: InfinityPcreTools --regex "est" --text "Testando"

## InfinityArchiveTools - Write ISO or IMG binary data directly in given disk WITHOUT FILE SYSTEM.
 - Created to write my Operating System kernel image directly in the pen drive to test in a real machine
 - 
 - WARNING: Be careful if you use this command in your hard drive WILL CORRUPT HARD DRIVE FILE SYSTEM BECAUSE WILL ERASE FILE SYSTEM DATA IN SECTORS WRITTEN.
 - drive-type 2 = EXTERNAL HARD DRIVE LIKE PEN-DRIVE
 - drive = Logical name of the drive to prevent you from writting in incorrect disk and LOSING YOUR DATA.
 - Open a CMD:
      wmic diskdrive list brief
      This will list your drives select one to write knowing that YOU CAN LOSE YOUR DATA IN THIS DISK BE CAREFUL.
 - Usage Example: InfinityArchiveTools --write --drive \\?\PhysicalDrive999 --drive-type 2 --input-file "build\floppy.img" --sectors 512
      This command will write the data of floppy.img bin into my Pendrive in binary form without file system.

## InfinityLastInput - Receive input keys and mouse events
 - Can disable the input events, i created this console app to fix mouse click my mouse was having a problem of double click in less than 100ms

🍵 If you like my work [buy me a coffe](https://www.buymeacoffee.com/richardgs) 🍵
