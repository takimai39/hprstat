hprstat
====

以前、 BEA 社が WebLogic/JavaVM のトラブルシューティング用として、HP-UX 用に hp_prstat というスレッド毎のCPU利用率を表示するツールを提供していました。
すでに、その hp_prstat へのリンクは無くなっていますので、同様のツールを作成してみました。

利用方法は hprstat <PID> として実行する事で、スレッド番号とCPU利用率を表示します。
  
## Compile

make hprstat

## Usage

./hprstat 4560  
commmand: java  
lwpid   pid     pri     status  UsrTime SysTime  
*-----   ---     ---     ----    ------  ------*  
16662   4560    154     1       2       0  
16663   4560    137     1       0       0  
16664   4560    137     1       0       0  
16665   4560    137     1       0       0  
16666   4560    137     1       0       0  
16667   4560    137     1       0       0  
16668   4560    154     1       0       0  
…  

## Other

なお、HPE の製品として HPE GlancePlus software を購入しインストールされていれば /opt/perf/bin/glance コマンドで、同様にスレッド毎の CPU 利用率を表示する事が可能です。
