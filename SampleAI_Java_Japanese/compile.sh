rm -rf ./bin/jp/ac/waseda/cs/washi/samurai/api/*.class
rm -rf ./bin/jp/ac/waseda/cs/washi/samurai/main/*.class
mkdir bin
javac -cp src -d bin src/jp/ac/waseda/cs/washi/samurai/main/*.java -encoding UTF-8