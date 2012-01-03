del ..\zenjiro\target\classes\com\wordpress\zenjiro\samurai\*.class
del ..\zenjiro\target\classes\jp\ac\waseda\cs\washi\samurai\api\*.class
mkdir ..\zenjiro\target
mkdir ..\zenjiro\target\classes
javac -cp ..\zenjiro\src\main\java -d ..\zenjiro\target\classes ..\zenjiro\src\main\java\com\wordpress\zenjiro\samurai\*.java -encoding UTF-8
