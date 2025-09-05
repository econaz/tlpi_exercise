// 代码处在设置sig_ign和raise之间
// 如果在此期间来个一个SIGTSTP信号，会停止运行，
// SIGCONT后又会raise导致停止。
