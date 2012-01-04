================================================================================
                   ■□■ WASEDA × GREE Samurai coding ■□■
================================================================================

                         WASEDA × GREE Samurai coding は
         グリー株式会社様の協賛で開催されるプログラミングコンテストです．
                     最強のAIを作成して優勝を目指しましょう！

================================================================================

■README
このファイルでは，主にゲームの起動方法について説明します．
コンテスト，問題の詳細については，必ずwebのドキュメントを参照ください．


■ゲーム本体の起動方法
このファイルのあるフォルダをカレントディレクトリとして，
以下のコマンドを実行することでゲームを起動します．

> java -jar SamuraiCoding2011-2.0.4.jar [options] command

option として以下の引数を指定できます．
 -a <arg> スコア送信用のアクセストークンを指定します.
 -c <arg> 各キャラクターの処理時間を指定します．デフォルト値は 1000[ms] です．
 -d       デバッグモードで起動します．
 -f <arg> ゲームスピードを調整する FPS を指定します．デフォルト値は 30[fps]です．
 -h       ヘルプを表示します．
 -n       CUIモードで起動します.
 -t <arg> ゲームのターン数を指定します．デフォルト値は 200[turns] です．
 -u       ユーザーモードで起動します．
          (AIを使用せず，キーボード操作でゲームが進行します)
		Player 0 Samurai ↑,→,←,↓
		Player 0 Dog      C, V, Z, X
		Player 1 Samurai  3, 4, 1, 2
		Player 1 Dog      7, 8, 5, 6
		Player 2 Samurai  E, R, Q, W
		Player 2 Dog      U, I, T, Y
		Player 3 Samurai  D, F, A, S
		Player 3 Dog      J, K, G, H

command に起動するAIの実行コマンドを指定します．コマンドは "" で括って指定します．
(コマンド例）
Java：
java -jar SamuraiCoding2011-2.0.4.jar "java -cp SampleAI_Java_Japanese/bin jp.ac.waseda.cs.washi.samurai.main.Main"

C：
java -jar SamuraiCoding2011-2.0.4.jar "ai.exe"

Ruby：
java -jar SamuraiCoding2011-2.0.4.jar "ruby ai.rb"


■サンプルAI(Java)
サンプルとしてJavaで記述したAIを同梱しています．
付属のバッチファイルで実行してください．

SampleAI_Java_Japanese/compile.bat(.sh):
　サンプルAIをクリーン，コンパイルします．
SampleAI_Java_Japanese/run.bat(.sh):
　サンプルAIを指定してゲームを起動します．

■更新履歴
2.0.4   SHOGUNモード状態の時間処理修正, バグ修正, -nオプションの追加(2012.1.1)
2.0.3   最新バージョンの本体からのみスコア送信を受け付けるよう修正(2011.12.26)
2.0.1   スコア送信機能の追加(2011.12.24)
2.0.0   ゲーム本体を予選用にアップデート(2011.12.23)
1.0.0   初回リリース(2011.12.16)

動作確認：JRE6(windowsXP, windows7, MacOSX10.6, MacOSX10.7, debian squeeze)
--------------------------------------------------------------------------------
WASEDA × GREE Samurai coding(http://www.washi.cs.waseda.ac.jp/samurai_coding/)
