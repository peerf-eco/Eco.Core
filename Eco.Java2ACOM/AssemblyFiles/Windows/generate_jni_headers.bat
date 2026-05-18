set JDK_PATH=C:\Program Files (x86)\Java\jdk1.8.0_77

"%JDK_PATH%\bin\javac" -h ..\..\HeaderFiles ..\..\SourceFiles\java\Eco\Core\IEcoUnknownNative.java ..\..\SourceFiles\java\Eco\System\CEcoSystem.java ..\..\SourceFiles\annotations\src\Eco\Annotations\*.java ..\..\SharedFiles\Eco\Core\*.java
del /S ..\..\SourceFiles\*.class
del /S ..\..\SharedFiles\*.class
