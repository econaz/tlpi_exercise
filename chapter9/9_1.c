

// ruid = 1000,euid = 0, suid = 0, fsuid = 0;
// setuid(2000) -> ruid = 2000,euid = 2000,suid = 2000, fsuid = 2000
// setreuid(-1,2000) -> ruid = 1000,euid = 2000,suid = 0,fsuid = 2000
// seteuid(2000) -> ruid = 1000, euid = 2000, suid = 2000, fsuid = 2000
// setfsuid(2000) -> ruid = 1000,euid = 0, suid = 0, fsuid = 2000
// setresuid(-1,2000,3000) -> ruid = 1000,euid = 2000,suid = 3000,fsuid = 0
