# winnice
Windows用のプロセスの優先度を設定するコマンドラインアプリです

# 環境
Visual Studio 2019 ランタイムライブラリ

# インストール
ダウンロードしたアーカイブを実行して展開します

# ２つの実行ファイル
* *winnice.exe* はコンソールアプリです
* *winnicew.exe* はGUIアプリです（コンソールを表示しません）

# アンインストール
展開したファイルを削除します

# 使い方
### プロセスIDが12345のプロセスを*アイドル*にします

```
> winnice --cpu-idle --pid 12345
```


### プロセスIDはいくつでも指定できます
```
> winnice --cpu-idle --pid 12345,7890
```


### すべてのnotepad.exeの優先度を*アイドル*にします
```
> winnice --cpu-idle --executable notepad.exe
```


### 'app.exe'のIO優先度を*アイドル*にします
```
> winnice.exe --io-idle --executable app.exe
```


### メモ帳を起動しすべての優先度（CPU,IO,メモリ）を*アイドル*にせっていし、起動したプロセスを切り離します（終了を待ちません）
```
> winnice.exe --all-idle --detach-newprocess --new-process notepad
```

# サポート
ここ <https://github.com/ambiesoft/winnice/issues>　に問題を投稿するか掲示板に投稿してください

# コンタクト
- 開発者: trueff
- E-mail: <ambiesoft.trueff@gmail.com>
- ウェブサイト: <https://ambiesoft.com/>
- 掲示板: <https://ambiesoft.com/minibbs/>
- 開発: <https://github.com/ambiesoft/winnice>