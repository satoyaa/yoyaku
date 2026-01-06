# 出力設定
set terminal pngcairo font "sans,12" size 800,600
set output 'temp5.png'

# グラフの基本設定
set grid
set xlabel "制限時間"
set ylabel "評価値"
set key outside

# y軸の範囲
set yrange [-300:200]

# X軸の範囲設定
# 行番号（0番目と1番目）を使用するため、範囲を少し広めに設定します
set xrange [-0.5:2.5]

# 描画のポイント:
# using 0  -> X座標として「行番号（0から開始）」を使用
# :i       -> Y座標として各列を使用
# :xtic(1) -> 1列目の文字列をX軸のラベルとして使用
plot for [i=2:5] 'data.dat' using 0:i:xtic(1) with linespoints lw 2 pt 7 title columnhead(i)