set JDK_PATH=C:\Program Files (x86)\Java\jdk1.8.0_77

"%JDK_PATH%\bin\javac" -h ..\..\HeaderFiles ..\..\java\Eco\Core\*.java ..\..\java\Eco\System\*.java ..\..\java\Eco\InterfaceBus\*.java ..\..\java\Eco\MemoryManager\*.java ..\..\java\Eco\FileSystemManagement\*.java ..\..\java\Eco\Calculator\*.java
del /S ..\..\java\*.class
