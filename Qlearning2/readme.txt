★最短経路を検索

S: スタート
G: ゴール
H: 穴(hole)
o: フリー


// 地図
block BlockMap1[] = {
 S, o, H, o, H, o, o, o,
 o, o, H, o, H, o, H, o,
 H, o, o, o, H, o, H, o,
 o, o, H, o, H, o, H, o,
 o, H, o, o, o, o, H, o,
 o, H, o, H, o, o, H, o,
 o, o, o, H, o, H, o, o,
 o, o, o, H, o, H, o, G
};


//以下　結果の例
...
cnt=22 状態価値の変化5.6345e-07
cnt=22 状態価値の変化5.02914e-07
cnt=22 状態価値の変化4.49363e-07
cnt=22 状態価値の変化4.02797e-07
cnt=22 状態価値の変化3.60422e-07
cnt=22 状態価値の変化3.21772e-07
cnt=22 状態価値の変化2.8545e-07
action:右 s_position:0->s_next_position:1 cnt=1
action:下 s_position:1->s_next_position:9 cnt=2
action:下 s_position:9->s_next_position:17 cnt=3
action:右 s_position:17->s_next_position:18 cnt=4
action:右 s_position:18->s_next_position:19 cnt=5
action:下 s_position:19->s_next_position:27 cnt=6
action:下 s_position:27->s_next_position:35 cnt=7
action:右 s_position:35->s_next_position:36 cnt=8
action:右 s_position:36->s_next_position:37 cnt=9
action:上 s_position:37->s_next_position:29 cnt=10
action:上 s_position:29->s_next_position:21 cnt=11
action:上 s_position:21->s_next_position:13 cnt=12
action:上 s_position:13->s_next_position:5 cnt=13
action:右 s_position:5->s_next_position:6 cnt=14
action:右 s_position:6->s_next_position:7 cnt=15
action:下 s_position:7->s_next_position:15 cnt=16
action:下 s_position:15->s_next_position:23 cnt=17
action:下 s_position:23->s_next_position:31 cnt=18
action:下 s_position:31->s_next_position:39 cnt=19
action:下 s_position:39->s_next_position:47 cnt=20
action:下 s_position:47->s_next_position:55 cnt=21
action:下 s_position:55->s_next_position:63 cnt=22
