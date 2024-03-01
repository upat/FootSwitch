# FootSwitch

概要
---
Digisparkで作るn番煎じのフットスイッチデバイス

機能
---
- スイッチ押下でキー入力を行う
- コードの改造で以下の変更が可能
    - サンプリング周期/回数(デフォルト：4ms*5回)
    - 入力するキー(デフォルト：右Ctrl)
    - PressまたはStrokeの入力(デフォルト：Press)
    - スイッチ押下後、指定した時間入力無効にして連打防止(デフォルト：未設定)
    - 最大3スイッチまで増設可能(動作未確認)

開発・動作環境
---
- [Digispark](https://s3.amazonaws.com/digistump-resources/files/97a1bb28_DigisparkSchematic.pdf)
- Arduino IDE 2.3.2
- [オジデン OFL-S-Pシリーズ フットスイッチ](https://www.ojiden.co.jp/item/detail.html?itemId=I20130823001)

使い方
---
1. DigisparkをArduino IDEで使用可能な状態にする
1. このコードをDigisparkに書き込む
1. フットスイッチから出ている2本の線をPB0とGNDに接続(適当なジャンパーワイヤとはんだで接合して熱収縮チューブで加工した)

作成例
---
![pic1](https://github.com/upat/FootSwitch/blob/master/images/pic1.png)

- USB配線
    - USB 5V ⇔ 5V
    - USB D+ ⇔ PB4(USB-)
    - USB D- ⇔ PB3(USB+)
    - USB GND ⇔ GND
- 使用したケース
    - [ダイソー 並べてカスタム小物収納ケース インナーＥ](https://jp.daisonet.com/products/4973430024341?_pos=438&_sid=b0a6c5fcc&_ss=r)

使用上の注意
---
Digisparkの特性としてUSBハブを介して接続するとデバイスとして認識できない場合があります

参考
---
[Qiita Digispark ATtiny85 にスイッチを付ける場合のコツ](https://qiita.com/nak435/items/5e9320342729ae76e40d)

ライセンス
---
MIT Licence
