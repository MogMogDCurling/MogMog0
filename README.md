# MogMog

MogMog is an AI agent of the digital curling.

At GPW-cup2018 of digital curling, mogmog has taken part in this meeting.


## MogMog0（もぐもぐ）とは
もぐもぐはデジタルカーリングのエージェントです。ピョンチャンオリンピックの女子チームへのリスペクトの気持ちから名前を付けました。（「そだねー」はさすがに遠慮しました。）

## 基本アルゴリズム
１.	ショットに使用できる時間のうち6割を「３手モンテカルロ（2手目以降はランダムショット）」に費やし、ショットを5種類程度に絞り込み、残り4割の時間で「５手モンテカルロ」を行っています。

２.	ショットの候補（これが最も大事なところだと思いますが）は「言葉で説明できるショット」に限定しています。Guard shot, draw shot, hit-and-stay shot, freeze shot, hit-and-roll shotなど、通常のゲームで戦略として使われる用語をそのまま援用し、それを候補手としていますので、ショットの候補は30種類から多くて70種類くらいです。実行時に、どの種類のショットを行ったのか表示するようにしています。

３.	時間管理を行い、1手に費やせる時間を概算し、その時間の範囲内でモンテカルロを繰り返します。（マルチスレッド対応もしています。）

４.	盤面評価については、mogmog0では単純に「それが最終場面だとしたときのポイント」で計算しています。「mogmog1=盤面を言葉で説明できる範囲で評価する」「mogmog2 =盤面についての学習（NN）からデータベースを作成し、それを参照して評価する」というバージョンもあるのですが、mogmog0でも十分意味のある行動が見て取れますので、こちらを発表いたします。
