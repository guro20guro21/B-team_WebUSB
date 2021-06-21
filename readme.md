## This is a test repository. 
詳細→ https://hackmd.io/@maple117/rJTVJclIO

WebUSB API(Web Serial API)を使ってウェブブラウザとmicro:bitとの通信(シリアル通信)を行うのが目的です。

## WebUSB API
web.htmlについてですが、dap.umd.jsが無いと動きません
https://github.com/ARMmbed/dapjs ←ここから手に入れてください
入手方法とかはhackmdに書いてあります。

## WebSerialAPI始めました
接続時に設定するボーレートについて
PCの性能によってはボーレートが高すぎると送信時にデータが溢れる可能性があるので、そういう時はボーレートを下げてください。
ゲーミングPCとかなら115200でも問題なく送れると思います。
